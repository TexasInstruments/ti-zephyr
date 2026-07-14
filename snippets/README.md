# snippets — TI Downstream Snippets

This directory mirrors `zephyr/snippets/` and holds TI board/SoC-specific
build snippets not yet in upstream Zephyr.

## Structure (mirrors upstream)

```
snippets/
└── <snippet-name>/
    ├── snippet.yml
    └── <snippet-name>.conf
```

## Upstreaming a snippet

```bash
cp -r ti-zephyr/snippets/<snippet-name>/ zephyr/snippets/<snippet-name>/
git -C zephyr commit -s -m "snippets: <snippet-name>: add TI snippet"
```
