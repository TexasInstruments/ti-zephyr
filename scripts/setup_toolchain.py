#!/usr/bin/env python3
"""
Set up Zephyr SDK NG compiler toolchain for TI Zephyr SDK.

Checks for an existing install, creates a symlink if found at the standard
location, otherwise downloads from GitHub releases.

Usage:
    python3 scripts/setup_toolchain.py              # auto-detect SDK root
    python3 scripts/setup_toolchain.py --sdk ~/ti/  # explicit SDK root
    python3 scripts/setup_toolchain.py --version 1.0.1
"""

import argparse
import subprocess
import sys
from pathlib import Path

_HERE = Path(__file__).parent.resolve()  # scripts/

try:
    from scripts.sdk_versions import SDK_NG_VERSION
except ImportError:
    try:
        sys.path.insert(0, str(_HERE.parent))
        from scripts.sdk_versions import SDK_NG_VERSION
    except ImportError:
        SDK_NG_VERSION = "1.0.1"


def _detect_sdk_root(explicit=None):
    """Return SDK root: explicit arg, or derive from this script's location."""
    if explicit:
        return Path(explicit).expanduser().resolve()
    # scripts/ → ti-zephyr/ → sdk_root/
    return _HERE.parent.parent


def _find_existing_toolchain(version):
    """Look for zephyr-sdk-{version} in common locations. Return path or None."""
    candidates = [
        Path.home() / f"zephyr-sdk-{version}",
        Path("/opt") / f"zephyr-sdk-{version}",
        Path("/usr/local") / f"zephyr-sdk-{version}",
    ]
    for p in candidates:
        if p.is_dir():
            return p
    return None


def main(sdk_root=None, version=None, dry=False):
    sdk   = _detect_sdk_root(sdk_root)
    ver   = version or SDK_NG_VERSION
    dest  = sdk / "toolchains" / f"zephyr-sdk-{ver}"

    print(f"\n[setup_toolchain] SDK root: {sdk}")
    print(f"[setup_toolchain] Target:   {dest}\n")

    if dest.exists():
        print(f"  Already installed at {dest}")
        return

    # Check for symlink target (already a symlink pointing elsewhere)
    if dest.is_symlink():
        print(f"  Symlink already exists at {dest}")
        return

    dest.parent.mkdir(parents=True, exist_ok=True)

    # Try to symlink from an existing local install (fast, no download)
    existing = _find_existing_toolchain(ver)
    if existing:
        print(f"  Found existing toolchain at {existing}")
        print(f"  Creating symlink: {dest} → {existing}")
        if not dry:
            dest.symlink_to(existing)
        print("  Running setup.sh to register with CMake ...")
        _run_setup(existing, dry)
        print(f"\n  Toolchain ready: {dest}")
        return

    # Download if no local install found
    print(f"  No existing zephyr-sdk-{ver} found. Downloading ...")
    dl_script = _HERE / "download_toolchain.py"
    if not dl_script.exists():
        print(f"  ERROR: {dl_script} not found. Run 'west update' first.")
        sys.exit(1)

    if not dry:
        subprocess.run(
            [sys.executable, str(dl_script),
             "--version", ver,
             "--dest", str(dest.parent)],
            check=True,
        )
    else:
        print(f"  [dry] would run: python3 {dl_script} --version {ver} --dest {dest.parent}")

    print(f"\n  Toolchain ready: {dest}")


def _run_setup(tc_dir, dry=False):
    """Run setup.sh to register the toolchain with CMake."""
    setup_sh = tc_dir / "setup.sh"
    if setup_sh.exists():
        if not dry:
            subprocess.run(["bash", str(setup_sh), "-c"], cwd=tc_dir, check=True)
    else:
        # Newer SDK NG: run cmake export directly
        cmake_script = tc_dir / "cmake" / "zephyr_sdk_export.cmake"
        if cmake_script.exists() and not dry:
            subprocess.run(["cmake", "-P", str(cmake_script)], cwd=tc_dir, check=True)


if __name__ == "__main__":
    p = argparse.ArgumentParser(
        description="Set up Zephyr SDK NG toolchain",
        epilog=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("--sdk",     default=None,
                   help="SDK workspace root (default: auto-detected from script location)")
    p.add_argument("--version", default=None,
                   help=f"Zephyr SDK NG version (default: {SDK_NG_VERSION})")
    p.add_argument("--dry-run", action="store_true")
    a = p.parse_args()
    main(a.sdk, a.version, a.dry_run)
