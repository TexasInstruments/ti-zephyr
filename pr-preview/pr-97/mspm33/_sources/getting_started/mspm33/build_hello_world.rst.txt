Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``ti-zephyr`` folder of the SDK.

Replace ``<board>`` with ``lp_mspm33c321a``:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west build -p always -b <board> samples/hello_world
