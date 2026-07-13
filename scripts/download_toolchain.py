#!/usr/bin/env python3
"""
Download and extract the Zephyr SDK NG toolchain for the current platform.

Usage:
    python3 download_toolchain.py --dest ~/ti/zephyr_sdk_v1.0.0/toolchains/
    python3 download_toolchain.py --version 1.0.1 --dest <dir> [--skip-verify]
"""

import argparse
import hashlib
import json
import platform
import shutil
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

try:
    from scripts.sdk_versions import SDK_NG_VERSION, SDK_NG_URL_BASE, SDK_NG_PLATFORM_MAP, SDK_NG_EXT
except ImportError:
    sys.path.insert(0, str(Path(__file__).parents[1]))
    from scripts.sdk_versions import SDK_NG_VERSION, SDK_NG_URL_BASE, SDK_NG_PLATFORM_MAP, SDK_NG_EXT


def _detect_platform():
    """Return platform key: linux-x86_64, linux-aarch64, macos-x86_64, macos-aarch64, windows-x86_64."""
    os_name = sys.platform
    machine = platform.machine().lower()

    if os_name.startswith("linux"):
        os_key = "linux"
    elif os_name == "darwin":
        os_key = "macos"
    elif os_name == "win32":
        os_key = "windows"
    else:
        raise RuntimeError(f"Unsupported platform: {os_name}")

    if machine in ("x86_64", "amd64"):
        arch = "x86_64"
    elif machine in ("aarch64", "arm64"):
        arch = "aarch64"
    else:
        raise RuntimeError(f"Unsupported architecture: {machine}")

    if os_key == "windows" and arch == "aarch64":
        raise RuntimeError("Windows ARM64 not yet supported by Zephyr SDK NG")

    return f"{os_key}-{arch}"


def _build_url(version, platform_key):
    stem = f"zephyr-sdk-{version}_{platform_key}"
    os_key = platform_key.split("-")[0]
    ext = SDK_NG_EXT[os_key]
    tag = f"v{version}"
    return f"{SDK_NG_URL_BASE}/{tag}/{stem}{ext}", stem, ext


def _fetch_sha256(version, platform_key):
    """Fetch SHA256 from GitHub release checksums file."""
    tag = f"v{version}"
    url = f"{SDK_NG_URL_BASE}/{tag}/sha256.sum"
    try:
        with urllib.request.urlopen(url, timeout=15) as r:
            content = r.read().decode()
        stem = f"zephyr-sdk-{version}_{platform_key}"
        for line in content.splitlines():
            if stem in line:
                return line.split()[0]
    except Exception as exc:
        print(f"  [WARN] Could not fetch SHA256: {exc}")
    return None


def _progress_hook(block_count, block_size, total_size):
    downloaded = block_count * block_size
    if total_size > 0:
        pct = min(100, downloaded * 100 // total_size)
        mb = downloaded / 1024 ** 2
        total_mb = total_size / 1024 ** 2
        bar = "#" * (pct // 5) + "." * (20 - pct // 5)
        print(f"\r  [{bar}] {pct:3d}% {mb:.0f}/{total_mb:.0f} MB", end="", flush=True)
    else:
        mb = downloaded / 1024 ** 2
        print(f"\r  Downloaded {mb:.0f} MB", end="", flush=True)


def _verify_sha256(path, expected):
    print(f"\n  Verifying SHA256 ...", end=" ")
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    got = h.hexdigest()
    if got == expected:
        print("OK")
        return True
    print(f"MISMATCH\n  Expected: {expected}\n  Got:      {got}")
    return False


def _extract_tar(archive, dest):
    print(f"  Extracting {archive.name} ...")
    with tarfile.open(archive, "r:xz") as tf:
        tf.extractall(dest)


def _extract_7z(archive, dest):
    # Try 7z / 7za / py7zr
    for cmd in ("7z", "7za", "7zz"):
        if shutil.which(cmd):
            subprocess.run([cmd, "x", str(archive), f"-o{dest}", "-y"], check=True)
            return
    try:
        import py7zr
        with py7zr.SevenZipFile(archive, mode="r") as z:
            z.extractall(path=dest)
        return
    except ImportError:
        pass
    raise RuntimeError(
        "Cannot extract .7z: install 7-Zip (https://www.7-zip.org/) or py7zr: pip install py7zr"
    )


def _run_setup(sdk_ng_dir):
    """Register SDK with CMake by running setup.sh / setup.cmd."""
    if sys.platform == "win32":
        setup = sdk_ng_dir / "setup.cmd"
        if setup.exists():
            subprocess.run(["cmd", "/c", str(setup)], check=True, cwd=sdk_ng_dir)
    else:
        setup = sdk_ng_dir / "setup.sh"
        if setup.exists():
            subprocess.run(["bash", str(setup), "-c"], check=True, cwd=sdk_ng_dir)
        else:
            # Fallback: run cmake export directly
            cmake_script = sdk_ng_dir / "cmake" / "zephyr_sdk_export.cmake"
            if cmake_script.exists():
                subprocess.run(
                    ["cmake", "-P", str(cmake_script)], check=True, cwd=sdk_ng_dir
                )


def download(version=SDK_NG_VERSION, dest=None, skip_verify=False, dry_run=False):
    platform_key = _detect_platform()
    url, stem, ext = _build_url(version, platform_key)
    dest = Path(dest or Path.home() / "ti" / "toolchains").expanduser()
    dest.mkdir(parents=True, exist_ok=True)

    sdk_ng_dir = dest / f"zephyr-sdk-{version}"
    if sdk_ng_dir.exists():
        print(f"  Already exists: {sdk_ng_dir}")
        return sdk_ng_dir

    archive = dest / f"{stem}{ext}"

    print(f"  Platform : {platform_key}")
    print(f"  Version  : {version}")
    print(f"  URL      : {url}")
    print(f"  Dest     : {dest}")

    if dry_run:
        print("  [dry-run] Would download and extract.")
        return sdk_ng_dir

    expected_sha = None if skip_verify else _fetch_sha256(version, platform_key)

    print(f"  Downloading ...")
    urllib.request.urlretrieve(url, archive, reporthook=_progress_hook)
    print()

    if expected_sha and not _verify_sha256(archive, expected_sha):
        archive.unlink(missing_ok=True)
        raise RuntimeError("SHA256 verification failed. Archive deleted.")

    if ext == ".tar.xz":
        _extract_tar(archive, dest)
    else:
        _extract_7z(archive, dest)

    archive.unlink(missing_ok=True)   # remove archive after extraction

    print(f"  Running setup to register with CMake ...")
    _run_setup(sdk_ng_dir)

    print(f"  Toolchain installed: {sdk_ng_dir}")
    return sdk_ng_dir


def main():
    parser = argparse.ArgumentParser(description="Download Zephyr SDK NG toolchain.")
    parser.add_argument("--version", default=SDK_NG_VERSION,
                        help=f"SDK NG version (default: {SDK_NG_VERSION})")
    parser.add_argument("--dest", default=None,
                        help="Destination directory (default: ~/ti/toolchains/)")
    parser.add_argument("--skip-verify", action="store_true",
                        help="Skip SHA256 verification")
    parser.add_argument("--dry-run", action="store_true",
                        help="Print what would be done without downloading")
    args = parser.parse_args()
    download(args.version, args.dest, args.skip_verify, args.dry_run)


if __name__ == "__main__":
    main()
