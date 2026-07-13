.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-boards:

TI Boards
#########

This section covers TI LaunchPad and EVM boards supported by the TI Zephyr SDK.

.. note::

   Boards marked **Upstream** are defined in the upstream Zephyr repository and
   are available automatically after ``west update``. Boards marked **SDK** are
   defined here in ``ti-zephyr/boards/ti/`` and are in active development or
   awaiting upstream acceptance.

MSPM0 Family
************

.. list-table::
   :header-rows: 1
   :widths: 25 20 15 40

   * - Board
     - SoC
     - Source
     - Notes
   * - :ref:`lp_mspm0g3519`
     - MSPM0G3519
     - Upstream + SDK
     - Active dev board; downstream copy in SDK for in-progress changes
   * - lp_mspm0g3507
     - MSPM0G3507
     - Upstream
     - ``west update`` → available
   * - lp_mspm0l1306
     - MSPM0L1306
     - Upstream
     - ``west update`` → available
   * - lp_mspm0l2228
     - MSPM0L2228
     - Upstream
     - ``west update`` → available

MSPM33 Family
*************

.. list-table::
   :header-rows: 1
   :widths: 25 20 15 40

   * - Board
     - SoC
     - Source
     - Notes
   * - :ref:`lp_mspm33c321a`
     - MSPM33C321A
     - SDK
     - In development; SoC support pending upstream

AM6x / AM2x Family
*******************

.. list-table::
   :header-rows: 1
   :widths: 25 20 15 40

   * - Board
     - SoC
     - Source
     - Notes
   * - am62l_evm
     - AM62L
     - Upstream
     - ``west update`` → available
   * - sk_am62
     - AM625
     - Upstream
     - ``west update`` → available
   * - am243x_evm
     - AM2434
     - Upstream
     - ``west update`` → available

CC13xx / CC26xx / CC32xx Family
*******************************

CC13xx, CC26xx, and CC32xx boards are defined in upstream Zephyr and available
after ``west update``. See the
`Zephyr board documentation <https://docs.zephyrproject.org/latest/boards/index.html>`_
for details.

.. toctree::
   :hidden:

   lp_mspm0g3519
   lp_mspm33c321a
