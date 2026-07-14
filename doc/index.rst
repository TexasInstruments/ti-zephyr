.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-sdk:

TI Zephyr SDK Documentation
############################

.. toctree::
   :maxdepth: 1
   :caption: Getting Started

   getting_started/index

.. toctree::
   :maxdepth: 2
   :caption: Boards

   boards/index

.. toctree::
   :maxdepth: 2
   :caption: Samples & Demos

   samples/index

.. toctree::
   :maxdepth: 1
   :caption: Contributing

   contribute/index

.. toctree::
   :maxdepth: 1
   :caption: Releases

   releases/release-notes-v1.0.0

Overview
********

The TI Zephyr SDK provides Zephyr RTOS support for Texas Instruments
microcontrollers and microprocessors across all product families including
MSPM0, MSPM33, AM62x, AM64x, AM243x, and CC13xx/CC26xx/CC32xx.

The SDK is a downstream Zephyr staging area: content developed here is
upstreamed to `Zephyr RTOS <https://github.com/zephyrproject-rtos/zephyr>`_
as it matures.

Supported Device Families
==========================

.. list-table::
   :header-rows: 1
   :widths: 20 30 50

   * - Family
     - Example Devices
     - Status
   * - MSPM0
     - MSPM0G3519, MSPM0G3507, MSPM0L2228
     - Production (upstream Zephyr)
   * - MSPM33
     - In development (this SDK)
   * - AM62x
     - AM625, AM623
     - Planned
   * - AM64x / AM243x
     - AM6442, AM2434
     - Upstream Zephyr (sk_am64, am243x_evm)
   * - CC13xx/CC26xx
     - CC1352, CC2652
     - Upstream Zephyr

Quick Start
===========

.. code-block:: bash

   # Clone the SDK (name gives the workspace its version label)
   cd ~/ti
   git clone https://github.com/TexasInstruments/ti-zephyr zephyr_ti_sdk_v1.0.0
   cd zephyr_ti_sdk_v1.0.0

   # Set up workspace, toolchain, and OpenOCD
   python3 install.py

   # Activate environment
   source ~/ti/env_setup.sh

   # Build and flash
   west build -b lp_mspm0g3519 zephyr_ti_sdk_v1.0.0/samples/basic/blinky
   west flash

See :ref:`ti-zephyr-getting-started` for the full installation guide.

Indices
*******

* :ref:`genindex`
* :ref:`search`
