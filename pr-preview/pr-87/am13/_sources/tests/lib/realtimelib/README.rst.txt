.. _realtimelib_test:

Real-Time Control Library Test
##############################

Overview
********

A ztest suite for the motor control building blocks in ``lib/realtimelib``. The
tests are grouped in four files.

Transforms, PI and first-order filter (``test_transforms_pi_filter.c``):

* Clarke transform for balanced three-phase currents, and the two-input form
  against the three-input form,
* Park and inverse Park: a round trip returns the input, a vector on the rotor
  angle is purely d-axis, and ``PARK_init()`` rejects a buffer that is too small,
* space vector generation: the line-to-line voltages are unchanged, the outputs
  are centered, and the bus voltage scale factor works,
* PI controller: proportional term, output limits, integrator and its clamp,
* first-order filter step response.

Filters and voltage (``test_filters.c``): the second-order filter (including a
non-zero ``a2``), the notch filter coefficients and difference equation, the
offset low-pass, phase voltage calculation, and the voltage reconstruction
setup.

Controllers (``test_controllers.c``): the PID and PR controllers against their
difference equations and output limits, and the volts per frequency profile.
The ``fwc``, ``vib_comp`` and ``dclink_ss`` modules are only checked for
initialisation and parameter storage; their algorithms are not verified.
``fwc`` has no run function.

Utilities (``test_utilities.c``): trajectory rate limiting and clamping, angle
generation and wrap, the ramp generator, and the CPU time counters.

The sine, cosine and square root calls run on the TMU, so the suite must run on
hardware.

Running
*******

.. code-block:: console

   west twister -p lp_am13e230 -T ../ti-zephyr/tests/lib/realtimelib --device-testing \
       --device-serial /dev/serial/by-id/<xds110-if00> --device-serial-baud 115200 \
       --west-flash="--openocd=<openocd>,--openocd-search=<openocd-scripts>"
