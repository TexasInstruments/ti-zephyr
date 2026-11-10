.. _realtimelib_test:

Real-Time Control Library Test
##############################

Overview
********

A ztest suite for the motor control building blocks in ``lib/realtimelib``. It
checks:

* the Clarke transform for balanced three-phase currents, and that the
  two-input form matches the three-input form,
* the Park and inverse Park transforms: a round trip returns the input, a
  vector on the rotor angle is purely d-axis, and ``PARK_init()`` rejects a
  buffer that is too small,
* space vector generation: the line-to-line voltages are unchanged, the
  outputs are centered, and the bus voltage scale factor works,
* the PI controller: the proportional term, the output limits, and the
  integrator with its anti-windup clamp,
* the first-order filter step response.

The sine, cosine and square root calls run on the TMU, so the suite must run
on hardware.

Running
*******

.. code-block:: console

   west twister -p lp_am13e230 -T ../ti-zephyr/tests/lib/realtimelib --device-testing \
       --device-serial /dev/serial/by-id/<xds110-if00> --device-serial-baud 115200 \
       --west-flash="--openocd=<openocd>,--openocd-search=<openocd-scripts>"
