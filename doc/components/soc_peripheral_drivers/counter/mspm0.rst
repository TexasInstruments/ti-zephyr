MSPM0 Counter
*************

Overview
========

The MSPM0 and MSPM33 General-Purpose Timer (GPTIMER) peripherals are exposed through Zephyr's
standard counter API (``<zephyr/drivers/counter.h>``). Each GPTIMER instance hosts a child
devicetree node (``ti,mspm0-timer-counter``) that binds to this driver. The counter runs in
up-counting mode and reloads to zero when it reaches the programmed top value. Compare-capture (CC)
registers generate one-shot alarm interrupts when the counter reaches a programmed value.

Features Supported
==================

The driver implements all mandatory counter API operations and the following optional operations:

- Up-counting mode; :c:func:`counter_is_counting_up` always returns ``true``
- 16-bit or 32-bit counter resolution, selected per instance in devicetree
- Up to 4 independent alarm channels backed by hardware CC registers (CC0–CC3); actual channel count
  is set per instance via the ``channels`` devicetree property
- Top value and top-wrap callback (:c:func:`counter_set_top_value`)
- Alarm scheduling in relative and absolute modes (:c:macro:`COUNTER_ALARM_CFG_ABSOLUTE`)
- Late alarm detection via :c:macro:`COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE`: when an absolute alarm
  deadline has already passed at call time, the driver pends the NVIC interrupt in software and
  fires the callback on the next interrupt dispatch instead of silently discarding the alarm
- Reliable alarm cancellation: :c:func:`counter_cancel_channel_alarm` does not produce spurious
  callbacks after the alarm has fired or been cancelled
- Guard period (:c:func:`counter_set_guard_period`, :c:func:`counter_get_guard_period`)
- :c:func:`counter_reset` — resets the counter to zero while it continues running
- :c:func:`counter_set_value` — writes an arbitrary value to the counter while it is running
- :c:func:`counter_get_freq` — returns the tick frequency after applying prescaler and clock divider

Features NOT Supported
======================

The following Zephyr counter API features are not implemented:

- 64-bit tick operations (``CONFIG_COUNTER_64BITS_TICKS``): :c:func:`counter_get_value_64`,
  :c:func:`counter_set_value_64`, :c:func:`counter_set_channel_alarm` with 64-bit ticks,
  :c:func:`counter_set_top_value` with 64-bit ticks
- 64-bit frequency (:c:func:`counter_get_frequency_64`, ``CONFIG_COUNTER_64BITS_FREQ``)

The following silicon capabilities have no corresponding Zephyr counter API and are therefore not
accessible through this driver:

- Input capture (edge timestamping via CC registers in capture mode)
- QEI / quadrature encoder input (TIMG8 instances only; no Zephyr qdec driver is available for this
  platform)
- Cross-trigger: hardware-to-hardware timer chains and ADC/DMA trigger output
- Event fabric publish/subscribe (ZERO, LOAD, CC events to the MSPM0 event bus)
- Repeat counter (trigger every N counts without CPU involvement)

Enabling the Driver
===================

Enable the counter child node of the target GPTIMER instance in the board devicetree or an
application overlay. ``CONFIG_COUNTER_MSPM0_TIMER`` is selected automatically when an enabled
``ti,mspm0-timer-counter`` node is present.

.. code-block:: devicetree

   &counterg0 {
           status = "okay";
   };

   / {
           aliases {
                   counter = &counterg0;
           };
   };

.. code-block:: kconfig

   CONFIG_COUNTER=y

The ``counter`` alias is required by the standard counter alarm sample
(``samples/drivers/counter/alarm``), which selects the device through ``DT_ALIAS(counter)`` when
``CONFIG_COUNTER_MSPM0_TIMER`` is set.

.. important::

   TIMA instances (``countera0_0``, ``countera0_1`` on MSPM33C; ``countera0`` on MSPM0G) share the
   same GPTIMER hardware block with the PWM driver (``ti,mspm0-timer-pwm``). Enabling both child
   nodes on the same parent TIMA instance simultaneously is not supported.

Devicetree Properties
=====================

