.. ti-socs: mspm0 mspm33

.. _ti-zephyr-component-wwdt:

Window Watchdog
###############


Overview
========

The MSPM0 and MSPM33 Window Watchdog Timer (WWDT) is exposed through
Zephyr's standard watchdog API (``<zephyr/drivers/watchdog.h>``).  The
driver implements all four mandatory operations: :c:func:`wdt_install_timeout`,
:c:func:`wdt_setup`, :c:func:`wdt_feed`, and :c:func:`wdt_disable`.

The WWDT is a 25-bit counter clocked by LFCLK (32.768 kHz path) with a
programmable divider.  It operates in one of two modes selected by the
flags passed to :c:func:`wdt_install_timeout`:

- **Watchdog mode** (``WDT_FLAG_RESET_CPU_CORE`` or ``WDT_FLAG_RESET_SOC``)
  — resets the device if the application fails to call :c:func:`wdt_feed`
  within the configured open window.  An optional closed window rejects
  feeds that arrive too early.
- **Interval timer mode** (``WDT_FLAG_RESET_NONE``) — fires a periodic
  callback without resetting the device; the WWDT acts as a system interval
  timer.

Features Supported
==================

- **Window watchdog mode**: feed must arrive in the open window
  (``window.min`` ≤ elapsed time ≤ ``window.max``).  Early feed (elapsed
  time < ``window.min``) or late feed (elapsed > ``window.max``) both
  trigger a violation and reset.
- **Closed-window guard**: ``window.min > 0`` activates the closed-window
  period.  The hardware supports eight discrete fractions of the total
  timeout period as the closed-window boundary: 0 %, 12.5 %, 18.75 %,
  25 %, 50 %, 75 %, 81.25 %, 87.5 %.  The driver rounds up to the next
  available fraction when the requested ``window.min`` does not align
  exactly.  Setting ``window.min = 0`` disables the closed window, giving
  traditional watchdog behaviour.
- **Interval timer mode**: ``WDT_FLAG_RESET_NONE`` reconfigures the WWDT as
  a periodic interrupt source.  The callback fires on every expiry.
  :c:func:`wdt_feed` (which restarts the counter) may be called from inside
  the callback; the driver handles ISR-context feed safely.
- **Interval timer callback**: a non-NULL callback is only valid with
  ``WDT_FLAG_RESET_NONE``.  Setting a callback with ``WDT_FLAG_RESET_SOC``
  or ``WDT_FLAG_RESET_CPU_CORE`` returns ``-EINVAL``.
- **Two reset actions** (hardware-fixed per WWDT instance, selected in
  devicetree via ``ti,watchdog-reset-action``):

  - ``WDT_FLAG_RESET_CPU_CORE`` — BOOTRST: resets peripherals and CPU, then
    re-runs the Boot Configuration Routine (BCR).  Suited for catching
    deeper system faults such as corrupted trim values.
  - ``WDT_FLAG_RESET_SOC`` — SYSRST: resets peripherals and CPU without
    re-running the BCR.  Suited for recovering from software stalls.

  The flags passed to :c:func:`wdt_install_timeout` must match the
  ``ti,watchdog-reset-action`` value in the devicetree node.  A mismatch
  returns ``-EINVAL``.

- **Pause in sleep** (``WDT_OPT_PAUSE_IN_SLEEP``): counter halts when the
  CPU enters a low-power sleep mode.
- **Pause when halted by debugger** (``WDT_OPT_PAUSE_HALTED_BY_DBG``):
  counter halts when the CPU is stopped by a debug probe.
- Eight selectable timeout periods (counter widths 2\ :sup:`6` to
  2\ :sup:`25` ticks of the divided LFCLK).
- Configurable clock divider on the parent clock (÷1 to ÷8 via LFCLK clock
  control).
- Single channel (channel 0) only.

Features NOT Supported
======================

- Multi-stage timeouts (``CONFIG_WDT_MULTISTAGE``)
- Multiple channels — only channel 0
- Pre-reset callback in watchdog mode — WWDT violations route through the
  ESM safety block, not the NVIC.  A callback that fires before a hardware
  reset is architecturally impossible in watchdog mode.  Setting a non-NULL
  callback with ``WDT_FLAG_RESET_SOC`` or ``WDT_FLAG_RESET_CPU_CORE``
  returns ``-EINVAL``; use ``WDT_FLAG_RESET_NONE`` (interval timer mode)
  if a pre-expiry callback is required.

.. important::

   Once :c:func:`wdt_setup` starts the WWDT in **watchdog mode**, the
   hardware counter cannot be stopped.  :c:func:`wdt_disable` returns
   ``-EPERM`` in watchdog mode.  In **interval timer mode**,
   :c:func:`wdt_disable` stops the counter and allows re-configuration.

   WWDT violations route through the ESM safety block, not the NVIC.  A
   pre-reset callback before a watchdog-mode violation is architecturally
   impossible.  Only interval timer mode can deliver a callback.

