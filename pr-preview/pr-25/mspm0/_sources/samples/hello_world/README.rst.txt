.. _sample-hello-world-ti:

Hello World
###########

Overview
********

A minimal application that prints "Hello World" to the UART console. Use this
sample to verify that the build toolchain, west workspace, and board flashing
are all working correctly before moving on to more complex samples.

The output includes the board name so you can confirm the correct board target
was used.

Building and Running
********************

Build for the MSPM0G3507 LaunchPad:

.. code-block:: console

   west build -p always -b lp_mspm0g3507 ti-zephyr/samples/hello_world
   west flash --openocd ../openocd/src/openocd --openocd-search ../openocd/tcl/

To target a different board, replace ``lp_mspm0g3507`` with the desired board
ID. See :ref:`ti-zephyr-introduction` for the full list of supported boards.

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   Hello World! lp_mspm0g3507