Counter node (``compatible = "ti,mspm0-timer-counter"``)
---------------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Property
     - Description
   * - ``resolution``
     - Counter bit width: ``16`` or ``32``.  Sets the hardware
       ``max_top_value`` to ``UINT16_MAX`` (65535) or ``UINT32_MAX``
       (4294967295).  Must match the capability of the parent GPTIMER instance.
   * - ``channels``
     - Number of alarm channels to expose (1–4).  Each channel maps to one
       CC register (CC0–CC3).  Hardware maximum is 4.

Parent timer node (``compatible = "ti,mspm0-timer"``)
------------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Property
     - Description
   * - ``ti,clk-prescaler``
     - TIMCLK prescaler.  Range 0–255; the clock is divided by
       ``prescaler + 1`` (÷1 to ÷256).  Not available on TIMG12 instances,
       which have no hardware prescaler.
   * - ``ti,clk-div``
     - Post-prescaler clock divider.  Accepted values: 1–8.

The counter tick frequency is:

.. code-block:: text

   f_counter = f_source / ((ti,clk-prescaler + 1) × ti,clk-div)

Usage
=====

Free-Running Counter
--------------------

With no top value set the counter wraps at the hardware maximum.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/counter.h>

   const struct device *cnt = DEVICE_DT_GET(DT_ALIAS(counter));

   if (!device_is_ready(cnt)) {
           return;
   }

   counter_start(cnt);

   uint32_t ticks;
   counter_get_value(cnt, &ticks);

Periodic Counter with Top Callback
------------------------------------

.. code-block:: c

   static void period_cb(const struct device *dev, void *user_data) { }

   struct counter_top_cfg top_cfg = {
           .ticks    = counter_us_to_ticks(cnt, 1000U), /* 1 ms */
           .callback = period_cb,
           .flags    = 0,
   };

   counter_set_top_value(cnt, &top_cfg);
   counter_start(cnt);

.. note::

   :c:func:`counter_set_top_value` returns ``-EBUSY`` if any alarm channel has an active callback.
   Cancel all pending alarms before changing the top value.

   When ``COUNTER_TOP_CFG_DONT_RESET`` is set and the current counter value is already past the new
   top, the call returns ``-ETIME``. Pass ``COUNTER_TOP_CFG_RESET_WHEN_LATE`` to force a counter
   reset in that case.

Alarm Scheduling
-----------------

Each alarm is one-shot and is automatically disarmed after firing.

**Relative alarm** (fires ``ticks`` after the current counter value):

.. code-block:: c

   static void alarm_cb(const struct device *dev, uint8_t chan_id,
                        uint32_t ticks, void *user_data) { }

   struct counter_alarm_cfg alarm_cfg = {
           .ticks    = counter_us_to_ticks(cnt, 500U), /* 500 µs */
           .callback = alarm_cb,
           .flags    = 0,
   };

   counter_set_channel_alarm(cnt, 0, &alarm_cfg);

**Absolute alarm** with late-expiry (uses ``cnt`` and ``alarm_cb`` from above):

.. code-block:: c

   struct counter_alarm_cfg alarm_cfg = {
           /* 1000 raw ticks at f_counter — use counter_us_to_ticks() for time-based values */
           .ticks    = 1000U,
           .callback = alarm_cb,
           .flags    = COUNTER_ALARM_CFG_ABSOLUTE |
                       COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE,
   };

   counter_set_channel_alarm(cnt, 0, &alarm_cfg);

Without :c:macro:`COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE`, a late absolute alarm returns ``-ETIME`` and
the callback is never invoked.

Guard Period
------------

The guard period prevents a relative alarm from being scheduled so close to the top wrap that the
counter may have advanced past the target before the CC register write takes effect.
:c:func:`counter_set_channel_alarm` returns ``-EINVAL`` for relative alarms whose tick value falls
within the guard period.

.. code-block:: c

   counter_set_guard_period(cnt, 200U, COUNTER_GUARD_PERIOD_LATE_TO_SET);

Samples
=======

- :ws-doc:`Counter Alarm Sample <zephyr/samples/drivers/counter/alarm/README.html>` (passes on
  ``lp_mspm0g3507``, ``lp_mspm0g3519``, ``lp_mspm0l2228``, ``lp_mspm33c321a``, and ``lp_am13e230``)

Tests
=====

- ``tests/drivers/counter/counter_basic_api`` — exercises the full mandatory and optional counter
  API: start/stop, get/set/reset value, alarm set/cancel, top value, guard period, absolute and
  relative modes, and late-alarm detection across all enabled ``ti,mspm0-timer-counter`` instances
