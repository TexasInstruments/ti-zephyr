<!--
Copyright (c) 2026 Texas Instruments Incorporated
SPDX-License-Identifier: Apache-2.0
-->

# TI Zephyr SDK — Structure Overview

## Current Structure (v1.0.0)

```
ti-zephyr/                          ← west manifest repo (git-tracked only)
│
├── install.py                      ← GitHub install entry point
├── west.yml                        ← west manifest (TI fork placeholders)
├── sdk_versions.yml                ← pinned version manifest
├── CONTRIBUTING.md                 ← branch model, PR rules, fork procedure
├── README.md                       ← quick-start
│
├── zephyr/
│   └── module.yml                  ← registers as Zephyr module
│                                      (board_root, dts_root, module_ext_root)
│
├── scripts/                        ← all Python, standalone + called from install.py
│   ├── sdk_versions.py             ← single source of truth for version pins
│   ├── env_setup.py                ← env var emitter (bash/sh)
│   ├── env_setup.sh                ← thin shell wrapper
│   ├── setup_toolchain.py          ← Zephyr SDK NG install (symlink or download)
│   ├── setup_openocd.py            ← TI custom OpenOCD clone + build
│   ├── download_toolchain.py       ← SDK NG downloader
│   └── check_deps.py               ← pre-install dependency checker
│
├── boards/ti/                      ← downstream TI boards (not yet upstream)
│   ├── lp_mspm0g3519/              ← MSPM0G3519 (active dev, downstream copy)
│   └── lp_mspm33c321a/             ← MSPM33C321A (in development)
│
├── samples/                        ← mirrors zephyr/samples/ structure
│   ├── basic/blinky/               ← LED blink (GPIO)
│   └── drivers/
│       ├── uart/echo_bot/          ← UART interrupt echo
│       └── adc/adc_read/           ← ADC single-channel read
│
├── tests/                          ← per-board Twister test runners
│   ├── run.py                      ← dispatcher (--board lp_mspm0g3519)
│   └── boards/lp_mspm0g3519/
│       ├── run.py                  ← Twister runner + OpenOCD setup
│       └── map.yml                 ← hardware map (serial, openocd paths)
│
├── doc/                            ← Sphinx/RST documentation
│   ├── conf.py                     ← Sphinx config
│   ├── index.rst                   ← doc root
│   ├── getting_started/index.rst
│   ├── boards/
│   │   ├── index.rst
│   │   ├── lp_mspm0g3519.rst
│   │   └── lp_mspm33c321a.rst
│   ├── samples/index.rst
│   ├── contribute/index.rst
│   ├── releases/release-notes-v1.0.0.rst
│   └── SDK_OVERVIEW.md             ← this file
│
├── drivers/                        ← downstream TI drivers (empty, staged)
├── dts/                            ← downstream TI DTS (empty, staged)
├── include/                        ← downstream headers (empty, staged)
└── lib/                            ← downstream libraries (empty, staged)
```

### West workspace layout (after `python3 install.py`)

```
~/ti/zephyr_ti_sdk_v1.0.0/      ← versioned SDK workspace root
├── ti-zephyr/                   ← the clone (west manifest, this repo)
├── .west/config                 ← manifest.path = ti-zephyr
├── .venv/                       ← Python 3.12 venv
├── toolchains/
│   ├── zephyr-sdk-1.0.1/       ← Zephyr SDK NG (symlink or downloaded)
│   └── openocd/                 ← TI custom OpenOCD (ti-openocd)
├── zephyr/                      ← Zephyr RTOS (TexasInstruments/zephyr)
├── modules/                     ← hal/ti, mbedtls, cmsis, etc.
├── bootloader/                  ← MCUboot
├── tools/                       ← west tools
└── env_setup.sh                 ← activate SDK
```

**Tip:** Clone `ti-zephyr` into any directory — `install.py` uses the clone's
parent as the workspace root. The SDK dir name is your version label.

---

## Branch Strategy

| Branch | Purpose |
|--------|---------|
| `main` | Stable, tagged releases. Clone target for users. |
| `next` | Integration. All PRs target here. |
| `feature/*` | Dev work. Branch from `next`. |

---

## Supported Boards (v1.0.0)

| Board | SoC | Family | Source | Status |
|-------|-----|--------|--------|--------|
| `lp_mspm0g3519` | MSPM0G3519 | MSPM0 | SDK + upstream | Active dev |
| `lp_mspm0g3507` | MSPM0G3507 | MSPM0 | Upstream | Production |
| `lp_mspm0l1306` | MSPM0L1306 | MSPM0 | Upstream | Production |
| `lp_mspm0l2228` | MSPM0L2228 | MSPM0 | Upstream | Production |
| `lp_mspm33c321a` | MSPM33C321A | MSPM33 | SDK | In development |
| `am62l_evm` | AM62L | AM6x | Upstream | Production |
| `sk_am62` | AM625 | AM6x | Upstream | Production |
| `am243x_evm` | AM2434 | AM2x | Upstream | Production |
| `cc1352*`, `cc26x2*` | CC13xx/CC26xx | Wireless | Upstream | Production |

---

## Fork Status (west.yml remotes)

| Remote | For | URL |
|--------|-----|-----|
| `ti-zephyr` | Zephyr RTOS | https://github.com/TexasInstruments/zephyr |
| `ti-mcuboot` | bootloader/mcuboot | https://github.com/TexasInstruments/mcuboot |
| `ti-hal` | modules/hal/ti | PLACEHOLDER_TI_HAL_FORK |
| `ti-tools` | tools | PLACEHOLDER_TI_TOOLS_FORK |
| `upstream` | all other modules | https://github.com/zephyrproject-rtos |

To replace a placeholder: update `url-base` in `west.yml` + `TI_*_REVISION` in
`scripts/sdk_versions.py`, then uncomment the project entry in `west.yml`.

---

## How to Upstream Content

When a board/driver/sample is ready for the Zephyr community:

```bash
# Example: upstream lp_mspm33c321a
cp -r ti-zephyr/boards/ti/lp_mspm33c321a/ zephyr/boards/ti/
rm -rf ti-zephyr/boards/ti/lp_mspm33c321a/
git -C zephyr add boards/ti/lp_mspm33c321a/
git -C zephyr commit -s -m "boards: ti: add lp_mspm33c321a LaunchPad"
# Push to TI zephyr fork → open PR to zephyrproject-rtos/zephyr
```

Same pattern for `drivers/`, `dts/`, `samples/`, `lib/`.

---

## Key Files Reference

| File | Purpose |
|------|---------|
| `west.yml` | west manifest: all project sources + TI fork placeholders |
| `install.py` | GitHub install: workspace + venv + optional toolchain/openocd |
| `scripts/sdk_versions.py` | All pinned version constants |
| `scripts/env_setup.py` | Shell env emitter (bash) |
| `scripts/setup_toolchain.py` | Zephyr SDK NG install |
| `scripts/setup_openocd.py` | TI OpenOCD build |
| `zephyr/module.yml` | Zephyr module registration |
| `tests/run.py` | Multi-board test dispatcher |
| `CONTRIBUTING.md` | Branch model, PR workflow, fork replacement |
