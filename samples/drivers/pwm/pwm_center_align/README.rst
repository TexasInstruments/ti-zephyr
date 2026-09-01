.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

.. _pwm_center_align:

TI MSPM0 PWM Center-Aligned Mode
#################################

Overview
********

Demonstrates center-aligned (up-down counting) PWM on TI MSPM0 TIMA/TIMG
timers, enabled via the ``ti,pwm-mode = "CENTER_ALIGN"`` board overlay
property.

In this mode the MSPM0 driver configures the CC shadow update to
``CCUPD_ZERO_LOAD_EVT``, latching new duty-cycle values at both the
zero-crossing **and** the counter peak.  This provides two glitch-free update
opportunities per period (vs. one in edge-aligned mode) and produces a
pulse that is symmetric around the centre of the period — the standard
waveform for motor-control applications (FOC, BLDC).

Behaviour
=========

The sample performs two operations:

1. **Duty sweep 10 → 90 %** (600 ms per step): demonstrates smooth
   center-aligned brightness ramp on the on-board LED.

2. **Shadow-latch test**: issues five ``pwm_set()`` calls back-to-back with
   no sleep.  All writes arrive within one 250 ms period; only the last
   value takes effect at the next update event — no partial or glitched
   pulses.

Requirements
************

A board with a ``ti,pwm-mode = "CENTER_ALIGN"`` overlay (provided for all
supported boards).  The PWM output is on pin PA0 (on-board LED).

For waveform symmetry verification, connect an oscilloscope to PA0.

Supported Boards
****************

- ``lp_mspm0g3507``
- ``lp_mspm0g3519``
- ``lp_mspm0l2228``

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_mspm0l2228 ti-zephyr/samples/drivers/pwm/pwm_center_align
   west flash

Sample Output
*************

.. code-block:: console

   TI MSPM0 PWM - Center-Aligned Mode
     Timer clock  : 15625 Hz
     Period       : 250000000 ns (3906 cycles)
     LOAD register: 1953 cycles (period/2, up-down counting)
     CC update    : CCUPD_ZERO_LOAD_EVT (shadow-latched, 2x per period)

   Duty sweep 10% -> 90% (600 ms per step)
     duty= 10%  pulse= 25000000 ns  (390 cycles)
     ...
     duty= 90%  pulse=225000000 ns  (3515 cycles)

   Shadow-latch test: 5 rapid pwm_set() calls, no sleep
     write 0: duty=20%
     write 1: duty=80%
     write 2: duty=35%
     write 3: duty=65%
     write 4: duty=50%
   All 5 writes completed in 1 ms (period = 250 ms)
   Effective duty: 50% (last-write wins, previous values discarded)
   Shadow-latch confirmed: all writes within one period

   Holding 50% duty. Done.
