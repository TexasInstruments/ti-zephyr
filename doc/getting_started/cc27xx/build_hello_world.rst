Step 3: Build Hello World Example
#################################

Build the Hello World sample (``ti-zephyr/samples/hello_world``) from the
``ti-zephyr`` folder of the SDK, with the Python environment activated as
described in the previous step.

Replace ``<board>`` with one of ``lp_em_cc2745r10_q1/cc2745r10_q1``,
``lp_em_cc2755p10`` or ``lp_em_cc2755p20``:

.. code-block:: shell

   cd <sdk-install-dir>/ti-zephyr
   west build -p always -b <board> samples/hello_world

If a CC2755R10 device is mounted on the ``lp_em_cc2745r10_q1`` board, use
``lp_em_cc2745r10_q1/cc2755r10`` instead.
