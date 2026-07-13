# drivers — TI Downstream Drivers

This directory mirrors `zephyr/drivers/` and holds TI peripheral drivers not yet in
upstream Zephyr, across all TI device families (MSPM0, MSPM33, AM6x, CC13xx, etc.).

## Structure (mirrors upstream)

When adding a driver, place it at the same path it would occupy upstream:

```
drivers/
├── gpio/
│   └── gpio_mspm33.c     ← example: MSPM33-specific GPIO driver
├── serial/
│   └── uart_mspm33.c
└── ...
```

Each subdirectory needs a `CMakeLists.txt` and `Kconfig` fragment when populated.

## Upstreaming a driver

```bash
cp -r ti-zephyr/drivers/<subsystem>/ zephyr/drivers/<subsystem>/
# integrate into zephyr/drivers/<subsystem>/CMakeLists.txt and Kconfig
git -C zephyr commit -s -m "drivers: <subsystem>: add TI <device> support"
```
