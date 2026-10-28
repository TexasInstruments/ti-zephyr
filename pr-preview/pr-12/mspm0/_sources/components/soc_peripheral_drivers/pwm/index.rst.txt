.. ti-socs: mspm0 mspm33

.. _ti-zephyr-component-pwm:

PWM
###


Overview
========

The MSPM0 and MSPM33 General-Purpose Timer (GPTIMER) peripheral — both the
advanced-control timer (TIMA) and general-purpose timer (TIMG) variants — is
exposed through Zephyr's standard PWM API (``<zephyr/drivers/pwm.h>``).  A
``ti,mspm0-timer-pwm`` child node under the parent ``ti,mspm0-timer`` instance
binds to this driver.  The same node can be configured either as a PWM output
or as a PWM capture input; the presence or absence of the ``ti,cc-mode``
property determines which function is active.

The driver implements :c:func:`pwm_set_cycles` and
:c:func:`pwm_get_cycles_per_sec`.  PWM capture
(:c:func:`pwm_configure_capture`, :c:func:`pwm_enable_capture`,
:c:func:`pwm_disable_capture`) is conditionally compiled under
``CONFIG_PWM_CAPTURE``.

Features Supported
==================

PWM Output
----------

- Three output alignment modes selectable per node in devicetree:

  - ``EDGE_ALIGN`` (default) — edge-aligned, down-counting.  Counter counts
    from ``LOAD`` to 0.  Output goes HIGH when the counter reaches the CC
    compare value (on the way down) and LOW at the LOAD reload event.
    Active-high pulse width equals ``pulse_cycles``.
  - ``EDGE_ALIGN_UP`` — edge-aligned, up-counting.  Active-high pulse:
    output goes high at ZERO, low at CC compare.
  - ``CENTER_ALIGN`` — center-aligned, up-down counting.  Pulse is
    symmetric around the counter peak.  The driver programs
    ``LOAD = period_cycles / 2`` automatically; the application passes the
    full period to :c:func:`pwm_set_cycles` unchanged.

- Normal and inverted polarity (``PWM_POLARITY_NORMAL``,
  ``PWM_POLARITY_INVERTED``).  Inverted mode subtracts the pulse from the
  period before writing the CC register.
- Glitch-free 0 % and 100 % duty cycle: the driver selects a fixed pin
  action instead of a compare-based action at the boundary values, so the
  output holds the correct level without a spurious pulse.
- Multi-channel output: multiple CC channels on the same GPTIMER instance
  can be driven independently by listing them all in ``ti,cc-index``.  In
  ``CENTER_ALIGN`` mode all channels share ``LOAD``; the driver recomputes
  every channel's CC value whenever the period changes.
- Configurable clock prescaler (÷1 to ÷256) and clock divider (÷1 to ÷8)
  on the parent timer node.  TIMG12 instances have no hardware prescaler.

PWM Capture (``CONFIG_PWM_CAPTURE``)
--------------------------------------

- Only **channel 0** is supported.  Passing any other channel to
  :c:func:`pwm_configure_capture` or :c:func:`pwm_enable_capture` returns
  ``-EINVAL``.

- Two hardware capture modes selectable via ``ti,cc-mode`` in devicetree:

  - ``PULSE_WIDTH`` — measures period and/or pulse width.  Two adjacent CC
    channels are used (the index in ``ti,cc-index`` for the falling edge and
    its hardware pair ``index ^ 1`` for the rising edge).  The first
    measurement after :c:func:`pwm_enable_capture` is discarded to establish
    a clean edge baseline.  Supports ``PWM_CAPTURE_TYPE_PERIOD``,
    ``PWM_CAPTURE_TYPE_PULSE``, and ``PWM_CAPTURE_TYPE_BOTH``.
  - ``EDGE_TIME`` — captures the raw timestamp of each successive edge on a
    single CC channel.  Supports ``PWM_CAPTURE_TYPE_PERIOD``,
    ``PWM_CAPTURE_TYPE_PULSE``, and ``PWM_CAPTURE_TYPE_BOTH``.

- Single-shot capture (``PWM_CAPTURE_MODE_SINGLE``) — stops the counter and
  disarms interrupts after the first valid measurement.
- Continuous capture (``PWM_CAPTURE_MODE_CONTINUOUS``) — delivers
  measurements to the callback on every cycle; counter ZERO events are
  treated as normal counter wraps, not errors.
- Inverted polarity in ``PULSE_WIDTH`` mode
  (``PWM_POLARITY_INVERTED``): swaps the falling-edge and rising-edge CC
  channel assignments so the driver measures the low-active pulse width
  instead of the high-active pulse width.

Features NOT Supported
======================

The following silicon capabilities have no Zephyr PWM API mapping.
Items marked **(TIMA only)** are absent on TIMG instances:

- Complementary output pairs with dead-band insertion **(TIMA only)**
- Asymmetric PWM **(TIMA only)** — phase-shifted center-aligned outputs
  across two synchronized TIMA instances
- Hardware fault handler **(TIMA only)** — drives outputs to a safe state
  when an internal comparator or external fault pin asserts
