# LP-MSPM33C3213 (Placeholder)

Placeholder directory for the MSPM33C3213 LaunchPad board definition.

No `board.yml` yet: the MSPM33C3213 SoC does not exist anywhere in the tree
(no upstream `soc/ti/mspm33/`, no downstream staging). A `board.yml`
declaring this SoC without backing content breaks `west`/`twister` board and
platform enumeration for the whole module (board_root scans every board.yml
unconditionally), so board files are deferred here until real SoC support
lands, following the same pattern as `boards/ti/am62x/`.

Add `board.yml`, DTS, Kconfig, defconfig, etc. once `soc/ti/mspm33/` exists
with real (not placeholder) content.
