.. _ti-zephyr-release-1.00.00:

Release Notes 1.00.00
#####################

.. attention::
   Also refer to individual module pages for more details on each feature,
   unsupported features, important usage guidelines.

Device and Validation Information
==================================

.. list-table::
   :header-rows: 1
   :widths: 15 15 50 20
   :class: force-wrap

   * - SOC
     - Supported CPUs
     - EVM
     - Host PC
   * - `AM62L <https://www.ti.com/product/AM62L>`__
     - A53
     - `AM62L EVM <https://www.ti.com/tool/TMDS62LEVM>`__
     - Ubuntu 24.04

Features Added in This Release
===============================

.. list-table::
   :header-rows: 1
   :widths: 70 30
   :class: force-wrap

   * - Feature
     - Module
   * - SCMI-based PM / Clock Management, PSCI
     - Power
   * - GPIO, I2C, UART, McSPI, OSPI NOR, ADC, MMC-SD
     - Peripheral Drivers
   * - ECAP, EPWM, EQEP, WDT, RTC, DMTIMER
     - Peripheral Drivers
   * - SDIO, MCAN, DSS Nano, USB, VTM
     - Peripheral Drivers
   * - CPSW (PKTDMA)
     - Networking 
   * - LVGL
     - Middleware & Stacks 

Components Information
=========================================

.. list-table::
   :header-rows: 1
   :widths: 30 20 50
   :class: force-wrap

   * - Components
     - Version
     - Branch Info
   * - Zephyr
     - `4.4.0 <https://docs.zephyrproject.org/latest/releases/release-notes-4.4.html>`__
     - `v4.4.0-ti <https://github.com/TexasInstruments/zephyr/tree/v4.4.0-ti-01.00.00.02>`__
   * - ti-zephyr
     - 4.4.0+
     - `v4.4.0-ti <https://github.com/TexasInstruments/ti-zephyr/tree/v4.4.0-ti-01.00.00.02>`__
   * - GCC (aarch64-zephyr-elf)
     - 14.3.0+
     - `Zephyr SDK v1.0.1 <https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v1.0.1>`__
   * - Trusted Firmware A
     - 2.14.y
     - `trusted-firmware-a <https://github.com/TexasInstruments/arm-trusted-firmware/tree/ti-tfa-2.14.y>`__
   * - TIFS Firmware
     - `12.01.02 <https://software-dl.ti.com/tisci/esd/12_01_02/release_notes/release_notes.html>`__
     - NA
   * - LVGL
     - 9.5+
     - `lvgl <https://github.com/zephyrproject-rtos/lvgl/>`__

Fixed Issues
============

.. list-table::
   :header-rows: 1
   :widths: 12 68 20
   :class: force-wrap

   * - ID
     - Head Line
     - Module
   * - NA
     - NA
     - NA

Known Issues
============

.. list-table::
   :header-rows: 1
   :widths: 12 68 20
   :class: force-wrap

   * - ID
     - Head Line
     - Module
   * - ZSOD-221
     - Watchdog test fails because board resets on a watchdog violation.
     - Watchdog