Enabling the Driver
===================

Enable the WWDT node in the board devicetree (or an application overlay) and
the watchdog subsystem in ``prj.conf``.  ``CONFIG_WDT_MSPM0`` is selected
automatically when an enabled ``ti,mspm0-watchdog`` node is present.

.. code-block:: devicetree

   &wdt0 {
           status = "okay";
   };

   / {
           aliases {
                   watchdog0 = &wdt0;
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
   * - ``ti,watchdog-reset-action``
     - Reset type on violation.  ``0`` = BOOTRST (``WDT_FLAG_RESET_CPU_CORE``),
       ``1`` = SYSRST (``WDT_FLAG_RESET_SOC``).  Must match the flag passed to
       :c:func:`wdt_install_timeout`.
   * - ``closed-window``
     - Selects which hardware window register stores the closed-window boundary:
       ``0`` = WINDOW0 (default for most applications), ``1`` = WINDOW1.  The
       driver programs the selected register with the hardware fraction closest
       to the ``window.min`` value passed to :c:func:`wdt_install_timeout`.
       Set to ``1`` only if WINDOW0 must be preserved for another use.
   * - ``clocks``
     - Reference to the clock controller providing LFCLK.  Required.
   * - ``interrupts``
     - INTTIM interrupt line.  Required only for interval timer mode
       (``WDT_FLAG_RESET_NONE``).  Not used in window watchdog mode.

Usage
=====

Window Watchdog Mode
--------------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/watchdog.h>

   static const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

   struct wdt_timeout_cfg cfg = {
           .window = {
                   .min = 500U,  /* closed window: 500 ms — feed before = violation */
                   .max = 2000U, /* open window:   2000 ms — feed after  = violation */
           },
           .callback = NULL,
           .flags    = WDT_FLAG_RESET_SOC,
   };

   int chan = wdt_install_timeout(wdt, &cfg);
   wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG);

   /* Feed periodically inside the open window */
   while (1) {
           k_sleep(K_MSEC(1000));
           wdt_feed(wdt, chan);
   }

Setting ``window.min = 0`` disables the closed window, allowing a feed at
any time before ``window.max`` expires.

Interval Timer Mode
-------------------

.. code-block:: c

   static void wdt_cb(const struct device *dev, int channel_id)
   {
           /* Called on every expiry — restart not automatic */
           wdt_feed(dev, channel_id);
   }

   struct wdt_timeout_cfg cfg = {
           .window   = { .min = 0U, .max = 1000U },
           .callback = wdt_cb,
           .flags    = WDT_FLAG_RESET_NONE,
   };

   int chan = wdt_install_timeout(wdt, &cfg);
   wdt_setup(wdt, 0); /* no pause options — counter runs through debug halt */

   /* wdt_disable() is valid in interval mode and stops the counter */

Samples
=======

In-tree
-------

- :ws-doc:`Watchdog Sample <zephyr/samples/drivers/watchdog/README.html>` —
  basic feed loop using the ``watchdog0`` alias.  Board overlays for
  ``lp_mspm0g3507``, ``lp_mspm0g3519``, and ``lp_mspm0l2228`` are included
  (``samples/drivers/watchdog/boards/``).

TI-SDK (``samples/drivers/watchdog/``)
--------------------------------------------

- **watchdog_window** — demonstrates all three feed zones across two boots:
  five correct feeds, then a deliberate early feed (closed-window violation)
  that triggers a SYSRST.  Boot 2 reads the SYSCTL RSTCAUSE register to
  confirm the violation.  Runs on ``lp_mspm0g3507``, ``lp_mspm0g3519``,
  ``lp_mspm0l2228``, ``lp_mspm33c321a``, and ``lp_am13e230``.

Tests
=====

- ``tests/drivers/watchdog/wdt_basic_api`` — install timeout, setup, feed,
  and hardware reset verification.  Passes on ``lp_mspm0g3507``,
  ``lp_mspm0g3519``, ``lp_mspm0l2228``, ``lp_mspm33c321a``, and
  ``lp_am13e230``.  Board-specific overlays are provided for the first three;
  ``lp_mspm33c321a`` and ``lp_am13e230`` use their board defaults.
- ``tests/drivers/watchdog/wdt_basic_reset_none`` — interval timer mode
  (``WDT_FLAG_RESET_NONE``) with callback.  Passes on all five boards.
- ``tests/drivers/watchdog/wdt_error_cases`` — verifies that invalid
  arguments and out-of-range configurations return correct error codes.
  Passes on all five boards.
- ``samples/subsys/task_wdt`` — Zephyr software task watchdog using the
  hardware WWDT as a backend.  Passes on ``lp_mspm0l2228``,
  ``lp_mspm33c321a``, and ``lp_am13e230``.

Zephyr Documentation
====================

- :ws-doc:`Watchdog Zephyr Documentation <zephyr/hardware/peripherals/watchdog.html>`
