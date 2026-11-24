Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``ti-zephyr`` folder of the SDK.

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west blobs fetch hal_ti
   west build -p always -b am62l_evm/am62l3/a53 samples/hello_world
