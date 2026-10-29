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
   * - MSPM0G, MSPM0L
     - M0
     - `LP-MSPM0G3507 <https://www.ti.com/tool/LP-MSPM0G3507>`__ , `LP-MSPM0G3519 <https://www.ti.com/tool/LP-MSPM0G3519>`__ , `LP-MSPM0L2228 <https://www.ti.com/tool/LP-MSPM0L2228>`__
     - Ubuntu 24.04

Features Added in This Release
===============================

.. list-table::
   :header-rows: 1
   :widths: 70 30
   :class: force-wrap

   * - Feature
     - Module
   * - TIMERA,  TIMERG, TIMERB, I2C, SPI, WWDT, IWDT, RTC, COMP, CRC
     - Peripheral Drivers
   * - DMA (Driver Only, No peripherals)
     - Peripheral Drivers
   * - Unicomm (UART, I2C, SPI), LCD, VREF, ADC
     - Peripheral Drivers
   * - GPIO, Pinctrl, Clock (Sysctrl), FLASHCTL
     - Peripheral Drivers
   * - CAN-FD and DAC for MSPM0G
     - Peripheral Drivers
   * - AES-ADV, TRNG
     - Security / Crypto
   * - System Power Management (RUN, SLEEP)
     - Power
   * - Runtime PM for UART, I2C, SPI
     - Power
   * - GUI composer Demo 
     - Applications
   * - GUI composer Telemetry 
     - Applications
   * - Power policy example 
     - Samples

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

   * - ZSOD-223
     - **drivers.spi.spi_loopback**

       * SPI loopback test failures because of timeout assert in twister tests.
     - SPI
