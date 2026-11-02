Step 4: Flash Hello World Example
#################################

Flash the application built in the previous step. Run the command from the same
``ti-zephyr`` folder used for the build:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west flash

.. attention::

   The CC23 boards flash with TI's OpenOCD. The build looks for it in
   ``$TI_OPENOCD_INSTALL_DIR/bin``, so set ``TI_OPENOCD_INSTALL_DIR`` to the
   OpenOCD install directory before running ``west flash``.

Verify
******

Open a serial terminal at 115200 8N1. Expected output:

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   Hello World! <board>
