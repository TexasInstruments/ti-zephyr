.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _eqep_freq_meas:

TI EQEP Frequency Measurement
#############################

Overview
********

Measures the frequency of a pulse train with the EQEP frequency-register 
(fr) method (a fixed unit-timer window). A software timer toggles a GPIO to
produce a 100 Hz signal at 25% duty. EQEP0 counts rising edges on phase A,
and the change in position over each 1 second window is the measured
frequency in hertz.

The position counter runs from 0 to 999 and then restarts, so it wraps every
few windows. Readings within 1 Hz of 100 Hz are in tolerance: the
software-timed edges can gain or lose one count per window. A reading outside
that range is marked ``[out of tolerance]``.

Behaviour
=========

1. **Signal.** ``signal-gpios`` is driven high for one step and low for three
   steps, at 400 steps per second (100 Hz, 25% duty).

2. **Decoder.** EQEP0 is configured for up-count on rising edges only, with
   the position counter resetting at its maximum value. The unit-timer alarm
   is re-armed on every expiry so the windows stay back to back.

3. **Report.** Each completed window prints the latched position and the
   position delta. A delta that crosses the maximum is printed with
   ``(counter wrapped)``.

Requirements
************

One jumper wire from the generated signal to the EQEP phase-A pin. No
external encoder is required.

Wiring
======

On ``lp_am13e230``, connect BoosterPack header J4 pin 40 (PB3) to J2 pin 11
(PB11). PB3 is the GPIO that generates the pulse train. PB11 (GPIO43) is
routed through Input X-BAR channel 16 (INPUTXBAR17) to EQEP0A.

Supported Boards
****************

- ``lp_am13e230``

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_am13e230 samples/drivers/counter/eqep_freq_meas
   west flash

Sample Output
*************

.. code-block:: console

   TI EQEP frequency measurement sample
   eQEP clock 200000000 Hz; position top 999
   Signal: 100 Hz, 25% duty
   position  100, measured 100 Hz
   position  200, measured 100 Hz
   ...
   position    0, measured 100 Hz (counter wrapped)
