.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _lp_mspm33c321a:

LP-MSPM33C321A
##############

Overview
********

The LP-MSPM33C321A LaunchPad is a low-cost development board for the
MSPM33C321A microcontroller, part of TI's MSPM33 Arm® Cortex®-M33 MCU family.

.. note::

   MSPM33C321A SoC support is under active development in this SDK.
   Board definition and SoC drivers are not yet accepted into upstream Zephyr.
   Track progress at ``ti-zephyr/boards/ti/lp_mspm33c321a/``.

.. list-table::
   :widths: 30 70

   * - **MCU**
     - MSPM33C321A (Arm® Cortex®-M33)
   * - **Debug probe**
     - Onboard XDS110
   * - **Product page**
     - `LP-MSPM33C321A <https://www.ti.com/tool/LP-MSPM33C321A>`_

Hardware
********

- Onboard XDS110 debug probe (USB-C)
- 40-pin BoosterPack compatible headers

Building
********

.. note::

   This board is not yet buildable. SoC DTS, Kconfig, and driver support
   must be added first. See ``boards/ti/lp_mspm33c321a/`` for current state.

References
**********

- `LP-MSPM33C321A Product Page`_
- `MSPM33C321A Product Page`_

.. _LP-MSPM33C321A Product Page:
   https://www.ti.com/tool/LP-MSPM33C321A

.. _MSPM33C321A Product Page:
   https://www.ti.com/product/MSPM33C321A
