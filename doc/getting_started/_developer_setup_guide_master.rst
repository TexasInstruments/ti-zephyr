.. _ti-zephyr-github-setup:

Getting Started from GitHub
###########################

Use this guide to build the |soc_name| SDK workspace from the ``ti-zephyr``
repository on GitHub instead of the SDK installer. The installer ships the
toolchain, host tools and a ready west workspace; here you assemble them
yourself. Once the workspace is ready, continue with
:doc:`build_hello_world` and :doc:`flash_hello_world`.

.. note::
   This guide covers Ubuntu 22.04 LTS and later, other distributions may be supported.

Install the host dependencies
*****************************

Update the system:

.. code-block:: shell

   sudo apt update
   sudo apt upgrade

The minimum versions of the main dependencies are:

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - Tool
     - Min. Version
   * - CMake
     - 3.28.0
   * - Python
     - 3.12
   * - Devicetree compiler
     - 1.4.6

Install the packages with apt:

.. code-block:: shell

   sudo apt install --no-install-recommends git cmake ninja-build gperf \
   ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
   xz-utils file make gcc gcc-multilib g++-multilib libmagic1

Check the installed versions against the table above:

.. code-block:: shell

   cmake --version
   python3 --version
   dtc --version

Get the workspace
*****************

#. Create and activate a Python virtual environment:

   .. code-block:: shell

      python3 -m venv ~/zephyrproject/.venv
      source ~/zephyrproject/.venv/bin/activate

#. Install west, the Zephyr workspace manager:

   .. code-block:: shell

      pip install west

#. Create the workspace from the ``ti-zephyr`` manifest and fetch all modules.
   Use the following command to initialize the workspace.

   .. note::
      You can use ``west update --narrow -o=--depth=1`` for a faster fetch.

   .. parsed-literal::
      west init -m |ti_repo_url|.git --mr |release_branch_tag| ~/zephyrproject
      cd ~/zephyrproject
      west update

#. Install the Python requirements of Zephyr:

   .. code-block:: shell

      west packages pip --install

#. Register the Zephyr checkout in the CMake user package registry
   (``~/.cmake/packages``), so ``find_package(Zephyr)`` finds it. The SDK
   installer does not need this step.

   .. code-block:: shell

      west zephyr-export

Install the Zephyr SDK toolchain
********************************

The Zephyr SDK provides the cross-compilers, assembler, linker and the other
programs needed to build for the target. Install version 1.0.1, the version
shipped with the SDK installer:

.. code-block:: shell

   cd ~/zephyrproject/zephyr
   west sdk install --version 1.0.1

.. only:: mspm0 or mspm33 or am13

   Install OpenOCD
   ***************

   OpenOCD is required to flash |soc_name| boards. Install the build
   dependencies and clone the OpenOCD sources:

   .. code-block:: shell

      sudo apt install libusb-1.0-0-dev libhidapi-dev
      cd ~/zephyrproject

   .. only:: mspm0 or mspm33 or am13

      .. code-block:: shell

         git clone -b ti-zephyr https://github.com/TexasInstruments/ti-openocd openocd

   Build and install it. ``sudo make install`` puts OpenOCD on the ``PATH``, so
   ``west flash`` finds it without extra options:

   .. code-block:: shell

      cd openocd
      git submodule update --init --recursive
      ./bootstrap
      ./configure --enable-xds110
      make
      sudo make install

.. only:: am62l

   Prepare the SD card
   *******************

   AM62L boots from an SD card. Download TI's
   `WIC image <https://dr-download.ti.com/software-development/software-development-kit-sdk/MD-YjEeNKJJjt/11.02.08.02/tisdk-default-image-am62lxx-evm-11.02.08.02.rootfs.wic.xz>`__
   and write it to the SD card with a disk-imaging tool.

Next steps
**********

In every new terminal, activate the environment before building or flashing:

.. code-block:: shell

   source ~/zephyrproject/.venv/bin/activate

Build and flash the Hello World sample from ``~/zephyrproject/ti-zephyr`` as
described in :doc:`build_hello_world` and :doc:`flash_hello_world`.
