# TI Zephyr SDK (`ti-zephyr`)

TI's downstream Zephyr staging area for **all TI devices** — MSPM0, MSPM33, AM62x,
AM64x, AM243x, CC13xx/CC26xx/CC32xx, and any future TI SoC with Zephyr support.

This repository mirrors the upstream [Zephyr RTOS](https://github.com/zephyrproject-rtos/zephyr)
directory structure. It holds content not yet in upstream: new boards, in-progress
drivers, downstream DTS, TI-specific samples, and test infrastructure.

---

## Install (GitHub method)

**Prerequisites:** Python ≥ 3.10, git

Create a versioned SDK directory, clone `ti-zephyr` inside it, then run
`install.py`. The workspace is the SDK directory (parent of the clone).

```bash
# Create versioned SDK dir (name it after the version you want)
mkdir ~/ti/zephyr_ti_sdk_v1.0.0
cd ~/ti/zephyr_ti_sdk_v1.0.0

# Clone ti-zephyr inside it
git clone https://github.com/TexasInstruments/ti-zephyr ti-zephyr
cd ti-zephyr

# Set up everything (interactive prompts for toolchain + OpenOCD)
python3 install.py
```

`install.py` does:
1. Creates `.venv` with west + Python host tools
2. Runs `west init` + `west update` (fetches Zephyr, modules, bootloader, tools)
3. Installs `zephyr/scripts/requirements.txt`
4. *Prompts:* set up Zephyr SDK NG compiler toolchain?
5. *Prompts:* set up TI custom OpenOCD for `west flash`?
6. Generates `env_setup.sh` / `env_setup.ps1` / `env_setup.bat`

After install, the workspace is self-contained in the versioned SDK dir:
```
~/ti/zephyr_ti_sdk_v1.0.0/   ← versioned SDK workspace root
├── ti-zephyr/                ← the clone (ti-zephyr manifest)
├── .west/config              ← manifest.path = ti-zephyr
├── .venv/                    ← Python tools
├── toolchains/               ← compiler + OpenOCD (on-demand)
├── zephyr/                   ← Zephyr RTOS (via west)
├── modules/                  ← HAL/TI, mbedtls, etc. (via west)
├── bootloader/               ← MCUboot (via west)
└── env_setup.sh              ← activate SDK
```

> **Tip:** The SDK dir name is your version label. Clone into any directory —
> `install.py` always uses the parent of the clone as the workspace root.

### Set up tools separately (if skipped during install)

```bash
# Zephyr SDK NG compiler toolchain
python3 zephyr_ti_sdk_v1.0.0/scripts/setup_toolchain.py

# TI custom OpenOCD (for west flash)
python3 zephyr_ti_sdk_v1.0.0/scripts/setup_openocd.py --remote <openocd-remote-url>
```

---

## Activate environment

Every new shell session:

```bash
source ~/ti/env_setup.sh
```

Sets: `ZEPHYR_BASE`, `TI_ZEPHYR_BASE`, `ZEPHYR_SDK_INSTALL_DIR`, `OPENOCD_TCL`,
adds openocd to `PATH`, activates `.venv`, sources `zephyr-env.sh`.

---

## Build, flash, test

```bash
# Build
west build -b lp_mspm0g3519 zephyr_ti_sdk_v1.0.0/samples/basic/blinky

# Flash (requires TI OpenOCD — see setup_openocd.py)
west flash

# Run Twister tests (requires board connected)
west twister -p lp_mspm0g3519 \
  --hardware-map zephyr_ti_sdk_v1.0.0/tests/boards/lp_mspm0g3519/map.yml \
  --device-testing

# Per-board test runner
python3 zephyr_ti_sdk_v1.0.0/tests/run.py --board lp_mspm0g3519
```

---

## Repo structure

```
ti-zephyr/           ← mirrors zephyr/ directory structure
├── install.py       ← SDK workspace installer
├── west.yml         ← west manifest (imports upstream Zephyr + all modules)
├── zephyr/
│   └── module.yml   ← registers as Zephyr module (board_root, dts_root, etc.)
├── boards/ti/       ← TI boards not yet in upstream
├── drivers/         ← downstream TI drivers
├── dts/             ← downstream TI DTS/DTSI
├── samples/         ← TI OOB samples (mirrors zephyr/samples/ structure)
├── tests/           ← per-board test runners (Twister-based)
├── scripts/         ← SDK setup scripts
│   ├── env_setup.py        ← emits shell env vars
│   ├── setup_toolchain.py  ← install Zephyr SDK NG compiler
│   ├── setup_openocd.py    ← build TI custom OpenOCD
│   └── sdk_versions.py     ← pinned version constants
└── doc/             ← documentation staging
```

---

## Supported boards

### In this SDK (not yet in upstream Zephyr)

| Board | SoC | Status |
|-------|-----|--------|
| `lp_mspm0g3519` | MSPM0G3519 | Active dev |
| `lp_mspm33c321a` | MSPM33C321A | In development |

### Upstream (available after `west update`)

`lp_mspm0g3507`, `lp_mspm0l1306`, `lp_mspm0l2228`, `am62l_evm`, `am243x_evm`,
`lp_am243`, `sk_am62`, `sk_am64`, `cc1352*`, `cc26x2*`, `cc32*`, and more.

---

## Contributing

Branch model: `feature/*` → `next` → `main`. All PRs target `next`.

```bash
git clone URL --branch next <sdk-dir>
cd <sdk-dir>
git checkout -b feature/my-feature
python3 install.py
```

See [CONTRIBUTING.md](CONTRIBUTING.md) for full workflow, commit conventions,
fork placeholder replacement procedure, and upstreaming guide.

---

## Upstreaming workflow

When content is ready for upstream Zephyr (boards, drivers, DTS, samples):

```bash
# Move from ti-zephyr → zephyr
cp -r ti-zephyr/boards/ti/lp_mspm33c321a/ zephyr/boards/ti/
rm -rf ti-zephyr/boards/ti/lp_mspm33c321a/
git -C zephyr add boards/ti/lp_mspm33c321a/
git -C zephyr commit -s -m "boards: ti: add lp_mspm33c321a LaunchPad"
# push branch, open PR to zephyrproject-rtos/zephyr
```

Same pattern for `drivers/`, `dts/`, `samples/`, `lib/`.
