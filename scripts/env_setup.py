#!/usr/bin/env python3
"""
Emit shell environment setup commands for TI Zephyr SDK.

Linux/macOS (bash/zsh):
    eval "$(python3 env_setup.py)"
    # or: source env_setup.sh

Windows PowerShell:
    python3 env_setup.py --shell powershell | Invoke-Expression
    # or: . env_setup.ps1

Windows CMD:
    env_setup.bat
"""

import argparse
import sys
from pathlib import Path

# Script lives at <clone>/scripts/env_setup.py
# parents[0] = scripts/
# parents[1] = <clone>/  (e.g. ti-zephyr/ or zephyr_ti_sdk_v1.0.0/)
# parents[2] = SDK root   (e.g. ~/ti/ or ~/ti/zephyr_ti_sdk_v1.0.0/)
_SCRIPTS_DIR = Path(__file__).parent.resolve()
_TI_ZEPHYR   = _SCRIPTS_DIR.parent
SDK_ROOT      = _TI_ZEPHYR.parent

IS_WIN   = sys.platform == "win32"
VENV_BIN = SDK_ROOT / ".venv" / ("Scripts" if IS_WIN else "bin")


def _find_toolchain():
    """Return path to zephyr-sdk-* inside SDK toolchains/, or None."""
    tc_base = SDK_ROOT / "toolchains"
    if not tc_base.is_dir():
        return None
    for p in sorted(tc_base.glob("zephyr-sdk-*")):
        if p.is_dir() or p.is_symlink():
            return p
    return None


def _find_openocd():
    """Return (binary, tcl_dir) if TI openocd is built, else (None, None)."""
    oc_dir = SDK_ROOT / "toolchains" / "openocd"
    binary = oc_dir / "src" / "openocd"
    tcl    = oc_dir / "tcl"
    if binary.exists():
        return binary, tcl
    return None, None


def _build_vars():
    tc_dir     = _find_toolchain()
    oc_bin, oc_tcl = _find_openocd()

    vars_ = {
        "ZEPHYR_BASE":            str(SDK_ROOT / "zephyr"),
        "TI_ZEPHYR_BASE":         str(_TI_ZEPHYR),
    }
    if tc_dir:
        vars_["ZEPHYR_SDK_INSTALL_DIR"] = str(tc_dir)
    if oc_tcl:
        vars_["OPENOCD_TCL"] = str(oc_tcl)

    return vars_, oc_bin


def _emit_bash():
    vars_, oc_bin = _build_vars()
    for k, v in vars_.items():
        print(f'export {k}="{v}"')
    # Add openocd binary dir to PATH if built
    if oc_bin:
        print(f'export PATH="{oc_bin.parent}:$PATH"')
    print(f'source "{VENV_BIN / "activate"}"')
    zephyr_env = SDK_ROOT / "zephyr" / "zephyr-env.sh"
    if zephyr_env.exists():
        print(f'source "{zephyr_env}"')


def _emit_powershell():
    vars_, oc_bin = _build_vars()
    for k, v in vars_.items():
        print(f'$env:{k} = "{v}"')
    if oc_bin:
        print(f'$env:PATH = "{oc_bin.parent};" + $env:PATH')
    print(f'. "{VENV_BIN / "Activate.ps1"}"')


def _emit_cmd():
    vars_, oc_bin = _build_vars()
    for k, v in vars_.items():
        print(f"SET {k}={v}")
    if oc_bin:
        print(f'SET PATH={oc_bin.parent};%PATH%')
    print(f'CALL "{VENV_BIN / "activate.bat"}"')


def main():
    parser = argparse.ArgumentParser(
        description="Print TI Zephyr SDK environment setup commands.",
    )
    parser.add_argument(
        "--shell",
        choices=["bash", "powershell", "cmd"],
        default="bash",
        help="Target shell syntax (default: bash)",
    )
    args = parser.parse_args()

    if args.shell == "powershell":
        _emit_powershell()
    elif args.shell == "cmd":
        _emit_cmd()
    else:
        _emit_bash()


if __name__ == "__main__":
    main()
