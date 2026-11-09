.. _fastrts_tmu_test:

TMU Fast Math Test
##################

Overview
********

A ztest suite for the AM13 TMU functions in ``<mathlib/fastrts_tmu.h>``. Each
test compares the TMU result with the C library over a set of inputs, including
negative values, ``|x| > 1`` for ``atanf_tmu()``, all four ``atan2f_tmu()``
quadrants and the ``atan2f_tmu(0, 0)`` case. A separate test checks the TMU
status flag helpers, including that ``set`` and ``clear`` return the previous
flag value.

The suite must run on hardware because the TMU is a physical coprocessor.

Running
*******

.. code-block:: console

   west twister -p lp_am13e230 -T ../ti-zephyr/tests/lib/mathlib --device-testing \
       --device-serial /dev/serial/by-id/<xds110-if00> --device-serial-baud 115200 \
       --west-flash="--openocd=<openocd>,--openocd-search=<openocd-scripts>"
