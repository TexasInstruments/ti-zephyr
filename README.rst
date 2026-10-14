ti-zephyr
#########

TI's downstream Zephyr SDK: a west-managed module for TI silicon.

Repository layout
******************

.. list-table::
   :header-rows: 1

   * - Path
     - Purpose
   * - ``west.yml``
     - West manifest — pins ``zephyr`` and every TI-relevant module
   * - ``drivers/``
     - Out-of-tree drivers
   * - ``samples/``
     - Out-of-box samples for TI boards
   * - ``tests/``
     - ztest/twister test suites for TI boards
   * - ``dts/``, ``include/``, ``lib/``
     - Devicetree bindings, public headers, libraries
   * - ``applications/``
     - Reference application scaffolding
   * - ``cmake/``
     - Shared CMake extensions
   * - ``doc/``
     - Sphinx documentation sources
   * - ``modules/``
     - TI modules pending their own west projects
   * - ``scripts/``
     - Development tooling

Getting started
****************
This guide covers **Ubuntu version 24.04 LTS** and later.

.. code-block:: shell

   sudo apt update
   sudo apt upgrade

Install dependencies
^^^^^^^^^^^^^^^^^^^^^
Next, install the host tools Zephyr needs to configure and build applications. The instructions below use the recommended package manager for each operating system so the tools are available from your terminal.

The current minimum required versions for the main dependencies are:


.. list-table::
   :header-rows: 1

   * - Tool
     - Min. Version
   * - CMake
     - 3.28.0
   * - Python
     - 3.12
   * - Devicetree compiler
     - 1.4.6

1. Use apt to install the required dependencies:

.. code-block:: shell

 # Ubuntu
 sudo apt install --no-install-recommends git cmake ninja-build gperf \
 ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
 xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1


2. Verify versioning using the following command :

.. code-block:: shell

 # Ubuntu
 cmake --version
 python3 --version
 dtc --version

Check those against the versions in the table in the beginning of this section.

Get Zephyr and install Python dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Create a new virtual environment:

.. code-block:: shell

 # Ubuntu
 python3 -m venv ~/zephyrproject/.venv

2. Activate the virtual environment:

.. code-block:: shell

 # Ubuntu
 source ~/zephyrproject/.venv/bin/activate

3. Install west:

West is Zephyr’s workspace manager; the next commands use it to create and update the workspace.

.. code-block:: shell

 # Ubuntu
 pip install west

4. Get the Zephyr source code:

.. code-block:: shell

 # Ubuntu
 west init -m https://github.com/TexasInstruments/ti-zephyr.git --mr main ~/zephyrproject
 cd ~/zephyrproject
 west update

1. Install Zephyr’s Python dependencies:

.. code-block:: shell

 # Ubuntu
 west packages pip --install

6. Export a Zephyr CMake package. This registers your current Zephyr checkout in CMake’s user package registry so find_package(Zephyr) can locate it automatically when building applications.

.. code-block:: shell

 # Ubuntu
 west zephyr-export

Install the Zephyr SDK
^^^^^^^^^^^^^^^^^^^^^^

The Zephyr Software Development Kit (SDK) contains toolchains for each of Zephyr’s supported architectures. Those toolchains include the compiler, assembler, linker, and other programs required to build Zephyr applications for your target hardware.

Install the Zephyr SDK with west sdk install from the Zephyr repository:

.. code-block:: shell

 # Ubuntu
 cd ~/zephyrproject/zephyr
 west sdk install

Install the Host tools
^^^^^^^^^^^^^^^^^^^^^^^

Install OpenOCD
""""""""""""""""
For the following list of device(s), OpenOCD is required for flashing

* MSPM0 SOCs
* MSPM33 SOCs
* AM13 SOCs

In the base folder run the following commands to get openocd onto the system

For the following list of device(s) use the below repository for openocd

* MSPM0 SOCs
* AM13 SOCs

.. code-block:: shell

 # Ubuntu
 git clone https://github.com/openocd-org/openocd/

For the following list of device(s) use the below repository for openocd

* MSPM33 SOCs

.. code-block:: shell

 # Ubuntu
 git clone -b ti-release https://github.com/TexasInstruments/ti-openocd
 mv ti-openocd openocd

Run the following commands from the base folder to load openocd onto the system

.. code-block:: shell

 #Ubuntu
 cd openocd
 git submodule update --init --recursive
 cd jimtcl
 ./configure
 make
 sudo make install
 cd .. 
 sudo apt install libusb-1.0-0-dev
 sudo apt install libhidapi-dev
 ./bootstrap
 ./configure --enable-xds110
 make
 sudo make install

Install SD Card based flashing support program
"""""""""""""""""""""""""""""""""""""""""""""""
For the following list of device(s), below tool is required

* AM62L SOCs

Download TI’s official `WIC <https://dr-download.ti.com/software-development/software-development-kit-sdk/MD-YjEeNKJJjt/11.02.08.02/tisdk-default-image-am62lxx-evm-11.02.08.02.rootfs.wic.xz>`_ and flash the WIC file with an etching software onto an SD-card.

Build the blinky Sample
^^^^^^^^^^^^^^^^^^^^^^^

Build the Blinky with west build. Replace <your-board-name> with the name of your board:

.. code-block:: shell

 #Ubuntu
 cd ~/zephyrproject/ti-zephyr
 west build -p always -b <your-board-name> samples/hello_world

Flashing Sample
^^^^^^^^^^^^^^^
Flashing using Openocd
""""""""""""""""""""""
For the following list of device(s), below method can be used

* MSPM0 SOCs
* MSPM33 SOCs
* AM13 SOCs

Using Openocd to Flash
~~~~~~~~~~~~~~~~~~~~~~

The following command can be used to flash using OpenOCD

.. code-block:: shell

 # Ubuntu
 cd zephyrproject/ti-zephyr
 west flash --openocd ../openocd/src/openocd --openocd-search ../openocd/tcl/


SD Card based flashing
""""""""""""""""""""""
For the following list of device(s), below method can be used

* AM62L SOCs

Copy the compiled `zephyr.bin` to the first FAT partition of the SD card and plug the SD card into the board. Power it up and stop the u-boot execution at prompt.

Use U-Boot to load and start zephyr.bin:

.. code-block:: shell

 #Ubuntu
 fatload mmc 1:1 0x82000000 zephyr.bin; dcache flush; icache flush; dcache off; icache off; go 0x82000000

The Zephyr application should start running on the A53 core.

License
*******

LicenseRef-TI-TFL License — see the `LICENSE file <LICENSE>`_.
