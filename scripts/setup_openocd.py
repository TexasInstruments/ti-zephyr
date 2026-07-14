#!/usr/bin/env python3
"""
Set up TI custom OpenOCD for west flash on TI boards.

Clones https://github.com/TexasInstruments/ti-openocd and builds it,
then configures .west/config so west flash works without extra arguments.

Usage:
    python3 scripts/setup_openocd.py                   # uses default remote
    python3 scripts/setup_openocd.py --remote <url>    # override remote URL
    python3 scripts/setup_openocd.py --sdk ~/ti/       # explicit SDK root
"""

import argparse
import configparser
import os
import subprocess
import sys
from pathlib import Path

_HERE = Path(__file__).parent.resolve()  # scripts/

try:
    from scripts.sdk_versions import OPENOCD_BRANCH, OPENOCD_URL
except ImportError:
    try:
        sys.path.insert(0, str(_HERE.parent))
        from scripts.sdk_versions import OPENOCD_BRANCH, OPENOCD_URL
    except ImportError:
        OPENOCD_BRANCH = "master"
        OPENOCD_URL    = "https://github.com/TexasInstruments/ti-openocd"


def _detect_sdk_root(explicit=None):
    if explicit:
        return Path(explicit).expanduser().resolve()
    return _HERE.parent.parent  # scripts/ → ti-zephyr/ → sdk_root/


def _run(*cmd, cwd=None, dry=False):
    print(f"  $ {' '.join(str(c) for c in cmd)}")
    if not dry:
        subprocess.run(cmd, cwd=cwd, check=True)


def _prompt_remote():
    """Return default OpenOCD remote, prompting only if env var not set."""
    default = os.environ.get("OPENOCD_REMOTE", OPENOCD_URL)
    print(f"\n  TI OpenOCD remote: {default}")
    print("  Press Enter to use default, or type a different URL.")
    try:
        remote = input("  Remote URL: ").strip()
    except (EOFError, KeyboardInterrupt):
        print()
        return default
    return remote or default


def main(sdk_root=None, remote=None, branch=None, dry=False):
    sdk    = _detect_sdk_root(sdk_root)
    branch = branch or OPENOCD_BRANCH
    oc_dir = sdk / "toolchains" / "openocd"
    binary = oc_dir / "src" / "openocd"

    print(f"\n[setup_openocd] SDK root:   {sdk}")
    print(f"[setup_openocd] Target dir: {oc_dir}")
    print(f"[setup_openocd] Branch:     {branch}\n")

    # Get remote URL
    if not remote:
        remote = os.environ.get("OPENOCD_REMOTE", "")
    if not remote:
        remote = _prompt_remote()
    if not remote:
        print("  No remote provided — skipping OpenOCD setup.")
        print("  Run later: python3 scripts/setup_openocd.py --remote <URL>")
        return

    # Clone or update the openocd repo
    if not oc_dir.exists():
        print(f"  Cloning OpenOCD from {remote} ...")
        if not dry:
            oc_dir.parent.mkdir(parents=True, exist_ok=True)
        _run("git", "clone", remote, str(oc_dir), dry=dry)

    # Fetch and checkout the TI branch
    print(f"  Checking out branch {branch} ...")
    _run("git", "fetch", remote, dry=dry, cwd=oc_dir)
    _run("git", "checkout", branch, dry=dry, cwd=oc_dir)
    _run("git", "reset", "--hard", f"origin/{branch}", dry=dry, cwd=oc_dir)

    result = subprocess.run(["git", "log", "--oneline", "-1"],
                            cwd=oc_dir, capture_output=True, text=True)
    print(f"  openocd HEAD: {result.stdout.strip()}")

    # Build if binary missing
    if not binary.exists():
        print("  Building OpenOCD (bootstrap → configure → make) ...")
        _run("./bootstrap", cwd=oc_dir, dry=dry)
        _run("./configure", "--disable-werror", cwd=oc_dir, dry=dry)
        jobs = max(1, os.cpu_count() // 2)
        _run("make", f"-j{jobs}", cwd=oc_dir, dry=dry)
    else:
        print(f"  Binary already exists: {binary}")

    # Configure .west/config so west flash uses this openocd
    _configure_west_runner(sdk, binary, oc_dir, dry)

    print(f"\n  OpenOCD ready: {binary}")
    print(f"  west flash will use it automatically.\n")


def _configure_west_runner(sdk, binary, oc_dir, dry=False):
    """Write [runner.openocd] section to .west/config."""
    west_cfg_path = sdk / ".west" / "config"
    if not west_cfg_path.exists():
        print(f"  [WARN] {west_cfg_path} not found — west init not done yet?")
        return

    print(f"  Configuring .west/config for west flash ...")
    cfg = configparser.ConfigParser()
    cfg.read(west_cfg_path)

    section = "runner.openocd"
    if not cfg.has_section(section):
        cfg.add_section(section)

    args = (
        f"--openocd {binary} "
        f"--openocd-search {oc_dir / 'tcl'}"
    )
    cfg.set(section, "args", args)

    if not dry:
        with open(west_cfg_path, "w") as f:
            cfg.write(f)
        print(f"  Written: {west_cfg_path}")
        print(f"  [runner.openocd] args = {args}")
    else:
        print(f"  [dry] would write [runner.openocd] args = {args}")


if __name__ == "__main__":
    p = argparse.ArgumentParser(
        description="Set up TI custom OpenOCD",
        epilog=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("--sdk",     default=None,
                   help="SDK workspace root (default: auto-detected)")
    p.add_argument("--remote",  default=None,
                   help="OpenOCD git remote URL (or set OPENOCD_REMOTE env var)")
    p.add_argument("--branch",  default=None,
                   help=f"OpenOCD branch to use (default: {OPENOCD_BRANCH})")
    p.add_argument("--dry-run", action="store_true")
    a = p.parse_args()
    main(a.sdk, a.remote, a.branch, a.dry_run)
