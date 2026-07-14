#!/usr/bin/env python3
"""
TI Zephyr SDK pre-install dependency checker.
Usage: python3 check_deps.py [--quiet]
Exit 0 = all checks passed. Exit 1 = one or more failures.
"""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

try:
    from scripts.sdk_versions import PYTHON_MIN_VERSION, WEST_MIN_VERSION
except ImportError:
    # Allow running standalone from repo root
    sys.path.insert(0, str(Path(__file__).parents[1]))
    from scripts.sdk_versions import PYTHON_MIN_VERSION, WEST_MIN_VERSION

DISK_NEEDED_GB = 6   # toolchain ~3 GB + workspace ~2 GB + buffer


def _pass(msg, quiet):
    if not quiet:
        print(f"  [OK]  {msg}")


def _fail(msg, fix=""):
    print(f"  [FAIL] {msg}")
    if fix:
        print(f"         Fix: {fix}")


def check_python_version(quiet):
    v = sys.version_info[:2]
    if v >= PYTHON_MIN_VERSION:
        _pass(f"Python {v[0]}.{v[1]}", quiet)
        return True
    _fail(
        f"Python {v[0]}.{v[1]} < {PYTHON_MIN_VERSION[0]}.{PYTHON_MIN_VERSION[1]}",
        f"Install Python >= {PYTHON_MIN_VERSION[0]}.{PYTHON_MIN_VERSION[1]}",
    )
    return False


def check_git(quiet):
    if shutil.which("git"):
        _pass("git found", quiet)
        return True
    _fail("git not found", "Install git: https://git-scm.com/downloads")
    return False


def check_west_installable(quiet):
    try:
        import importlib.util
        if importlib.util.find_spec("west"):
            result = subprocess.run(
                [sys.executable, "-m", "west", "--version"],
                capture_output=True, text=True,
            )
            ver = result.stdout.strip()
            _pass(f"west found: {ver}", quiet)
            return True
    except Exception:
        pass
    # west not installed yet — check pip works
    result = subprocess.run(
        [sys.executable, "-m", "pip", "--version"],
        capture_output=True, text=True,
    )
    if result.returncode == 0:
        _pass("pip available (west will be installed)", quiet)
        return True
    _fail("Neither west nor pip found", "Install pip: https://pip.pypa.io/en/stable/installation/")
    return False


def check_cmake(quiet):
    if shutil.which("cmake"):
        result = subprocess.run(["cmake", "--version"], capture_output=True, text=True)
        ver = result.stdout.splitlines()[0] if result.stdout else "unknown"
        _pass(f"cmake: {ver}", quiet)
        return True
    _fail("cmake not found (optional — bundled in Zephyr SDK NG)",
          "Install cmake or let the SDK installer use the bundled version")
    return True   # non-fatal: SDK NG provides cmake in hosttools


def check_ninja(quiet):
    if shutil.which("ninja"):
        _pass("ninja found", quiet)
    else:
        _pass("ninja not found (optional — bundled in Zephyr SDK NG)", quiet)
    return True   # non-fatal


def check_disk_space(prefix, quiet):
    prefix = Path(prefix).expanduser()
    prefix.mkdir(parents=True, exist_ok=True)
    stat = shutil.disk_usage(prefix)
    free_gb = stat.free / 1024 ** 3
    if free_gb >= DISK_NEEDED_GB:
        _pass(f"Disk space: {free_gb:.1f} GB free at {prefix}", quiet)
        return True
    _fail(
        f"Only {free_gb:.1f} GB free at {prefix}, need ~{DISK_NEEDED_GB} GB",
        f"Free up space on the volume containing {prefix}",
    )
    return False


def check_network(quiet):
    try:
        import urllib.request
        urllib.request.urlopen("https://github.com", timeout=5)
        _pass("Network: github.com reachable", quiet)
        return True
    except Exception as exc:
        _fail(f"Network check failed: {exc}",
              "Check internet connection and proxy settings (https_proxy env var)")
        return False


def run_all(prefix="~/ti", quiet=False):
    print("TI Zephyr SDK — dependency check")
    print("-" * 40)
    results = [
        check_python_version(quiet),
        check_git(quiet),
        check_west_installable(quiet),
        check_cmake(quiet),
        check_ninja(quiet),
        check_disk_space(prefix, quiet),
        check_network(quiet),
    ]
    print("-" * 40)
    passed = sum(results)
    total  = len(results)
    if all(results):
        print(f"All {total} checks passed. Ready to install.")
        return 0
    failed = total - passed
    print(f"{passed}/{total} checks passed, {failed} failed. Fix issues above before installing.")
    return 1


def main():
    parser = argparse.ArgumentParser(description="Check TI Zephyr SDK install prerequisites.")
    parser.add_argument("--prefix", default="~/ti",
                        help="Target install prefix (default: ~/ti)")
    parser.add_argument("--quiet", action="store_true",
                        help="Only print failures")
    args = parser.parse_args()
    sys.exit(run_all(args.prefix, args.quiet))


if __name__ == "__main__":
    main()
