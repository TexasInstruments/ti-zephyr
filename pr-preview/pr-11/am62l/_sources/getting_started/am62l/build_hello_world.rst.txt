Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``zephyr`` folder of the SDK.

Replace ``<board>`` with ``am62l_evm``:

.. code-block:: shell

   cd <sdk-install-dir>/zephyr
   west build -p always -b <board> ../ti-zephyr/samples/hello_world
