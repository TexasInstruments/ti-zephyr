# Contributing to TI Zephyr SDK

## Branch model

| Branch | Purpose | Who targets it |
|--------|---------|----------------|
| `main` | Stable. SDK users clone from here. Release tags. | `next` only (via PR) |
| `next` | Integration. All feature PRs merge here. | `feature/*` branches |
| `feature/*` | Individual features and fixes. | developer |

**Rule:** Never commit directly to `main`. PRs always target `next`.
`main` is promoted from `next` via a PR after sufficient testing.

---

## Feature branch workflow

```bash
# 1. Create versioned SDK dir and clone from next
mkdir ~/ti/zephyr_ti_sdk_next
cd ~/ti/zephyr_ti_sdk_next
git clone https://github.com/TexasInstruments/ti-zephyr --branch next ti-zephyr
cd ti-zephyr

# 2. Create your feature branch
git checkout -b feature/my-feature

# 3. Set up the SDK workspace (works on any branch)
python3 install.py

# 4. Work, build, test
source ~/ti/env_setup.sh
west build -b lp_mspm0g3519 next_workspace/samples/basic/blinky

# 5. Commit (Signed-off-by required)
git add ...
git commit -s -m "feat(boards): add lp_mspm33c321a support"

# 6. Push and open PR targeting 'next'
git push origin feature/my-feature
# Open PR: feature/my-feature → next
```

---

## Commit message convention

```
<type>(<scope>): <short description>

[optional body]

Signed-off-by: Your Name <your@email.com>
```

**Types:** `feat`, `fix`, `docs`, `refactor`, `test`, `ci`, `chore`

**Scope examples:** `boards`, `drivers`, `samples`, `install`, `west`, `openocd`, `tests`

`Signed-off-by` is **required** on every commit (DCO).

---

## PR rules

- Target branch: always `next` (never `main` directly)
- All builds must pass before merge
- At least one reviewer approval required
- Squash or rebase preferred over merge commits

---

## Replacing a fork placeholder

When TI provides an official downstream fork URL:

```bash
# 1. Update the remote in west.yml:
#    Find: PLACEHOLDER_TI_ZEPHYR_FORK
#    Replace url-base with actual TI fork URL

# 2. Update revision in scripts/sdk_versions.py:
#    TI_ZEPHYR_REVISION = "v4.4.0-ti.1"  # pin to tested branch/tag

# 3. Uncomment the project entry in west.yml (for hal_ti, mcuboot, tools)

# 4. Test the change:
west update
west build -b lp_mspm0g3519 ti-zephyr/samples/basic/blinky

# 5. Find remaining placeholders:
grep -r "PLACEHOLDER_" ti-zephyr/

# 6. Commit to feature/wire-ti-<name>-fork, PR to next
```

---

## Upstreaming TI content to Zephyr

When content in `ti-zephyr/` is ready for upstream Zephyr (boards, drivers, DTS, samples):

```bash
# Move from ti-zephyr → zephyr
cp -r ti-zephyr/boards/ti/lp_mspm33c321a/ zephyr/boards/ti/
rm -rf ti-zephyr/boards/ti/lp_mspm33c321a/
git -C zephyr add boards/ti/lp_mspm33c321a/
git -C zephyr commit -s -m "boards: ti: add lp_mspm33c321a LaunchPad"
# Push to a branch on the TI zephyr fork, open PR to zephyrproject-rtos/zephyr
```

---

## SDK install on any branch

Pattern: `mkdir <sdk-dir> && cd <sdk-dir> && git clone URL [--branch <branch>] ti-zephyr && cd ti-zephyr && python3 install.py`

```bash
# main (stable release)
mkdir ~/ti/zephyr_ti_sdk_v1.0.0
cd ~/ti/zephyr_ti_sdk_v1.0.0
git clone https://github.com/TexasInstruments/ti-zephyr ti-zephyr
cd ti-zephyr && python3 install.py

# next (integration)
mkdir ~/ti/zephyr_ti_sdk_next
cd ~/ti/zephyr_ti_sdk_next
git clone https://github.com/TexasInstruments/ti-zephyr --branch next ti-zephyr
cd ti-zephyr && python3 install.py

# feature branch
mkdir ~/ti/zephyr_ti_sdk_feature_xyz
cd ~/ti/zephyr_ti_sdk_feature_xyz
git clone https://github.com/TexasInstruments/ti-zephyr --branch feature/xyz ti-zephyr
cd ti-zephyr && python3 install.py
```

`install.py` always uses the parent directory as the workspace root and shows
the current branch in the header. Branch selection does not affect workspace setup.
