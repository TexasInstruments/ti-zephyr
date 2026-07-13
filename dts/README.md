# dts — TI Downstream Device Tree Sources

This directory mirrors `zephyr/dts/` and holds DTSI files for TI SoCs not yet in
upstream Zephyr, across all TI device families.

## Structure (mirrors upstream)

```
dts/
└── arm/
    └── ti/
        ├── mspm33/
        │   ├── mspm33c321a.dtsi
        └── am62x/
            └── ...
```

DTS files follow the same conventions as `zephyr/dts/arm/ti/`.

## Notes

- MSPM0 DTS already in upstream at `zephyr/dts/arm/ti/mspm0/`
- MSPM0 pinctrl DTSI in HAL: `modules/hal/ti/dts/ti/mspm0/`
- New SoC DTSI for MSPM33 / AM62x variants go here until upstreamed

## Upstreaming

```bash
cp -r ti-zephyr/dts/<path>/ zephyr/dts/<path>/
git -C zephyr commit -s -m "dts: arm: ti: add <soc> DTSI"
```
