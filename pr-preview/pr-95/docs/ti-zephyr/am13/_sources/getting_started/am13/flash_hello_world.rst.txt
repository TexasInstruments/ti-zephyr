Step 4: Flash Hello World Example
#################################

Flash the application built in the previous step. Run the command from the same
``ti-zephyr`` folder used for the build:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west flash

.. attention::

   ``west flash`` may not work as expected in the case of multiple connected
   boards / it being unable to detect the device serial number.
   This can be provided using ``--cmd-pre-init "adapter serial <serial number>"``.
   The serial number for TI LaunchPads usually shows up after ``CMSIS-DAP_`` in the
   output of ``ls -l /dev/serial/by-id``.

Verify
******

Open a serial terminal at 115200 8N1. Expected output:

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   Hello World! <board>
