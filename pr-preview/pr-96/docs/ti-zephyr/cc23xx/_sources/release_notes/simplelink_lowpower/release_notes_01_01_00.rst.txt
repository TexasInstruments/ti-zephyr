.. _ti-zephyr-cc23-release-01.01.00:

SimpleLink Zephyr v4.4.0-ti-01.01.00 Release
#################################################

Texas Instruments Zephyr GitHub repository is the downstream fork of
`Zephyr <https://github.com/zephyrproject-rtos/zephyr>`__ and is the starting
point for developers wanting to start developing Zephyr applications.

This SimpleLink™ downstream Zephyr release is production grade and the team is
also working on upstreaming this content to mainline Zephyr.

To quickly get started, see the
`Zephyr Trainings on SimpleLink Academy <https://dev.ti.com/tirex/explore/node?node=A__AEIJm0rwIeU.2P1OBWwlaA__SIMPLELINK-ACADEMY-CC23XX__gsUPh5j__LATEST>`__.

Supported Devices
=================

.. list-table::
   :header-rows: 1
   :widths: 35 40 25
   :class: force-wrap

   * - Device
     - Board
     - Quality Level
   * - CC2340R5
     - LP_EM_CC2340R5
     - GA
   * - CC2340R53
     - LP_EM_CC2340R53
     - GA
   * - CC2340R5MODA
     - LP_EM_CC2340R5MODA
     - GA
   * - CC2340R53YBG
     - LP_EM_CC2340R53_WCSP
     - GA
   * - CC2745R10_Q1
     - LP_EM_CC2745R10_Q1
     - GA
   * - CC2755R10
     - LP_EM_CC2745R10_Q1
     - GA
   * - CC2755P10
     - LP_EM_CC2755P10
     - GA
   * - CC2755P20
     - LP_EM_CC2755P20
     - GA
   * - CC2755R10_BG
     - LP_EM_CC2755R10_BG
     - GA
   * - CC2755R7_BG
     - LP_EM_CC2755R7_BG
     - GA

What's New
==========

**hal_ti**

- Added support for LP_EM_CC2755R10_BG
- Added support for LP_EM_CC2755R7_BG

Known Issues and Limitations
=============================

**Zephyr**

Below sections are common across CC23xx/CC27xx.

.. list-table::
   :header-rows: 1
   :widths: 15 85
   :class: force-wrap

   * - Key
     - Summary
   * - ZEPHYR-806
     - test_clockp_stop SLATE tests fails on lp_em_cc2755p10
   * - ZEPHYR-805
     - Several samples and test fail with "unknown error"
   * - ZEPHYR-804
     - settings.nvs sample fails with error -22
   * - ZEPHYR-803
     - Comparator SLATE tests fail
   * - ZEPHYR-802
     - ADC SLATE tests fail on lp_em_cc2340r5
   * - ZEPHYR-801
     - counter_basic_api tests fail on cc27 boards
   * - ZEPHYR-800
     - gpio_api_1pin tests fail
   * - ZEPHYR-797
     - No-optimization tests fail with stack overflow on lp_em_cc2755p20
   * - ZEPHYR-792
     - Debug test fail for cc2755r10_bg and cc2755r7_bg
   * - ZEPHYR-791
     - UART async API tests fail for cc2755r10_bg and cc2755r7_bg
   * - ZEPHYR-790
     - Counter basic API tests fail for cc2755r10_bg and cc2755r7_bg
   * - ZEPHYR-789
     - I2S API tests fail for lp_em_cc2755r10_bg and lp_em_cc2755r7_bg
   * - ZEPHYR-788
     - Stack overflows for lp_em_cc2755r10_bg and lp_em_cc2755r7_bg
   * - ZEPHYR-787
     - CMSIS-DSP transform tests fail for lp_em_cc2755r10_bg
   * - ZEPHYR-662
     - Validate transfer size in DMA reload
   * - ZEPHYR-660
     - DMA is not re-enabled after exiting standby
   * - ZEPHYR-625
     - Low frequency clock support is limited to external crystal (LFXT) sources only
   * - ZEPHYR-599
     - CC27xx reference clock out-of-range event causes system hang
   * - ZEPHYR-598
     - [CC23xx/CC27xx] flash driver writes below datasheet min voltage (e.g., 1.8V) corrupt flash data
   * - ZEPHYR-597
     - [CC23xx/CC27xx] zephyr flash driver allows unbounded writes to same row (83-write HW limit not tracked)
   * - ZEPHYR-511
     - Change board-based include path to soc-based include path for the generated sysconfig files
   * - ZEPHYR-423
     - [CC23xx/CC27xx] I2C driver conflict between speed and 10-bit addressing flags
   * - ZEPHYR-333
     - ADC FIFO becomes out of sync after FIFO underflow
   * - ZEPHYR-263
     - uart_lpf3 async API missing support for inactivity period timeout
   * - ZEPHYR-250
     - CC23XX/CC27XX SOC prevents idle power mode during peripheral operations
   * - ZEPHYR-112
     - [CC23xx] Power Management Conflict between Zephyr App and MCUBoot