- QEI / quadrature encoder input (TIMG8 instances only; no Zephyr qdec
  driver is available for this platform)
- Cross-trigger: hardware timer-to-timer and timer-to-ADC/DMA chaining
- Event fabric publish/subscribe (ZERO, LOAD, CC events to the MSPM0 event bus)
- Repeat counter — interrupt only after every N counter cycles

Enabling the Driver
===================

Enable the PWM child node of the target GPTIMER instance.
``CONFIG_PWM_MSPM0`` and ``CONFIG_PINCTRL`` are selected automatically when
an enabled ``ti,mspm0-timer-pwm`` node is present.

.. code-block:: devicetree

   &pwma0 {
           status = "okay";
           /* pwma0_default is pre-defined in the SoC pinctrl DTSI.
            * For a new board, define a pinctrl state under &pinctrl
            * (see :ref:`ti-zephyr-component-pinctrl`). */
           pinctrl-0 = <&pwma0_default>;
           pinctrl-names = "default";
   };

   / {
           aliases {
                   pwm-out = &pwma0;
           };
   };

.. code-block:: kconfig

   CONFIG_PWM=y

For capture, also enable ``CONFIG_PWM_CAPTURE``:

.. code-block:: kconfig

   CONFIG_PWM=y
   CONFIG_PWM_CAPTURE=y

.. important::

   TIMA instances share their GPTIMER hardware block with the counter driver
   (``ti,mspm0-timer-counter``).  Enabling both child nodes on the same
   parent TIMA instance simultaneously is not supported.

   In ``PULSE_WIDTH`` capture mode the driver uses two adjacent CC channels.
   Set ``ti,cc-index`` to the **even** channel of the pair (e.g. ``<0>``);
   the driver claims channel ``index`` for the falling edge and channel
   ``index ^ 1`` for the rising edge automatically.

Devicetree Properties
=====================

PWM node (``compatible = "ti,mspm0-timer-pwm"``)
-------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Property
     - Description
   * - ``ti,cc-index``
     - Array of CC channel indices this node drives (0–3, up to 4 entries).
       For output, each entry maps to one output pin.  For ``PULSE_WIDTH``
       capture, only the first entry is used; the paired channel
       (``index ^ 1``) is claimed automatically.
   * - ``ti,pwm-mode``
     - Output alignment: ``EDGE_ALIGN`` (default), ``EDGE_ALIGN_UP``, or
       ``CENTER_ALIGN``.  Ignored on capture nodes.
   * - ``ti,cc-mode``
     - Capture mode: ``EDGE_TIME`` or ``PULSE_WIDTH``.  Presence of this
       property marks the node as a capture node; absence means PWM output.
   * - ``ti,period``
     - Initial period in TIMCLK cycles.  For ``CENTER_ALIGN``, pass the full
       period; the driver programs ``LOAD = ti,period / 2`` internally.
   * - ``pinctrl-0``, ``pinctrl-names``
     - Required.  Pin control configuration for the PWM output or capture
       input pin.

Parent timer node (``compatible = "ti,mspm0-timer"``)
------------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Property
     - Description
   * - ``ti,clk-prescaler``
     - TIMCLK prescaler, range 0–255 (÷1 to ÷256).  Not present on TIMG12
       instances, which have no hardware prescaler.
   * - ``ti,clk-div``
     - Post-prescaler clock divider, values 1–8.

The PWM tick frequency follows the same formula as the counter driver:

.. code-block:: text

   f_pwm = f_source / ((ti,clk-prescaler + 1) × ti,clk-div)

Optional Kconfig
================

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Symbol
     - Effect
   * - ``CONFIG_PWM_CAPTURE``
     - Enables PWM capture support (:c:func:`pwm_configure_capture`,
       :c:func:`pwm_enable_capture`, :c:func:`pwm_disable_capture`).
       Required when a ``ti,mspm0-timer-pwm`` node with ``ti,cc-mode`` is
       used.  Adds interrupt registration and ISR code for the capture path.

Usage
=====

Basic PWM Output
-----------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/pwm.h>

   static const struct pwm_dt_spec pwm_out =
           PWM_DT_SPEC_GET(DT_ALIAS(pwm_out));

   if (!pwm_is_ready_dt(&pwm_out)) {
           return;
   }

   /* 1 kHz, 50 % duty cycle, normal polarity */
   pwm_set_dt(&pwm_out, PWM_HZ(1000), PWM_HZ(1000) / 2);

   /* Inverted polarity — use pwm_set() to override the flags from the DT spec */
   pwm_set(pwm_out.dev, pwm_out.channel,
           PWM_HZ(1000), PWM_HZ(1000) / 2,
           PWM_POLARITY_INVERTED);

Center-Aligned Output
---------------------

Only a devicetree change is needed; the API call is identical.

.. code-block:: devicetree

   &pwma0 {
           ti,pwm-mode = "CENTER_ALIGN";
   };

.. code-block:: c

   /* Period and pulse are still full-cycle counts.
    * The driver halves the period for LOAD internally. */
   pwm_set_dt(&pwm_out, PWM_KHZ(10), PWM_KHZ(10) / 4); /* 25 % */

PWM Capture Usage
------------------

