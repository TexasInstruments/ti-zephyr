.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _lp_mspm0g3519:

LP-MSPM0G3519
#############

Overview
********

The LP-MSPM0G3519 LaunchPad is a low-cost evaluation and development board
for the MSPM0G3519 MCU. It features the onboard XDS110 debug probe with
EnergyTrace technology, providing a convenient single-USB development
experience.

.. list-table::
   :widths: 30 70

   * - **MCU**
     - MSPM0G3519 (Arm® Cortex®-M0+, 80 MHz)
   * - **Flash**
     - 512 KB (2 × 256 KB banks)
   * - **SRAM**
     - 128 KB (2 × 64 KB with ECC option)
   * - **Debug probe**
     - Onboard XDS110 with EnergyTrace
   * - **USB**
     - Single USB-C connector (power + debug)
   * - **Product page**
     - `LP-MSPM0G3519 <https://www.ti.com/tool/LP-MSPM0G3519>`_

Hardware
********

- 3 user LEDs (Blue: PB22, Red: PB26, Green: PB27)
- 2 user push buttons (S2: PA18, S3: PB3)
- 40-pin BoosterPack connectors (compatible with MSPM0 BoosterPacks)
- Onboard 32.768 kHz and 40 MHz crystals
- Temperature sensor circuit
- Light sensor circuit
- External OPA2365 for ADC evaluation

Supported Features
==================

.. zephyr:board-supported-hw::

The following table lists the drivers verified on this board:

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Peripheral
     - Driver
     - Notes
   * - GPIO
     - :dtcompatible:`ti,mspm0-gpio`
     - Input, output, interrupt
   * - UART
     - :dtcompatible:`ti,mspm0-uart`
     - UART0 (PA10/PA11); 115200 baud default
   * - SPI
     - :dtcompatible:`ti,mspm0-spi`
     - SPI0 (PB18/PB17/PB19), SPI1 (PB9/PB8/PB7)
   * - ADC
     - :dtcompatible:`ti,mspm0-adc`
     - Two 12-bit 4-Msps ADCs
   * - DAC
     - :dtcompatible:`ti,mspm0-dac`
     - 12-bit 1-Msps DAC
   * - CAN-FD
     - :dtcompatible:`ti,mspm0-can`
     - CANFD0 (PA12/PA13)
   * - RTC
     - :dtcompatible:`ti,mspm0-rtc`
     - Alarm + calendar modes
   * - DMA
     - :dtcompatible:`ti,mspm0-dma`
     - 7-channel DMA
   * - Timers
     - :dtcompatible:`ti,mspm0-timer`
     - 9 general-purpose timers

Building and Flashing
*********************

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/basic/blinky
   :board: lp_mspm0g3519
   :goals: build flash
   :compact:

Using OpenOCD (TI custom build):

.. code-block:: bash

   west flash --runner openocd

Using JLink:

.. code-block:: bash

   west flash --runner jlink

Debugging
*********

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/basic/blinky
   :board: lp_mspm0g3519
   :goals: debug
   :compact:

References
**********

- `LP-MSPM0G3519 Product Page`_
- `MSPM0G3519 Datasheet`_
- `MSPM0Gx51x Technical Reference Manual (TRM)`_
- `Zephyr MSPM0 support <https://docs.zephyrproject.org/latest/boards/ti/>`_

.. _LP-MSPM0G3519 Product Page:
   https://www.ti.com/tool/LP-MSPM0G3519

.. _MSPM0G3519 Datasheet:
   https://www.ti.com/product/MSPM0G3519

.. _MSPM0Gx51x Technical Reference Manual (TRM):
   https://www.ti.com/lit/slau846
