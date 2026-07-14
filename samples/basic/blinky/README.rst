.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. zephyr:code-sample:: ti-blinky
   :name: TI Blinky
   :relevant-api: gpio_interface

   Blink an LED at 1 Hz on TI LaunchPad boards using the GPIO API.

Overview
********

TI-specific blinky sample targeting TI LaunchPad boards. Uses the
:ref:`GPIO API <gpio_api>` to toggle the ``led0`` alias defined in each
board's devicetree.

This sample demonstrates:

* Obtaining a GPIO pin specification from devicetree as :c:struct:`gpio_dt_spec`
* Configuring a GPIO pin as output
* Toggling the LED in an infinite loop at 1 Hz

Requirements
************

A TI LaunchPad board with an LED connected to the ``led0`` devicetree alias.

All supported boards have this configured:

.. list-table::
   :header-rows: 1
   :widths: 30 30 40

   * - Board
     - LED Pin
     - Color
   * - ``lp_mspm0g3519``
     - PB22
     - Blue
   * - ``lp_mspm0g3507``
     - PB22
     - Blue
   * - ``lp_mspm0l2228``
     - PA0
     - Green

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/basic/blinky
   :board: lp_mspm0g3519
   :goals: build flash
   :compact:

After flashing, the Blue LED blinks at 1 Hz. No console output by default.

Sample Output
=============

This sample produces no serial output. Observable behavior: LED toggles
every 1000 ms on the LaunchPad.

References
**********

- :dtcompatible:`gpio-leds`
- :ref:`gpio_api`
- `LP-MSPM0G3519 product page <https://www.ti.com/tool/LP-MSPM0G3519>`_
