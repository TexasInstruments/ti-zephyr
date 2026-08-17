.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

.. _pwm_capture_accuracy:

TI MSPM0 PWM Precision Analysis
################################

Overview
********

Generates a known PWM signal on the ``pwm-out`` device and captures it on
``pwm-cap``, measuring four precision properties of the MSPM0 PWM subsystem.
All test frequencies are derived at runtime from the capture timer clock, so
the sample adapts automatically to any board overlay and timer configuration.

Unlike the upstream ``samples/drivers/pwm/capture`` sample which captures an
unknown external signal, this sample generates a precisely programmed signal and
measures the round-trip accuracy against theoretical limits.

Hardware requirement
********************

Wire the ``pwm-out`` pin to the ``pwm-cap`` pin (see board overlay for specific
pin numbers).

Sections
********

1. **Jitter** — captures the same signal 5 times at three frequencies. Confirms
   MSPM0 timer output is bit-reproducible (jitter = 0 cycles).

2. **Frequency accuracy** — sweeps five frequencies and compares measured vs
   programmed frequency against the theoretical ±1-cycle quantisation limit.
   Note: the MSPM0 timer counts ``LOAD+1`` cycles per period, so the measured
   frequency is consistently 1 cycle below the programmed value — this is
   correct hardware behaviour, not a driver bug.

3. **Duty accuracy** — at a mid-range frequency, sweeps duty 10–90% and shows
   that duty error scales with ``1/pulse_cycles``, confirming ±1-cycle rounding.

4. **Dynamic tracking** — changes duty each period and captures it, confirming
   every update is applied within one period without loss or corruption.

Supported boards
****************

- ``lp_mspm0g3507`` — wire PA15 (pwm-out) to PB4 (pwm-cap)
- ``lp_mspm0g3519`` — wire PB6 (pwm-out) to PB4 (pwm-cap)
- ``lp_mspm0l2228`` — wire PA15 (pwm-out) to PA29 (pwm-cap)

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_mspm0g3507 ti-zephyr/samples/drivers/pwm/pwm_capture_accuracy
   west flash

Sample Output
*************

.. code-block:: console

   TI MSPM0 PWM Precision Analysis
     Output  (pwma0): 5000000 Hz
     Capture (pwma1): 5000000 Hz
     Wire pwm-out to pwm-cap (see board overlay for pin names)

   === 1. Jitter: 5 captures per frequency ===
     200 Hz    25001   25001   25001    0 cycles
    1000 Hz     5001    5001    5001    0 cycles
    5000 Hz     1001    1001    1001    0 cycles

   === 2. Frequency accuracy vs +-1 cycle quantisation limit ===
      200.00 Hz   199.99 Hz      -40      +-    40    AT LIMIT
      500.00 Hz   499.95 Hz     -100      +-   100    AT LIMIT
     ...
   Max error: 1000 ppm

   === 3. Duty accuracy at 1000 Hz ===
     10%   10.00%   10.01%   501    +0.01%   +-0.19%
     ...

   === 4. Dynamic duty tracking at 1000 Hz ===
   8/8 duty changes tracked correctly.
