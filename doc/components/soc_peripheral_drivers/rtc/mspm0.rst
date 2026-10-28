Overview
********

The MSPM0 RTC 
(:ws-doc:`ti,mspm0-rtc <zephyr/build/dts/api/bindings/rtc/ti,mspm0-rtc.html>`)
is a hardware calendar peripheral clocked from a 32768 Hz source. The driver
runs it in binary calendar mode and implements the standard Zephyr RTC API
directly on the peripheral, with additional MSPM0-specific timer extensions.
The ``RTC_x`` variant, found in the Low-Frequency Subsystem (LFSS), shares
the same register layout.

Features Supported
******************

- Calendar time: set and get (year, month, day, hour, minute, second)
- Two independent alarms with callbacks (``CONFIG_RTC_ALARM``)
- Update callback fired once per second (``CONFIG_RTC_UPDATE``)
- Frequency calibration in ppb (``CONFIG_RTC_CALIBRATION``)
- Interval timer (RTCTEV): interrupt at every minute, hour, midnight, or noon
  (``CONFIG_RTC_TI_MSPM0_INTERVAL_TIMER``)
- Prescaler timers RT0PS and RT1PS with configurable sub-second intervals
  (``CONFIG_RTC_TI_MSPM0_PS_TIMER``)

Features NOT Supported
***********************

- BCD mode (driver uses binary calendar mode only)
- DMA-based time-stamp capture
- Tamper detection (time-stamp registers are present in hardware but not
  exposed through the Zephyr API)

Usage
*****

The driver implements Zephyr's standard RTC API (``<zephyr/drivers/rtc.h>``).
MSPM0-specific extensions (interval timer and prescaler timers) are declared in
``<zephyr/drivers/rtc/rtc_ti_mspm0.h>``.

Enabling the Driver
===================

Enable the RTC node in the board devicetree (or an application overlay) and
the RTC subsystem in ``prj.conf``. ``CONFIG_RTC_TI_MSPM0`` is selected
automatically when an enabled ``ti,mspm0-rtc`` node is present.

.. code-block:: devicetree

   &rtc0 {
           status = "okay";
           alarms-count = <2>;
   };

.. code-block:: kconfig

   CONFIG_RTC=y

Devicetree Bindings
===================

The node properties are described in 
:ws-doc:`ti,mspm0-rtc <zephyr/build/dts/api/bindings/rtc/ti,mspm0-rtc.html>`
devicetree binding.

Optional Kconfig
================

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Symbol
     - Effect
   * - ``CONFIG_RTC_ALARM``
     - Enables alarm support. Requires the ``alarms-count`` devicetree
       property.
   * - ``CONFIG_RTC_UPDATE``
     - Enables update callback (:c:func:`rtc_update_set_callback`), fired
       once per second.
   * - ``CONFIG_RTC_CALIBRATION``
     - Enables frequency calibration in ppb (:c:func:`rtc_set_calibration`
       and :c:func:`rtc_get_calibration`).
   * - ``CONFIG_RTC_TI_MSPM0_INTERVAL_TIMER``
     - Enables the RTCTEV interval timer and the
       :c:func:`rtc_mspm0_set_interval_callback` extension API.
   * - ``CONFIG_RTC_TI_MSPM0_PS_TIMER``
     - Enables the RT0PS / RT1PS prescaler timers and the
       :c:func:`rtc_mspm0_set_ps_callback` extension API.

Extension API
=============

Two non-standard callbacks are available when the respective Kconfig options
are enabled.

Passing ``NULL`` as the callback disables the corresponding timer.

Interval Timer
--------------

Fires at a calendar boundary. The ``interval`` argument of
:c:func:`rtc_mspm0_set_interval_callback` takes one of the following values:

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - Option
     - Interrupt fires
   * - ``RTC_MSPM0_INTERVAL_MINUTE``
     - when the minute changes
   * - ``RTC_MSPM0_INTERVAL_HOUR``
     - when the hour changes
   * - ``RTC_MSPM0_INTERVAL_MIDNIGHT``
     - every day at midnight (00:00)
   * - ``RTC_MSPM0_INTERVAL_NOON``
     - every day at noon (12:00)

.. code-block:: c

   #include <zephyr/drivers/rtc/rtc_ti_mspm0.h>

   static void on_interval(const struct device *dev, void *user_data)
   {
           /* called every minute */
   }

   rtc_mspm0_set_interval_callback(rtc_dev, RTC_MSPM0_INTERVAL_MINUTE,
                                   on_interval, NULL);

Prescaler Timers
----------------

Fires at a sub-second rate. The RTC has two prescaler timers, selected with
the ``timer`` argument of :c:func:`rtc_mspm0_set_ps_callback`:

- ``RTC_MSPM0_PS_TIMER_0`` (RT0PS): microsecond to millisecond intervals
- ``RTC_MSPM0_PS_TIMER_1`` (RT1PS): millisecond to second intervals

The ``interval`` argument must come from the set that belongs to the selected
timer.

**RT0PS** (``RTC_MSPM0_PS_TIMER_0``)

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - Option
     - Interval
   * - ``RTC_MSPM0_RT0PS_DIV8``
     - 244 microseconds
   * - ``RTC_MSPM0_RT0PS_DIV16``
     - 488 microseconds
   * - ``RTC_MSPM0_RT0PS_DIV32``
     - 976 microseconds
   * - ``RTC_MSPM0_RT0PS_DIV64``
     - 1.95 milliseconds
   * - ``RTC_MSPM0_RT0PS_DIV128``
     - 3.90 milliseconds
   * - ``RTC_MSPM0_RT0PS_DIV256``
     - 7.81 milliseconds

**RT1PS** (``RTC_MSPM0_PS_TIMER_1``)

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - Option
     - Interval
   * - ``RTC_MSPM0_RT1PS_DIV2``
     - 15.6 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV4``
     - 31.2 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV8``
     - 62.5 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV16``
     - 125 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV32``
     - 250 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV64``
     - 500 milliseconds
   * - ``RTC_MSPM0_RT1PS_DIV128``
     - 1 second
   * - ``RTC_MSPM0_RT1PS_DIV256``
     - 2 seconds

.. code-block:: c

   static void on_ps(const struct device *dev, void *user_data)
   {
           /* called every 2 seconds */
   }

   rtc_mspm0_set_ps_callback(rtc_dev, RTC_MSPM0_PS_TIMER_1,
                             RTC_MSPM0_RT1PS_DIV256, on_ps, NULL);

Samples and Tests
*****************

Samples
=======

- :ti-ref:`RTC Calendar <rtc_calendar>`

  - Sets the calendar time and configures alarms.
  - Demonstrates the interval and prescaler timer callbacks.

Tests
=====

- ``rtc_api`` — Validates the standard Zephyr RTC API (time, alarms, update
  callback, calibration).
