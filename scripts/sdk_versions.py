"""
Single source of truth for TI Zephyr SDK pinned versions.
Imported by download_toolchain.py, env_setup.py, check_deps.py.

SDK_VERSION is metadata for release tracking only — not used by install.py
for directory naming or branch selection (those use --branch arg).
"""

SDK_VERSION     = "v1.0.0"          # informational: SDK release label
SDK_NG_VERSION  = "1.0.1"           # Zephyr SDK NG (compiler toolchain)
OPENOCD_BRANCH  = "master"            # TI custom OpenOCD branch (ti-openocd)
OPENOCD_URL     = "https://github.com/TexasInstruments/ti-openocd"
MANIFEST_URL    = "https://github.com/TexasInstruments/ti-zephyr"
SDK_NG_URL_BASE = "https://github.com/TexasInstruments/sdk-ng/releases/download"

# ── TI downstream fork revisions ────────────────────────────────────────────
# Mirror the revision fields in west.yml.
# Update here when pinning to a specific branch/tag.
TI_ZEPHYR_REVISION  = "main"
TI_HAL_REVISION     = "main"   # PLACEHOLDER: update when TI HAL fork URL known
TI_MCUBOOT_REVISION = "main"
TI_TOOLS_REVISION   = "main"   # PLACEHOLDER: update when TI tools fork URL known

WEST_MIN_VERSION   = "1.5.0"
PYTHON_MIN_VERSION = (3, 10)

# Disk space estimates (bytes)
SDK_NG_SIZE_ESTIMATE = 3 * 1024 ** 3   # ~3 GB full SDK NG
WORKSPACE_SIZE_ESTIMATE = 2 * 1024 ** 3  # ~2 GB west workspace

# Platform → SDK NG archive filename stem
SDK_NG_PLATFORM_MAP = {
    "linux-x86_64":   f"zephyr-sdk-{SDK_NG_VERSION}_linux-x86_64",
    "linux-aarch64":  f"zephyr-sdk-{SDK_NG_VERSION}_linux-aarch64",
    "macos-x86_64":   f"zephyr-sdk-{SDK_NG_VERSION}_macos-x86_64",
    "macos-aarch64":  f"zephyr-sdk-{SDK_NG_VERSION}_macos-aarch64",
    "windows-x86_64": f"zephyr-sdk-{SDK_NG_VERSION}_windows-x86_64",
}

# Archive extension per OS
SDK_NG_EXT = {
    "linux":   ".tar.xz",
    "macos":   ".tar.xz",
    "windows": ".7z",
}
