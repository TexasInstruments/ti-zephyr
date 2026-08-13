.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

.. _watchdog_window:

TI MSPM0 WWDT Window Mode
##########################

Overview
********

Demonstrates the MSPM0 Window Watchdog Timer (WWDT) closed-window constraint
using the standard Zephyr watchdog API.

The WWDT enforces a three-zone feed window::

  t=0 ---- closed window ---- t=min == open window == t=max ---- RESET

- Feed before ``t=min``: immediate SYSRST (closed-window violation)
- Feed after  ``t=max``: immediate SYSRST (timeout)
- Feed in ``[t=min, t=max)``: safe zone

The upstream ``samples/drivers/watchdog`` sample sets ``WDT_MIN_WINDOW=0``
for MSPM0, so the closed window is never exercised.  This sample demonstrates
all three zones across two boots.

Behaviour
=========

**Boot 1 (clean):** Installs a window with ``min=500 ms``, ``max=2000 ms``.
Feeds five times at 1000 ms (inside the open window), then intentionally feeds
at 100 ms (inside the closed window) triggering an immediate SYSRST.

**Boot 2 (after violation):** ``hwinfo_get_reset_cause()`` returns
``RESET_WATCHDOG`` confirming the window violation.  The sample then feeds
correctly at 1000 ms intervals forever to demonstrate recovery.

Requirements
************

A board with a ``ti,mspm0-watchdog`` node enabled (``watchdog0`` alias).
No extra hardware is needed.

Supported Boards
****************

- ``lp_mspm0g3507``
- ``lp_mspm0g3519``
- ``lp_mspm0l2228``

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_mspm0l2228 ti-zephyr/samples/drivers/watchdog/watchdog_window
   west flash

Sample Output
*************

**Boot 1:**

.. code-block:: console

   TI MSPM0 WWDT Window Mode
   Clean boot.

   WWDT window:  min=500 ms (closed)  max=2000 ms (open)
   Feed interval: 1000 ms (inside open window)

   Phase 1: 5 correct feeds, then deliberate early feed.

     Feed 1/5: 1004 ms -- OK (inside window)
     Feed 2/5: 1005 ms -- OK (inside window)
     Feed 3/5: 1005 ms -- OK (inside window)
     Feed 4/5: 1005 ms -- OK (inside window)
     Feed 5/5: 1005 ms -- OK (inside window)

   Feeding at 100 ms (before min=500 ms) -- expects SYSRST...

**Boot 2 (after WWDT violation):**

.. code-block:: console

   TI MSPM0 WWDT Window Mode
   *** Rebooted by WWDT window violation ***

   WWDT window:  min=500 ms (closed)  max=2000 ms (open)
   Feed interval: 1000 ms (inside open window)

   Phase 2: recovered -- feeding correctly.
     Feed  1: 1005 ms (window=[500..2000) ms)
     Feed  2: 1005 ms (window=[500..2000) ms)
     Feed  3: 1005 ms (window=[500..2000) ms)
     Feed  4: 1005 ms (window=[500..2000) ms)
     Feed  5: 1005 ms (window=[500..2000) ms)
   Window-mode recovery confirmed. Done.
