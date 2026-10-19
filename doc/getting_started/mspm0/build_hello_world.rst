Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``zephyr`` folder of the SDK.

Replace ``<board>`` with one of ``lp_mspm0g3507``, ``lp_mspm0g3519`` or
``lp_mspm0l2228``:

.. code-block:: shell

   cd <sdk-install-dir>/zephyr
   west build -p always -b <board> ../ti-zephyr/samples/hello_world
