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
   * - `MSPM33C <https://www.ti.com/product/MSPM33C321A>`__
     - M33
     - `LP-MSPM33C321A <https://www.ti.com/tool/LP-MSPM33C321A>`__
     - Ubuntu 24.04

Features Added in This Release
===============================

.. list-table::
   :header-rows: 1
   :widths: 70 30
   :class: force-wrap

   * - Feature
     - Module
   * - Zephyr SDK Baseport
     - Core SDK
   * - MSPM33C SoC support
     - SDK
   * - GPIO, TIMERA, TIMERG, WWDT, IWDT, Clock (Sysctrl)
     - Peripheral Drivers
   * - Unicomm (I2C, UART, SPI), 
     - Peripheral Drivers
   * - RTC, VREF, CRC, COMP, CAN-FD
     - Peripheral Drivers
   * - LVGL
     - Middleware & Stacks
   * - LittleFS
     - Middleware & Stacks
   * - LVGL GUI based demo
     - Applications

Components Information
=========================================

.. list-table::
   :header-rows: 1
   :widths: 30 20 50
   :class: force-wrap

   * - Components
     - Version
     - Branch Info
   * - Zephyr Kernel
     - `4.4.0 <https://docs.zephyrproject.org/latest/releases/release-notes-4.4.html>`__
     - `v4.4.0-ti <https://github.com/TexasInstruments/zephyr/tree/v4.4.0-ti>`__

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
   * - ZSOD-224
     - **drivers.gpio.get_direction fails**

       * ``test_disconnect`` asserts the pin is not an input after ``GPIO_DISCONNECTED``, but ``gpio_pin_is_input()`` returns 1.
       * Cause: the driver derives direction only from the output-enable bit, so a disconnected pin reads as an input.
     - GPIO
   * - ZSOD-225
     - **drivers.watchdog (wdt_basic_api) fails on lp_mspm33c321a**

       * The board resets and the test restarts at ``test_wdt_no_callback`` on every boot instead of advancing to the next stage.
       * Cause: test state held in noinit RAM is not preserved across the WWDT reset.
     - WWDT
   * - ZSOD-226
     - **tests.drivers.dma.chan_blen_transfer and tests.drivers.dma.loop_transfer fails**

       * These tests fail because of improper platform_allow in tests.yaml
       * Fix: `fc84130 <https://github.com/TexasInstruments/zephyr/commit/fc84130f3d6e7f21e2ae17a1adf9c6ee02337c8e>`__
     - DMA
