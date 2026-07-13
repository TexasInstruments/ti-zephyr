.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-getting-started:

Getting Started
###############

This guide walks through installing the TI Zephyr SDK and building your
first application on a TI LaunchPad.

Prerequisites
*************

* Python 3.10 or later
* Git 2.x or later
* 8 GB free disk space (workspace + toolchain)

Install the SDK
***************

Create a versioned SDK directory, clone ``ti-zephyr`` inside it, then run
``install.py``. The workspace is the SDK directory (parent of the clone),
keeping it isolated from other tools in ``~/ti/``.

.. code-block:: bash

   # Create versioned SDK dir
   mkdir ~/ti/zephyr_ti_sdk_v1.0.0
   cd ~/ti/zephyr_ti_sdk_v1.0.0

   # Clone ti-zephyr inside it
   git clone https://github.com/TexasInstruments/ti-zephyr ti-zephyr
   cd ti-zephyr

   # Run the installer
   python3 install.py

.. tip::

   Clone into any directory — ``install.py`` always uses the clone's parent
   as the workspace root. Name the SDK dir to reflect the SDK version.

The installer performs these steps automatically:

1. Creates a Python virtual environment (``.venv/``)
2. Runs ``west init`` and ``west update`` to fetch Zephyr and all modules
3. Installs Zephyr host tools from ``requirements.txt``
4. Offers to set up the Zephyr SDK NG compiler toolchain
5. Offers to set up TI custom OpenOCD for ``west flash``
6. Generates ``env_setup.sh`` / ``env_setup.ps1`` / ``env_setup.bat``

.. note::

   The compiler toolchain and OpenOCD can be installed separately at any time:

   .. code-block:: bash

      python3 zephyr_ti_sdk_v1.0.0/scripts/setup_toolchain.py
      python3 zephyr_ti_sdk_v1.0.0/scripts/setup_openocd.py --remote <url>

Workspace Layout
================

After installation, all SDK content is isolated in the versioned directory:

.. code-block:: none

   ~/ti/zephyr_ti_sdk_v1.0.0/   ← versioned SDK workspace root
   ├── ti-zephyr/                ← the clone (west manifest)
   ├── .west/config              ← manifest.path = ti-zephyr
   ├── .venv/                    ← Python tools
   ├── toolchains/               ← compiler + OpenOCD
   ├── zephyr/                   ← Zephyr RTOS
   ├── modules/                  ← HAL/TI, mbedtls, etc.
   ├── bootloader/               ← MCUboot
   └── env_setup.sh              ← activate script

Activate Environment
********************

Source the environment script in every new shell session:

.. code-block:: bash

   source ~/ti/env_setup.sh

The script sets:

* ``ZEPHYR_BASE`` — path to the Zephyr source tree
* ``TI_ZEPHYR_BASE`` — path to the TI Zephyr SDK repo
* ``ZEPHYR_SDK_INSTALL_DIR`` — path to the compiler toolchain
* Adds OpenOCD to ``PATH`` (if installed)
* Activates the Python virtual environment

Build Your First Application
*****************************

Build the :zephyr:code-sample:`blinky` sample for the MSPM0G3519 LaunchPad:

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/basic/blinky
   :board: lp_mspm0g3519
   :goals: build flash
   :compact:

The LED on the LaunchPad will begin blinking at 1 Hz.

Run Tests
*********

Run the hardware-in-the-loop Twister test suite:

.. code-block:: bash

   # Edit the hardware map first (set your serial device and OpenOCD path)
   vi zephyr_ti_sdk_v1.0.0/tests/boards/lp_mspm0g3519/map.yml

   # Run all tests
   python3 zephyr_ti_sdk_v1.0.0/tests/run.py --board lp_mspm0g3519

Next Steps
**********

* :ref:`ti-zephyr-boards` — explore supported LaunchPad boards
* :ref:`ti-zephyr-samples` — TI-specific sample applications
* `Zephyr Application Development Guide <https://docs.zephyrproject.org/latest/develop/application/index.html>`_
* `Zephyr Board Porting Guide <https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html>`_
