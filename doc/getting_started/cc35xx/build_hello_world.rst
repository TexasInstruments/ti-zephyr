Step 3: Build Hello World Example
#################################

[placeholder only - needs update for cc35x1]

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``ti-zephyr`` folder of the SDK, with the Python environment activated as
described in the previous step.

Replace ``<board>`` with ``lp_em_cc35x1``:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west build -p always -b <board> samples/hello_world
