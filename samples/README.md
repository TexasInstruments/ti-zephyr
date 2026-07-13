# samples — TI OOB Samples

This directory mirrors `zephyr/samples/` and holds TI-specific out-of-box samples
for TI devices — demos, getting-started examples, and peripheral showcases.

## Directory structure (mirrors upstream)

```
samples/
├── basic/
│   └── blinky/          ← LED blink (all TI LaunchPads)
└── drivers/
    ├── uart/echo_bot/   ← UART echo
    └── adc/adc_read/    ← ADC single-channel read
```

## Adding a new sample

Follow the same layout as upstream `zephyr/samples/`:

```
samples/<category>/<name>/
├── CMakeLists.txt       ← find_package(Zephyr) + target_sources
├── prj.conf             ← Kconfig fragments
├── sample.yaml          ← twister descriptor (platform_allow, tags)
└── src/main.c
```

Use `platform_allow` in `sample.yaml` to restrict to TI boards.

## Upstreaming a sample

```bash
cp -r ti-zephyr/samples/<path>/ zephyr/samples/<path>/
rm -rf ti-zephyr/samples/<path>/
git -C zephyr add samples/<path>/
git -C zephyr commit -s -m "samples: <category>: add <name> for TI boards"
```
