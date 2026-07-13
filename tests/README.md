# tests — TI SDK Board Tests

Per-board Twister-based test runners for TI LaunchPad/EVM boards.

## Structure

```
tests/
├── run.py                          ← dispatcher: routes --board to per-board runner
└── boards/
    ├── lp_mspm0g3519/
    │   ├── run.py                  ← Twister runner for lp_mspm0g3519
    │   └── map.yml                 ← Twister hardware map (serial device, openocd path)
    ├── lp_mspm33c321a/             ← (to be added)
    └── ...
```

## Usage

```bash
# Run all tests for a board (hardware required)
python3 ti-zephyr/tests/run.py --board lp_mspm0g3519

# Build only (no hardware)
python3 ti-zephyr/tests/run.py --board lp_mspm0g3519 --build-only

# Run specific test directories
python3 ti-zephyr/tests/run.py --board lp_mspm0g3519 tests/drivers/gpio/gpio_api_1pin

# List available board runners
python3 ti-zephyr/tests/run.py --board x --list-boards

# Run per-board runner directly
python3 ti-zephyr/tests/boards/lp_mspm0g3519/run.py [tests ...]
```

## Adding a new board

```bash
mkdir ti-zephyr/tests/boards/<new_board>

# Copy and adapt the runner
cp ti-zephyr/tests/boards/lp_mspm0g3519/run.py \
   ti-zephyr/tests/boards/<new_board>/run.py
# Edit: update BRANCH, board string in run_tests(), openocd args

# Copy and adapt the hardware map
cp ti-zephyr/tests/boards/lp_mspm0g3519/map.yml \
   ti-zephyr/tests/boards/<new_board>/map.yml
# Edit: update platform, serial device path, openocd binary path
```

## map.yml

The `map.yml` is a Twister hardware map file. It contains machine-specific paths
(serial device, OpenOCD binary). Edit it to match your local setup before running.

Key fields:
- `platform`: board name (e.g., `lp_mspm0g3519`)
- `serial`: UART device (e.g., `/dev/ttyACM0`)
- `runner_params`: path to OpenOCD binary and TCL scripts
