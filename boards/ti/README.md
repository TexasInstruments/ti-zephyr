# boards/ti — TI Downstream Boards

This directory mirrors `zephyr/boards/ti/` and holds TI board definitions that are
**not yet in upstream Zephyr**, or are under active TI development before upstreaming.

## What goes here

- New TI LaunchPad or EVM boards not yet submitted upstream
- Existing upstream boards with in-progress changes (downstream copy during development)
- Board variants that are TI-internal

## What does NOT go here

Boards already in upstream Zephyr do **not** need to be duplicated here — they are
automatically available after `west update` via the `west.yml` manifest import.

Current upstream TI boards (in `zephyr/boards/ti/`):
- MSPM0: `lp_mspm0g3507`, `lp_mspm0g3519`, `lp_mspm0l1306`, `lp_mspm0l2228`
- AM6x: `am62l_evm`, `sk_am62`, `sk_am64`
- AM243x: `am243x_evm`, `lp_am243`
- CC13xx/CC26xx: `cc1312r1_launchxl`, `cc1352*`, `cc26x2r1_launchxl`, `lp_em_cc2340r5`
- CC32xx: `cc3220sf_launchxl`, `cc3235sf_launchxl`
- MSP432: `msp_exp432p401r_launchxl`

## Board structure (mirrors upstream)

Each board directory follows the same layout as upstream `zephyr/boards/ti/<board>/`:

```
<board>/
├── board.yml           ← board metadata (name, vendor, SoC)
├── board.cmake         ← flash runner configuration (openocd, jlink)
├── Kconfig.<board>     ← board Kconfig (select SOC_*)
├── <board>_defconfig   ← default Kconfig symbols
├── <board>.dts         ← board-level device tree
├── <board>.yaml        ← twister board descriptor
├── support/
│   └── openocd.cfg     ← OpenOCD target configuration
└── doc/
    └── index.rst       ← board documentation
```

## Upstreaming a board

```bash
cp -r ti-zephyr/boards/ti/<board>/ zephyr/boards/ti/
rm -rf ti-zephyr/boards/ti/<board>/
git -C zephyr add boards/ti/<board>/
git -C zephyr commit -s -m "boards: ti: add <board>"
# push + open PR to zephyrproject-rtos/zephyr
```
