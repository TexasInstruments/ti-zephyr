.. ti-socs: mspm0 mspm33

.. _ti-zephyr-component-iwdt:

Independent Watchdog
####################

.. important::

   The IWDT lives inside the Low-Frequency Subsystem (LFSS).  LFSS is
   present on **MSPM0L** (``lp_mspm0l2228``) and **MSPM33**
   (``lp_mspm33c321a``) devices.  **MSPM0G3507 and MSPM0G3519 do not
   have LFSS** and therefore do not have an IWDT.


Overview
========

The MSPM0L and MSPM33 Independent Watchdog Timer (IWDT) is exposed through
Zephyr's standard watchdog API (``<zephyr/drivers/watchdog.h>``).  The
driver implements all four mandatory operations: :c:func:`wdt_install_timeout`,
:c:func:`wdt_setup`, :c:func:`wdt_feed`, and :c:func:`wdt_disable`.

The IWDT lives inside the Low-Frequency Subsystem (LFSS) — the same always-on
IP block that hosts the RTC.  It is clocked by the LFOSC oscillator and
continues running through STOP and STANDBY power modes.  On timeout it
generates a Power-On Reset (POR) of the entire SoC.  There is no window
constraint and no interrupt.

Features Supported
==================

- Free-running watchdog: feed any time before the timeout expires.
- Eight selectable timeout periods (counter widths 2\ :sup:`6` to
  2\ :sup:`25` LFOSC ticks), selected automatically from ``window.max``.
- Configurable clock divider (÷1 to ÷8) applied before the counter.
- Pause when halted by debugger (``WDT_OPT_PAUSE_HALTED_BY_DBG``): counter
  halts when the CPU is stopped by a debug probe.
- Only ``WDT_FLAG_RESET_SOC`` is accepted.  The hardware always generates a
  POR; no other reset action is possible.
- Single channel (channel 0) only.
- After :c:func:`wdt_setup`, the IWDT register file is write-locked against
  further configuration changes.  Call :c:func:`wdt_disable` to unlock it
  before calling :c:func:`wdt_install_timeout` again with a new timeout.

Features NOT Supported
======================

- Window minimum (``window.min != 0`` returns ``-EINVAL``) — the IWDT has
  no closed-window hardware.
- Callback — the IWDT generates a POR with no interrupt path.  Passing a
  non-NULL callback to :c:func:`wdt_install_timeout` returns ``-ENOTSUP``.
- ``WDT_FLAG_RESET_CPU_CORE`` and ``WDT_FLAG_RESET_NONE`` — unsupported;
  only ``WDT_FLAG_RESET_SOC`` is accepted.
- ``WDT_OPT_PAUSE_IN_SLEEP`` — the IWDT is in the always-on LFSS domain
  and continues counting through all sleep states.
- Multi-stage timeouts (``CONFIG_WDT_MULTISTAGE``).

.. important::

   The IWDT register key mechanism is a hardware safety feature: writes to
   WDTEN, WDTCTL, and WDTLOCK require the correct key value in the upper
   byte.  Writing the **wrong key** to any of these registers immediately
   triggers a POR — it does not silently fail.  This is a hardware
   invariant, not a driver behaviour.

   After :c:func:`wdt_setup` the IWDT is locked against further
   configuration writes.  Call :c:func:`wdt_disable` to unlock it for
   re-configuration.

.. warning::

   The IWDT lives in the always-on LFSS domain and **survives software and
   debug resets**.  Once armed with :c:func:`wdt_setup`, it keeps counting
   and will reset the device on every expiry until the VBAT supply is
   power-cycled.  A debugger-initiated reset does **not** stop or clear the
   IWDT.

   This means: if the IWDT fires during development or automated testing,
   the board will keep rebooting on every subsequent debug session until the
   board is power-cycled at the VBAT rail.  For development and test
   workflows use the WWDT (``&wdt0``) as ``watchdog0`` instead; switch to
   IWDT only in production firmware.

Enabling the Driver
===================

Enable the IWDT node in the board devicetree (or an application overlay) and
the watchdog subsystem in ``prj.conf``.  ``CONFIG_WDT_MSPM0_IWDT`` is
selected automatically when an enabled ``ti,mspm0-iwdt`` node is present.

.. code-block:: devicetree

   &iwdt {
           status = "okay";
   };

   / {
           aliases {
                   watchdog0 = &iwdt;
           };
   };

.. code-block:: kconfig

   CONFIG_WATCHDOG=y

Devicetree Properties
=====================

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Property
     - Description
   * - ``clock-frequency``
     - Frequency of the IWDT clock source (LFOSC) in Hz.  Used by the driver
       to calculate the achievable timeout range and select the best PER and
       CLKDIV combination.  Required.

Usage
=====

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/watchdog.h>

   static const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

   struct wdt_timeout_cfg cfg = {
           .window = {
                   .min = 0U,    /* IWDT has no window minimum */
                   .max = 5000U, /* 5 s timeout */
           },
           .callback = NULL,         /* no callback — IWDT triggers POR */
           .flags    = WDT_FLAG_RESET_SOC,
   };

   int chan = wdt_install_timeout(wdt, &cfg);
   wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG);

   while (1) {
           k_sleep(K_MSEC(1000));
           wdt_feed(wdt, chan);
   }

.. note::

   The IWDT counts from LFOSC independently of the main system clock.
   The actual timeout period is rounded up to the nearest hardware-supported
   value (2\ :sup:`N` / (``clock-frequency`` / CLKDIV)).  The effective
   period may be larger than ``window.max`` by up to one step of the period
   lookup table.

Samples
=======

The standard in-tree watchdog sample (``samples/drivers/watchdog``) works
with the IWDT using the overlay shown in `Enabling the Driver`_.  The sample
uses ``WDT_FLAG_RESET_SOC`` and ``window.min = 0``, which are the only
values accepted by the IWDT.

Tests
=====

Automated regression testing of the IWDT is not supported.  Because the IWDT
survives debug resets and keeps asserting POR until the VBAT supply is
power-cycled, running it under a standard test harness would break all
subsequent tests on the same board.  Validate IWDT behaviour manually on
hardware with a controlled power-cycle between runs.

Zephyr Documentation
====================

- :ws-doc:`Watchdog Zephyr Documentation <zephyr/hardware/peripherals/watchdog.html>`