**Radio**

.. list-table::
   :header-rows: 1
   :widths: 15 85
   :class: force-wrap

   * - Key
     - Summary
   * - RCL-1352
     - RCL_BLE5_getStartTimeDelay APIs Can Return Garbage SYNTHCALTIMEOUT and MDM.SPARE0 Value

**BLE**

.. list-table::
   :header-rows: 1
   :widths: 15 85
   :class: force-wrap

   * - Key
     - Summary
   * - ZEPHYR-799
     - [Samples] Sometimes Bluetooth fail to initialize ( err -5 )
   * - ZEPHYR-733
     - [DFU][Off-Chip] DFU process is unstable in different times on CC2755xx platform
   * - ZEPHYR-642
     - Bluetooth: Some samples may fail to build on lp_em_cc2340r5 due to RAM overflow
   * - ZEPHYR-634
     - BLE: bt_ctlr_hci_driver fails to send HCI commands during init and bonding (err -2)

Test Coverage
=============

Twister on-device testing (limited) was enabled for:

- ``lp_em_cc2340r53``
- ``lp_em_cc2755p10``
- ``lp_em_cc2755p20``
- ``lp_em_cc2340r5``
- ``lp_em_cc2745r10_q1/cc2745r10_q1``
- ``lp_em_cc2340r5moda``
- ``lp_em_cc2340r53_wcsp``
- ``lp_em_cc2755r10_bg``
- ``lp_em_cc2755r7_bg``

Test Report
===========

`Zephyr Test Report <https://software-dl.ti.com/simplelink/esd/simplelink_lowpower_zephyr_sdk/4.4.0-ti-01.01.00/exports/Zephyr_test_report.html>`__

Versioning
==========

See the `README <https://github.com/TexasInstruments/simplelink-zephyr?tab=readme-ov-file#versioning>`__
in this repository.

**Operating System Support**

- Windows® 11
- Ubuntu 24.04.4 LTS (64-bit)
- macOS 13.x

Dependencies
============

Zephyr v4.4.0 STS

.. note::
   Flashing a Zephyr 4.4 ``.elf``/``.out`` file containing a TLS header
   segment will fail when using UniFlash or CCS. For CCS (version 21.0) and
   UniFlash (version 9.6.0) or earlier, use a ``.hex`` file instead.

Technical Support and Product Updates
======================================

- `TI SimpleLink Solutions <https://www.ti.com/wireless-connectivity/overview.html>`__
- `TI E2E Community <https://e2e.ti.com/>`__
- `Zephyr Trainings on SimpleLink Academy <https://dev.ti.com/tirex/explore/node?node=A__AEIJm0rwIeU.2P1OBWwlaA__SIMPLELINK-ACADEMY-CC23XX__gsUPh5j__LATEST>`__
- `TI Zephyr RTOS overview <https://www.ti.com/design-development/software-design/Zephyr-RTOS.html>`__
