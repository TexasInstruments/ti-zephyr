Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``ti-zephyr`` folder of the SDK, with the Python environment activated as
described in the previous step.

Replace ``<board>`` with one of ``lp_em_cc2340r53``, ``lp_em_cc2340r53_wcsp``
or ``lp_em_cc2340r5moda``:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west build -p always -b <board> samples/hello_world
