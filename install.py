#!/usr/bin/env python3
"""
TI Zephyr SDK — GitHub install (developer / technically capable users)

Standard install:
    mkdir ~/ti/zephyr_ti_sdk_v1.0.0
    cd ~/ti/zephyr_ti_sdk_v1.0.0
    git clone https://github.com/TexasInstruments/ti-zephyr ti-zephyr
    cd ti-zephyr
    python3 install.py

After install, workspace is in the parent directory (~/ti/zephyr_ti_sdk_v1.0.0/):
    ti-zephyr/        ← this repo (manifest)
    zephyr/           ← Zephyr RTOS (via west)
    modules/          ← HAL, crypto, etc. (via west)
    .venv/            ← Python tools (west, pyserial, imgtool, ...)
    toolchains/       ← compiler + openocd (on-demand)
    env_setup.sh      ← activate SDK environment

Activate (every new shell):
    source ~/ti/zephyr_ti_sdk_v1.0.0/env_setup.sh
    .    ~/ti/zephyr_ti_sdk_v1.0.0/env_setup.ps1    # PowerShell
         ~/ti/zephyr_ti_sdk_v1.0.0/env_setup.bat    # CMD

Standalone (curl, no prior clone):
    curl -fsSL https://raw.githubusercontent.com/TexasInstruments/ti-zephyr/main/install.py | python3 -

Options:
    --prefix ~/ti           Workspace root for standalone mode (default: ~/ti)
    --branch main           Branch/tag for standalone GitHub clone (default: main)
    --non-interactive       Skip all Y/n prompts; only do workspace + venv
    --dry-run               Print steps without executing

Tools can always be set up later:
    python3 ti-zephyr/scripts/setup_toolchain.py
    python3 ti-zephyr/scripts/setup_openocd.py
"""
import argparse
import subprocess
import sys
from pathlib import Path

_HERE    = Path(__file__).parent.resolve()   # ti-zephyr/ directory
_IN_REPO = (_HERE / ".git").exists()         # True when running inside a clone

# Inline defaults so install.py works standalone (before scripts/ is available)
MANIFEST_URL   = "https://github.com/TexasInstruments/ti-zephyr"
SDK_NG_VERSION = "1.0.1"

if _IN_REPO:
    sys.path.insert(0, str(_HERE))
    try:
        from scripts.sdk_versions import SDK_NG_VERSION, MANIFEST_URL
    except ImportError:
        pass


def run(*cmd, cwd=None, dry=False):
    print(f"  $ {' '.join(str(c) for c in cmd)}")
    if not dry:
        subprocess.run(cmd, cwd=cwd, check=True)


def _current_branch():
    """Return current git branch name when running inside a clone."""
    try:
        r = subprocess.run(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            capture_output=True, text=True, cwd=str(_HERE),
        )
        return r.stdout.strip() or "unknown"
    except Exception:
        return "unknown"


def _prompt_yn(question, default_yes=True):
    """Prompt user for Y/n. Returns True for yes."""
    hint = "[Y/n]" if default_yes else "[y/N]"
    try:
        answer = input(f"\n  {question} {hint} ").strip().lower()
    except (EOFError, KeyboardInterrupt):
        print()
        return False
    if not answer:
        return default_yes
    return answer.startswith("y")


def setup(prefix, branch, non_interactive, dry):
    if _IN_REPO:
        sdk = _HERE.parent   # workspace root = parent of the clone
    else:
        sdk = Path(prefix).expanduser().resolve()

    venv = sdk / ".venv"
    py   = venv / ("Scripts" if sys.platform == "win32" else "bin") / "python"
    pip  = venv / ("Scripts" if sys.platform == "win32" else "bin") / "pip"
    clone_name = _HERE.name if _IN_REPO else "ti-zephyr"

    print(f"\nTI Zephyr SDK workspace  →  {sdk}")
    if _IN_REPO:
        print(f"  Branch     : {_current_branch()}")
    print()

    if not dry:
        sdk.mkdir(parents=True, exist_ok=True)

    # ── Step 1: Python venv ────────────────────────────────────────────────
    if not venv.exists():
        print("[1/5] Creating Python virtual environment ...")
        run(sys.executable, "-m", "venv", str(venv), dry=dry)
        run(str(pip), "install", "-q", "west", "pyserial", dry=dry)
    else:
        print("[1/5] Python venv already exists — skipping")

    # ── Step 2: west workspace init ────────────────────────────────────────
    if not (sdk / ".west" / "config").exists():
        print("[2/5] Initialising west workspace ...")
        if _IN_REPO:
            run(str(py), "-m", "west", "init", "-l", str(_HERE), dry=dry)
        else:
            run(str(py), "-m", "west", "init",
                "--mr", branch, "-m", MANIFEST_URL, str(sdk), dry=dry)
    else:
        print("[2/5] west workspace already initialised — skipping west init")

    # ── Step 3: west update ────────────────────────────────────────────────
    print("[3/5] Fetching west projects (zephyr, modules, bootloader, tools) ...")
    run(str(py), "-m", "west", "update", cwd=str(sdk), dry=dry)

    # ── Step 4: Zephyr host tools ──────────────────────────────────────────
    print("[4/5] Installing Zephyr host tools ...")
    req = sdk / "zephyr" / "scripts" / "requirements.txt"
    if req.exists() or dry:
        run(str(pip), "install", "-q", "-r", str(req), dry=dry)
    else:
        print("  [WARN] zephyr/scripts/requirements.txt not found — run again after west update")

    # ── Step 5: env setup wrappers ─────────────────────────────────────────
    print("[5/5] Generating environment setup scripts ...")
    _write_env(sdk, clone_name, dry)

    # ── Interactive: Zephyr SDK NG toolchain ───────────────────────────────
    if not non_interactive:
        if _prompt_yn("Set up Zephyr SDK NG compiler toolchain?"):
            _setup_toolchain(sdk, py, dry)
        else:
            print("  Skipped. Run later: python3 ti-zephyr/scripts/setup_toolchain.py")

    # ── Interactive: TI OpenOCD ────────────────────────────────────────────
    if not non_interactive:
        if _prompt_yn("Set up TI custom OpenOCD for west flash on TI boards?"):
            _setup_openocd(sdk, py, dry)
        else:
            print("  Skipped. Run later: python3 ti-zephyr/scripts/setup_openocd.py")

    # ── Summary ────────────────────────────────────────────────────────────
    if not dry:
        _print_summary(sdk, clone_name)
    else:
        print("\n[dry-run] No changes made.\n")