Configure a capture node in devicetree:

.. code-block:: devicetree

   &tima1 {
           status = "okay";

           pwma1: pwma1 {
                   compatible = "ti,mspm0-timer-pwm";
                   ti,cc-index = <0>;        /* uses CC0 (fall) + CC1 (rise) */
                   ti,cc-mode = "PULSE_WIDTH";
                   ti,period = <0xffff>;  /* max 16-bit top — signals longer than
                                          * 0xffff / f_pwm seconds will time out;
                                          * reduce ti,clk-div or ti,clk-prescaler
                                          * for slower signals */
                   pinctrl-0 = <&tima1_ccp0_pb4>;
                   pinctrl-names = "default";
                   status = "okay";
           };
   };

   / {
           aliases {
                   capture = &pwma1;
           };
   };

Single-shot capture:

.. code-block:: c

   #include <zephyr/drivers/pwm.h>

   static const struct device *cap = DEVICE_DT_GET(DT_ALIAS(capture));

   if (!device_is_ready(cap)) {
           return;
   }

   static void capture_cb(const struct device *dev, uint32_t channel,
                          uint32_t period_cycles, uint32_t pulse_cycles,
                          int status, void *user_data)
   {
           if (status != 0) {
                   /* -ERANGE: signal not seen before counter wrapped */
                   return;
           }
           uint64_t clk_hz;
           pwm_get_cycles_per_sec(dev, channel, &clk_hz);
           uint32_t period_us = (uint32_t)(period_cycles * 1000000ULL / clk_hz);
           uint32_t pulse_us  = (uint32_t)(pulse_cycles  * 1000000ULL / clk_hz);
   }

   pwm_configure_capture(cap, 0,
                         PWM_CAPTURE_TYPE_BOTH | PWM_CAPTURE_MODE_SINGLE,
                         capture_cb, NULL);
   pwm_enable_capture(cap, 0);

.. note::

   In ``PULSE_WIDTH`` mode the driver discards the first measurement to
   establish a clean edge baseline; the callback fires on the second complete
   cycle.  A ``-ERANGE`` status means the input signal period exceeded the
   counter top value (``ti,period``) before an edge arrived — increase
   ``ti,period`` or reduce the prescaler.

   ``ti,period`` sets the counter top for the capture timer.  A signal with
   a period longer than ``ti,period / f_pwm`` seconds will time out.

Samples
=======

In-tree
-------

- `Blinky PWM Sample <../../../../../zephyr/samples/basic/blinky_pwm/README.html>`_ —
  fades an LED using :c:func:`pwm_set_dt` on the ``pwm-led0`` alias.  Passes
  on ``lp_mspm0g3507`` and ``lp_mspm33c321a``.
- `PWM Capture Sample <../../../../../zephyr/samples/drivers/pwm/capture/README.html>`_ —
  measures period and pulse width of an input signal using ``PULSE_WIDTH``
  mode.  Board overlay for ``lp_mspm0g3507`` is included
  (``samples/drivers/pwm/capture/boards/lp_mspm0g3507.overlay``).

TI-SDK (``samples/drivers/pwm/``)
---------------------------------------

- **pwm_center_align** — demonstrates ``CENTER_ALIGN`` mode on
  ``lp_mspm0g3507``, ``lp_mspm0g3519``, ``lp_mspm0l2228``, and
  ``lp_mspm33c321a``.  The board overlays show how to switch an existing
  ``EDGE_ALIGN`` node to ``CENTER_ALIGN`` with a single devicetree property
  change and no application code modification.
- **pwm_capture_accuracy** — generates a reference signal on ``pwm-out`` and
  measures it on ``pwm-cap`` (a wire between the two pins is required).
  Reports jitter (in cycles), frequency accuracy in ppm, duty-cycle accuracy,
  and dynamic duty-change tracking across multiple test frequencies.  All
  test frequencies are derived at runtime from the capture timer clock, so
  the sample adapts to any board overlay without code changes.  Runs on
  ``lp_mspm0g3507``, ``lp_mspm0g3519``, ``lp_mspm0l2228``, and
  ``lp_mspm33c321a``.

Tests
=====

- ``tests/drivers/pwm/pwm_api`` — exercises :c:func:`pwm_set_cycles` and
  :c:func:`pwm_set` at 50 %, 100 %, and 0 % duty cycle.  Board overlays for
  ``lp_mspm0g3507``, ``lp_mspm0g3519``, ``lp_mspm0l2228``, and
  ``lp_mspm33c321a`` are included; each sets a ``pwm-test`` alias to the
  default TIMA PWM node.
- ``tests/drivers/pwm/pwm_loopback`` — hardware loopback test: drives a PWM
  output and captures it on a second channel to verify period and pulse width
  round-trip accuracy.  Board overlays for ``lp_mspm0g3507``,
  ``lp_mspm0g3519``, ``lp_mspm0l2228``, and ``lp_mspm33c321a`` are included.
  Requires a wire connecting the output pin to the capture pin on the board.

Zephyr Documentation
====================

- `PWM Zephyr Documentation <../../../../../zephyr/hardware/peripherals/pwm.html>`_
