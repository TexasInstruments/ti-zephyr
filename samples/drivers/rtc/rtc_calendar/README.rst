.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

.. _rtc_calendar:

TI MSPM0 RTC Calendar
######################

Overview
********

Demonstrates the Zephyr RTC driver API on TI MSPM0 LaunchPad boards.

The sample:

1. **Sets** the calendar date/time to 2024-11-17 04:19:00.
2. **Sets an alarm** to fire at minute :20 (one minute later).
3. **Reads back** the current time every second when ``CONFIG_RTC_UPDATE`` is
   disabled, or via the once-per-second update callback when it is enabled.
4. **Calibration** — applies a -10 000 ppb correction when
   ``CONFIG_RTC_CALIBRATION`` is enabled.
5. **Interval timer** — fires a callback once per minute via
   ``rtc_mspm0_set_interval_callback()``.
6. **Prescaler timers** — fires RT0PS at ~7.8 ms and RT1PS at ~1 s via
   ``rtc_mspm0_set_ps_callback()``.

SDK analog: ``rtc_calendar``

Supported boards
****************

- ``lp_mspm0g3507``
- ``lp_mspm0g3519``
- ``lp_mspm0l2228``

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_mspm0g3507 ti-zephyr/samples/drivers/rtc/rtc_calendar
   west flash

Sample Output
*************

.. code-block:: console

   RTC calibration set to -10000 ppb
   Alarm set for minute :20
   RTC date and time: 2026-09-07 04:19:00
   RTC date and time: 2026-09-07 04:19:01
   RT0PS prescaler event fired
   RT1PS prescaler event fired
   ...
   RTC interval event fired
   Alarm 0 fired!