def _setup_toolchain(sdk, py, dry):
    """Call setup_toolchain.py to install or symlink Zephyr SDK NG."""
    tc_script = (sdk / (_HERE.name if _IN_REPO else "ti-zephyr")) / "scripts" / "setup_toolchain.py"
    if tc_script.exists() or dry:
        run(str(py), str(tc_script), "--sdk", str(sdk), dry=dry)
    else:
        print("  [WARN] setup_toolchain.py not found — do 'west update' first")


def _setup_openocd(sdk, py, dry):
    """Call setup_openocd.py to clone and build TI custom OpenOCD."""
    oc_script = (sdk / (_HERE.name if _IN_REPO else "ti-zephyr")) / "scripts" / "setup_openocd.py"
    if oc_script.exists() or dry:
        run(str(py), str(oc_script), "--sdk", str(sdk), dry=dry)
    else:
        print("  [WARN] setup_openocd.py not found — do 'west update' first")


def _write_env(sdk, clone_name, dry=False):
    """Write env_setup wrappers at workspace root."""
    if dry:
        return
    rel     = f"{clone_name}/scripts/env_setup.py"
    win_rel = rel.replace("/", "\\")

    sh = sdk / "env_setup.sh"
    sh.write_text(
        "#!/bin/bash\n"
        "# TI Zephyr SDK environment setup — Linux/macOS\n"
        "# Usage: source env_setup.sh\n"
        f'eval "$(python3 "$(dirname "${{BASH_SOURCE[0]}}")/{rel}")"\n'
        'echo "TI Zephyr SDK ready."\n'
        'echo "  ZEPHYR_BASE=$ZEPHYR_BASE"\n'
        'echo "  ZEPHYR_SDK_INSTALL_DIR=$ZEPHYR_SDK_INSTALL_DIR"\n'
    )
    sh.chmod(0o755)
    (sdk / "env_setup.ps1").write_text(
        "# TI Zephyr SDK environment setup — PowerShell\n"
        "# Usage: . env_setup.ps1\n"
        f'python3 "$PSScriptRoot\\{win_rel}" --shell powershell | Invoke-Expression\n'
        'Write-Host "TI Zephyr SDK ready."\n'
    )
    (sdk / "env_setup.bat").write_text(
        "@echo off\n"
        "REM TI Zephyr SDK environment setup -- CMD\n"
        "REM Usage: env_setup.bat\n"
        f'FOR /F "usebackq tokens=*" %%i IN (`python3 "%~dp0{win_rel}" --shell cmd`) DO %%i\n'
        'echo TI Zephyr SDK ready.\n'
    )


def _print_summary(sdk, clone_name):
    tc_dir  = next((sdk / "toolchains").glob("zephyr-sdk-*"), None) if (sdk / "toolchains").exists() else None
    oc_bin  = sdk / "toolchains" / "openocd" / "src" / "openocd"
    sep = "─" * 60
    print(f"\n{sep}")
    print(f"  TI Zephyr SDK workspace: {sdk}")
    print(f"  ✓ west workspace + venv")
    print(f"  {'✓' if tc_dir else '✗'} Zephyr SDK NG toolchain"
          + ("" if tc_dir else "  ← run: python3 ti-zephyr/scripts/setup_toolchain.py"))
    print(f"  {'✓' if oc_bin.exists() else '✗'} TI OpenOCD"
          + ("" if oc_bin.exists() else "  ← run: python3 ti-zephyr/scripts/setup_openocd.py"))
    print(sep)
    print(f"\n  Activate:  source {sdk}/env_setup.sh")
    print(f"  Build:     west build -b lp_mspm0g3519 {clone_name}/samples/basic/blinky")
    print(f"  Flash:     west flash")
    print(f"  Test:      west twister -p lp_mspm0g3519 "
          f"--hardware-map {clone_name}/tests/boards/lp_mspm0g3519/map.yml\n")


if __name__ == "__main__":
    p = argparse.ArgumentParser(
        description="TI Zephyr SDK — GitHub install",
        epilog=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("--prefix",           default=str(Path.home() / "ti"),
                   help="Workspace root for standalone mode (default: ~/ti)")
    p.add_argument("--branch",           default="main",
                   help="Branch/tag for standalone clone (default: main)")
    p.add_argument("--non-interactive",  action="store_true",
                   help="Skip toolchain/openocd prompts (workspace + venv only)")
    p.add_argument("--dry-run",          action="store_true",
                   help="Print steps without executing")
    a = p.parse_args()
    setup(a.prefix, a.branch, a.non_interactive, a.dry_run)
