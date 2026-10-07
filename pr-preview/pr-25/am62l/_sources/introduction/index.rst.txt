.. _ti-zephyr-introduction:

Introduction
############

TI Zephyr is a west workspace module maintained by Texas Instruments that
provides TI-specific samples and tests on top of the upstream
`Zephyr RTOS <https://zephyrproject.org>`_.

What It Contains
****************

This repository does **not** fork or modify the Zephyr kernel. It contains:

- **Samples** — standalone applications demonstrating TI peripheral drivers
  and hardware features (``samples/``)
- **Tests** — automated test suites for TI-specific hardware (``tests/``)

All samples and tests are built against Zephyr |zephyr_ver|.

Supported Boards
****************

Current samples target the following TI LaunchPad development kits:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Board ID
     - Hardware
   * - ``lp_mspm0g3507``
     - MSPM0G3507 LaunchPad
   * - ``lp_mspm0g3519``
     - MSPM0G3519 LaunchPad
   * - ``lp_mspm0l2228``
     - MSPM0L2228 LaunchPad
   * - ``lp_mspm33c321``
     - MSPM33C321 LaunchPad
   * - ``lp_am13e230``
     - AM13E230 LaunchPad
   * - ``am62l_evm``
     - AM62L TMDS62LEVM evaluation module (EVM)

Relation to Main Zephyr
***********************

TI Zephyr is registered as a Zephyr module via ``zephyr/module.yml``. The
west manifest (``west.yml``) pins the TI fork of Zephyr at |zephyr_ver| and
imports the standard set of upstream Zephyr modules (mbedtls, cmsis, mcuboot,
etc.).

For core Zephyr documentation — kernel, device drivers, build system, and
boards — refer to the
`main Zephyr documentation <https://docs.zephyrproject.org>`_.
