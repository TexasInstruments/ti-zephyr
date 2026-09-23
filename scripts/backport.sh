#!/usr/bin/env bash

set -euo pipefail

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

VERSION="4.4.0"

UPSTREAM_REMOTE="upstream"
TARGET_REMOTE="ti"

UPSTREAM_REPO="zephyrproject-rtos/zephyr"
UPSTREAM_BRANCH="main"

TARGET_BRANCH="v${VERSION}-ti-next"

UPSTREAM_TRACKING="${UPSTREAM_REMOTE}/${UPSTREAM_BRANCH}"
TARGET_TRACKING="${TARGET_REMOTE}/${TARGET_BRANCH}"

MODE="auto"
PR=""

RANGE=""

# ---------------------------------------------------------------------------
# Arguments
# ---------------------------------------------------------------------------

while [[ $# -gt 0 ]]; do
    case "$1" in
        --upstream)
            MODE="upstream"
            shift
            ;;

        --ti)
            MODE="ti"
            shift
            ;;

        --pr)
            MODE="pr"

            if [[ $# -ge 2 && "$2" =~ ^[0-9]+$ ]]; then
                PR="$2"
                shift 2
            else
                shift
            fi
            ;;

        --fetch)
            git fetch "$UPSTREAM_REMOTE" "$UPSTREAM_BRANCH" --prune
            git fetch "$TARGET_REMOTE" "$TARGET_BRANCH" --prune
            shift
            ;;

        --help|-h)
            echo "usage: $0 [mode] [--fetch] <sha | sha1..sha2>"
            echo
            echo "Modes:"
            echo "  (default)          Automatically determine UPSTREAM/UPSTREAM-PEND/TI"
            echo "  --upstream         Force UPSTREAM"
            echo "  --pr [number]      Force UPSTREAM-PEND with optional PR number"
            echo "  --ti               Force TI"
            exit 0
            ;;

        -*)
            echo "error: unknown option: $1" >&2
            exit 1
            ;;

        *)
            [[ -z "$RANGE" ]] || {
                echo "error: only one commit/range may be specified" >&2
                exit 1
            }

            RANGE="$1"
            shift
            ;;
    esac
done

if [[ -z "$RANGE" && -n "$PR" ]]; then
    echo "No range provided. Fetching commit boundaries directly from PR #$PR..."

    PR_SHAS=$(gh pr view "$PR" --repo "$UPSTREAM_REPO" --json commits --jq '.commits[].oid' 2>/dev/null) || {
        echo "error: failed to retrieve commits for PR #$PR." >&2
        exit 1
    }

    if [[ -z "$PR_SHAS" ]]; then
        echo "error: PR #$PR contains no commits" >&2
        exit 1
    fi

    mapfile -t TMP_SHAS <<< "$PR_SHAS"

    FIRST_PR_SHA="${TMP_SHAS[0]}"
    LAST_PR_SHA="${TMP_SHAS[-1]}"

    # construct standard git range expression
    RANGE="${FIRST_PR_SHA}^..${LAST_PR_SHA}"
    echo "--> Resolved PR range string: $RANGE"
fi

[[ -n "$RANGE" ]] || {
    echo "usage: $0 [--upstream|--pr [number]|--ti] [--fetch] <sha | sha1..sha2>" >&2
    exit 1
}


# ---------------------------------------------------------------------------
# Configuration output
# ---------------------------------------------------------------------------

echo
echo "============================================================"
echo "Backport configuration"
echo "============================================================"
printf '%-16s: %s\n' "Version" "$VERSION"
echo
printf '%-16s: %s\n' "Upstream repo" "$UPSTREAM_REPO"
printf '%-16s: %s\n' "Upstream remote" "$UPSTREAM_REMOTE"
printf '%-16s: %s\n' "Upstream branch" "$UPSTREAM_BRANCH"
echo
printf '%-16s: %s\n' "Target remote" "$TARGET_REMOTE"
printf '%-16s: %s\n' "Target branch" "$TARGET_BRANCH"
echo
printf '%-16s: %s\n' "Mode" "$MODE"
[[ -n "$PR" ]] && printf '%-16s: #%s\n' "PR" "$PR"
printf '%-16s: %s\n' "Range" "$RANGE"
echo "============================================================"
echo

if [[ "$MODE" == "auto" ]]; then
    echo "!! WARNING: auto mode will search upstream and GitHub to determine the mode !!"
    echo
fi


# ---------------------------------------------------------------------------
# Checks
# ---------------------------------------------------------------------------

git diff --quiet && git diff --cached --quiet || {
    echo "error: working tree is dirty" >&2
    exit 1
}

git rev-parse "$UPSTREAM_TRACKING" >/dev/null || {
    echo "error: cannot find $UPSTREAM_TRACKING" >&2
    exit 1
}

# ---------------------------------------------------------------------------
# Resolve commits
# ---------------------------------------------------------------------------

if [[ "$RANGE" != *..* ]]; then
    git rev-parse --verify "${RANGE}^{commit}" >/dev/null || {
        echo "error: invalid commit: $RANGE" >&2
        exit 1
    }

    RANGE="${RANGE}^..${RANGE}"
fi

mapfile -t COMMITS < <(
    git rev-list --reverse --no-merges "$RANGE"
)

