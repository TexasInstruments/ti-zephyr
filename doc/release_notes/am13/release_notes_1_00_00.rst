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
   * - `AM13E2X <https://www.ti.com/product/AM13E23019>`__
     - M33
     - `LP-AM13E230 <https://www.ti.com/tool/LP-AM13E230>`__
     - Ubuntu 24.04

Features Added in This Release
===============================

.. list-table:: 
   :header-rows: 1
   :widths: 70 30
   :class: force-wrap

   * - Feature
     - Module
   * - TIMERG, WWDT, Unicomm (UART, I2C, SPI)
     - Peripheral Drivers
   * - GPIO, Pinctrl, Clock (Sysctrl), ECAP, EQEP
     - Peripheral Drivers
   * - MCAN, XBAR, AES-ADV, CRC
     - Peripheral Drivers
   * - Runtime PM for UART, I2C, SPI
     - Power
   * - LVGL Demo
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


