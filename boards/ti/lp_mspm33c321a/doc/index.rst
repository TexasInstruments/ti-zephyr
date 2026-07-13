.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _lp_mspm33c321a:

LP-MSPM33C321A
##############

Overview
********

The LP-MSPM33C321A LaunchPad is a low-cost development board for the Texas
Instruments MSPM33C321A microcontroller from the MSPM33 Arm® Cortex®-M33
MCU family.

.. note::

   This board definition is in active development. SoC DTS and driver support
   are being developed in this SDK pending upstream Zephyr acceptance.

Hardware
********

* MSPM33C321A MCU (Arm® Cortex®-M33)
* Onboard XDS110 debug probe (USB-C)
* User LEDs and push buttons
* 40-pin BoosterPack compatible headers

Supported Features
******************

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Feature
     - Status
   * - GPIO
     - Planned
   * - UART
     - Planned
   * - SPI
     - Planned
   * - I2C
     - Planned
   * - ADC
     - Planned
   * - DMA
     - Planned
   * - Timers
     - Planned

Building and Flashing
*********************

.. note::

   This board cannot be built yet. SoC support (DTS, Kconfig, drivers)
   must be completed first. Track progress in ``boards/ti/lp_mspm33c321a/``.

References
**********

- `LP-MSPM33C321A Product Page`_
- `MSPM33C321A Product Page`_

.. _LP-MSPM33C321A Product Page:
   https://www.ti.com/tool/LP-MSPM33C321A

.. _MSPM33C321A Product Page:
   https://www.ti.com/product/MSPM33C321A