((${#COMMITS[@]})) || {
    echo "error: range contains no commits" >&2
    exit 1
}

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
#
ask_yes_no() {
    local prompt="${1:-Proceed} [y/N] "
    local reply

    while true; do
        read -r -p "$prompt" reply
        case "$reply" in
            [yY][eE][sS]|[yY]) return 0 ;;
            [nN][oO]|[nN]|"")  return 1 ;;
            *) echo "Please answer yes or no." ;;
        esac
    done
}

find_upstream()
{
    local sha="$1"

    local upstream_sha
    upstream_sha=$(git log "${sha}...${UPSTREAM_TRACKING}" \
        --cherry-mark --right-only\
        --format='%H %m' | awk '$2 == "=" {print $1; exit}')

    if [[ -n "$upstream_sha" ]]; then
        echo "$upstream_sha"
        return 0
    fi

    return 1
}

find_pr()
{
    local sha="$1"
    local pr

    pr="$(
        gh api \
            "/repos/$UPSTREAM_REPO/commits/$sha/pulls" \
            --jq '.[] | select(.state == "open") | .number'
    )"

    if [[ -n "$pr" ]]; then
        echo "${pr%%$'\n'*}"
        return 0
    fi

    pr="$(
        gh pr list \
            --repo "$UPSTREAM_REPO" \
            --state open \
            --search "$sha" \
            --json number \
            --jq '.[0].number'
    )"

    [[ -n "$pr" ]] && echo "${pr%%$'\n'*}"
}

get_pr_title() {
    local pr_num="$1"
    gh pr view "$pr_num" --repo "$UPSTREAM_REPO" --json title --jq '.title' 2>/dev/null || echo "Unknown Title"
}

make_message()
{
    local kind="$1"
    local sha="$2"
    local extra="${3:-}"
    local subject
    local body

    subject="$(git show -s --format='%s' "$sha")"
    body="$(git show -s --format='%b' "$sha")"

    case "$kind" in
        upstream)
            subject="UPSTREAM: $subject"
            ;;

        pending)
            subject="UPSTREAM-PEND: $subject"
            ;;

        ti)
            subject="TI: $subject"
            ;;
    esac

    if (( ${#subject} > 75 )); then
        subject="${subject:0:72}..."
    fi

    printf '%s\n\n' "$subject"

    case "$kind" in
        upstream)
            printf 'commit %s upstream.\n\n' "$extra"
            ;;

        pending)
            printf 'PR: %s\n\n' "$extra"
            ;;
    esac

    [[ -n "$body" ]] && printf '%s\n' "$body"
}

# ---------------------------------------------------------------------------
# Backport
# ---------------------------------------------------------------------------

handle_upstream() {
    local sha="$1"

    if git branch -r --contains "$sha" 2>/dev/null | grep -qE "(^|\s)$UPSTREAM_TRACKING$"; then
        echo "$sha"
        return 0
    elif upstream="$(find_upstream "$sha")"; then
        echo "$upstream"
        return 0
    fi

    return 1
}


resolve_pr() {
    local sha="$1"
    local pr_title

    # If the user already provided a PR, we are done
    if [[ -n "$PR" ]]; then
        return 0
    fi

    # Otherwise find PR #
    PR=$(find_pr "$sha") || return 1

    # Fetch the title
    pr_title="$(get_pr_title "$PR")"

    if ask_yes_no "Proceed with discovered PR #$PR: \"$pr_title\"?"; then
        return 0
    fi

    PR=""
    return 1
}

ORIG_HEAD="$(git rev-parse HEAD)"
FIRST_SHA="${COMMITS[0]}"

if [[ "$MODE" == "pr" ]]; then
    if ! resolve_pr "$FIRST_SHA"; then
        echo "error: No PR resolved." >&2
        exit 1
    fi
elif [[ "$MODE" == "auto" ]]; then
    if handle_upstream "$FIRST_SHA"; then
        MODE="upstream"
    elif resolve_pr "$FIRST_SHA"; then
        MODE="pr"
    else
        MODE="ti"
    fi

    echo "--> Automatically selected execution mode: [${MODE}] <--"
fi

for sha in "${COMMITS[@]}"; do
    short="$(git rev-parse --short "$sha")"
    subject="$(git show -s --format='%s' "$sha")"

    echo
    echo "== $short: $subject =="

    fi

    case "$MODE" in
        upstream)
            extra="$sha"
            kind="upstream"

            extra=$(handle_upstream "$sha") || {
                if ask_yes_no "No patch in $UPSTREAM_TRACKING, continue with provided SHA $sha?"; then
                    extra="$sha"
                else
                    echo "Not proceeding"
                    exit 0
                fi
            }
            ;;

        pr)
            kind="pending"
            extra="$PR"
            ;;

        ti)
            kind="ti"
            extra=""
            ;;
    esac

    tmp="$(mktemp)"

    make_message "$kind" "$sha" "$extra" > "$tmp"

    author="$(git show -s --format='%an <%ae>' "$sha")"

    git cherry-pick --no-commit "$sha" || {
        echo
        echo "Cherry-pick conflict."
        echo "Resolve the conflict, then:"
        echo "  git add <files>"
        echo "  git commit --author=\"$author\"  --file='$tmp'"
        echo
        echo "To completely abort and roll back EVERYTHING done so far:"
        echo "  git reset --hard $ORIG_HEAD"
        exit 1
    }

    if git diff --cached --quiet; then
        echo "-- skipping commit: empty diff --"
        rm -f "$tmp"
        continue
    fi

    git commit --author="$author" --file="$tmp"
    rm -f "$tmp"
done

echo "Done."
