.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

MSPM0 Auxdisplay (Segment LCD) Test
####################################

Overview
********

This ztest suite exercises the ``auxdisplay`` driver API against the
MSPM0 segment LCD (SLCD) controller, using a 6-digit, 1-row display
(``slcd_panel``) wired to the ``auxdisplay_0`` device.

Coverage includes:

* capability reporting (columns/rows)
* display on/off
* cursor positioning (absolute, relative, and out-of-bounds errors)
* clear
* digit and letter rendering, including lowercase folding
* decorator characters (``.`` and ``:``) and their interaction with
  indicator segments
* custom indicators (annunciators/colons), including out-of-range handling
* position blinking
* writing while the display is off (shadow register behavior)
* a write/cursor stress loop

Several test cases (digit scans, blinking, indicator toggling) drive the
physical glass and are meant to be visually observed on hardware, not just
checked for return codes.

Building and Running
*********************

This test requires the ``lp_mspm0l2228`` board, which provides the
``slcd_panel`` devicetree overlay used by the suite.

.. code-block:: shell

   west build -p always -b lp_mspm0l2228 tests/drivers/auxdisplay/auxdisplay_mspm0
   west flash

Since this suite drives real display hardware, run it on the target board
and observe the glass during the digit/letter scans, blinking, and
indicator toggling test cases.
