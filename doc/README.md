# doc — TI SDK Documentation

This directory mirrors `zephyr/doc/` and holds documentation for TI-specific content
in `ti-zephyr` that is not yet in upstream Zephyr.

Currently a placeholder. Documentation will be added here as boards, drivers, and
samples are developed.

## Structure (follows upstream conventions)

```
doc/
├── boards/
│   └── ti/
│       └── lp_mspm33c321a.rst   ← board docs (RST format)
├── drivers/
│   └── ...
└── samples/
    └── ...
```

When content is upstreamed, move the doc alongside the code:
```bash
cp -r ti-zephyr/doc/<path>/ zephyr/doc/<path>/
```
