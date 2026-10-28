Step 1: Download, Install and Setup the SDK and Tools
#####################################################

.. note::

   This page is a placeholder. The download location and installation steps
   will be added once the MSPM0 SDK installer is finalized.

This guide covers Ubuntu version 24.04 LTS and later.

The SDK installer places the Zephyr workspace, toolchain and flashing tools in
<sdk-install-dir>. A few host packages must still be installed; see Setup below.

Download
********

Download the MSPM0 SDK installer from `MSPM0-ZEPHYR-SDK page <https://www.ti.com/tool/download/MSPM0-ZEPHYR-SDK>`__

Install
*******

Run the installer and follow its prompts. The SDK is installed to
``<sdk-install-dir>``, which the following steps refer to.

Setup
*****

Run the setup script once after the installation:

.. code-block:: shell

   cd <sdk-install-dir>
   source setup_env.sh

The script checks that the host packages are present (``git``, ``cmake``,
``ninja-build``, ``gperf``, ``python3`` and ``python3-venv``; it prints the
``sudo apt-get`` command to install any that are missing and exits, it does not
install them itself), creates a Python virtual environment in
``<sdk-install-dir>/.venv`` with ``west`` and the Python requirements of Zephyr
(Python 3.12 or later is required), and prepares the bundled host tools.

In every new terminal, activate the environment before building or flashing:

.. code-block:: shell

   source <sdk-install-dir>/.venv/bin/activate

Activating the environment sets ``ZEPHYR_BASE`` and ``ZEPHYR_SDK_INSTALL_DIR``,
so ``west`` finds the Zephyr tree and the toolchain in ``<sdk-install-dir>``.
No global CMake registry entries are written.
