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

Replace ``<board>`` with the ID of your board. The supported boards are listed
in the |soc_datasheet|.

.. code-block:: console

   west build -p always -b <board> ti-zephyr/samples/hello_world
   west flash

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   Hello World! <board>
