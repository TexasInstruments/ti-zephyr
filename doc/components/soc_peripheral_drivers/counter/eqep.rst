EQEP Counter
************

.. important::

   The ``ti,am3352-eqep`` driver controls the Enhanced Quadrature Encoder Pulse (EQEP) modules of
   the device. An EQEP module connects to a linear or rotary incremental encoder and reports
   position, direction and speed. Applications see it as a 32-bit counter of the encoder quadrature
   clock.

Features Supported
==================

- Counter API (``<zephyr/drivers/counter.h>``): :c:func:`counter_start`, :c:func:`counter_stop`,
  :c:func:`counter_get_value`, :c:func:`counter_get_top_value`, :c:func:`counter_set_top_value`,
  :c:func:`counter_set_channel_alarm`, :c:func:`counter_cancel_channel_alarm`,
  :c:func:`counter_get_pending_int`, :c:func:`counter_get_freq`, :c:func:`counter_is_counting_up`
  and the guard period calls
- 32-bit position counter. The top value (maximum position, ``QPOSMAX``) is 0xFFFFFFFF after init.
  The counter wraps to 0 above the top value and to the top value below 0. The top callback is
  called on every overflow and underflow.
- Direction reported by the hardware: counting up when quadrature input A leads B (clockwise), down
  otherwise
- Four counting modes, selected with :c:func:`ti_eqep_configure_decoder` (``enum ti_eqep_src``):

  - quadrature count: clock and direction decoded from A and B, four counts per line (both edges of
    both inputs)
  - direction count: A is the clock, B is the direction
  - up count and down count: fixed direction, the counter measures the frequency of input A

- Decoder options: swapping A and B to reverse the counting direction, and counting only rising
  edges (``rising_edge_only``) instead of both
- Position counter reset modes (``enum ti_eqep_reset_mode``, through
  :c:func:`ti_eqep_configure_qep`): reset on every index event, on the maximum position, on the
  first index event only, or on the unit time-out (frequency measurement)
- Latch modes of the index, strobe and capture inputs (``struct ti_eqep_qep_cfg``): index latch on
  the rising edge, the falling edge or the index marker; strobe latch on the rising edge or
  direction dependent; capture latch on a counter read or on the unit time-out
- Four alarm channels (``enum ti_eqep_alarm_channel``). An alarm fires once and passes the latched
  value as the ticks argument of the callback:

  .. list-table::
     :header-rows: 1
     :widths: 30 70

     * - Channel
       - Event
     * - ``TI_EQEP_ALARM_CHAN_COMPARE`` (0)
       - The position counter reaches the compare value. Relative alarms are added to or
         subtracted from the current position, depending on the counting direction.
         ``COUNTER_ALARM_CFG_ABSOLUTE`` and ``COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE`` are honored.
         The callback receives the compare value.
     * - ``TI_EQEP_ALARM_CHAN_STROBE`` (1)
       - A strobe event. The callback receives the position latched on the strobe.
     * - ``TI_EQEP_ALARM_CHAN_INDEX`` (2)
       - An index event. The callback receives the position latched on the index.
     * - ``TI_EQEP_ALARM_CHAN_TIMEOUT`` (3)
       - The unit timer matches its period. The alarm ticks value is the period in
         module-clock cycles. The callback receives the position latched on the time-out.

- Edge capture unit for low-speed measurement (:c:func:`ti_eqep_configure_capture`): a capture timer
  with a clock prescaler of 1 to 128 and a unit position event prescaler of 1 to 2048 quadrature
  periods. :c:func:`ti_eqep_get_latched_capture_values` returns the latched capture timer and
  capture period, optionally scaled by the prescaler.
- Run-free operation under a debugger halt (the unit timer keeps running)
- Module clock rate from the clock controller, reported by :c:func:`counter_get_freq`
- Default pinctrl state applied at init
- ``ti_eqep_configure_qep()``, ``ti_eqep_configure_decoder()``, ``ti_eqep_configure_capture()`` and
  ``ti_eqep_get_latched_capture_values()`` are system calls usable from user threads

.. only:: am13

   - AM13 variant only (``CONFIG_COUNTER_TI_AM3352_EQEP_VARIANT_AM13``):

     - source selection for the QEPA, QEPB and QEPI signals (``ti,qepa-src``, ``ti,qepb-src``,
       ``ti,qepi-src``): the Input X-BAR (default), a comparator output (CMPSS0 to CMPSS3) or a PWM
       X-BAR output (PWMXBAR1 to PWMXBAR8)
     - GPIO routing to an EQEP input through the Input X-BAR, with ``mux-states``
     - peripheral power-up through the SYSCTL power enable register (``ti,pwren``)

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - the watchdog timer that detects a stalled quadrature clock
  - position counter initialization on an index or strobe event (software only: 0 when counting up,
    the top value when counting down)
  - software-forced interrupts

- Device power management is not implemented.

Usage
=====

Enabling the Driver
-------------------

``CONFIG_COUNTER`` has to be set by the application:

.. code-block:: kconfig

   CONFIG_COUNTER=y

``CONFIG_COUNTER_TI_AM3352_EQEP`` defaults to on when an enabled ``ti,am3352-eqep`` node exists. It
needs ``CONFIG_CLOCK_CONTROL`` and selects ``CONFIG_PINCTRL``.

.. only:: am13

   On the AM13 variant, ``CONFIG_COUNTER_TI_AM3352_EQEP_VARIANT_AM13`` defaults to on. It selects
   ``CONFIG_MUX`` and ``CONFIG_SYSCON`` for the X-BAR routing and power enable.

Devicetree Configuration
------------------------

.. only:: am62l

   The SoC devicetree defines each module as a node like this one:

   .. code-block:: devicetree

      main_eqep0: counter@23200000 {
              compatible = "ti,am3352-eqep";
              reg = <0x23200000 0x100>;
              power-domains = <&main_eqep0_pd>;
              clocks = <&scmi_clk 127>;
              interrupts = <GIC_SPI 162 IRQ_TYPE_EDGE IRQ_DEFAULT_PRIORITY>;
              interrupt-parent = <&gic>;
      };

   To use a module, set ``status = "okay"`` and apply the pin settings:

   .. code-block:: devicetree

      &main_eqep0 {
              pinctrl-0 = <&eqep0_a &eqep0_b &eqep0_i &eqep0_s>;
              pinctrl-names = "default";
              status = "okay";
      };

      eqep0_a: eqep0_a_default {
              pinmux = <K3_PINMUX(0x0cc, PIN_INPUT, MUX_MODE_4)>; /* (P23) EQEP0_A */
      };

.. only:: am13

   On the AM13 variant, each EQEP input is hard-wired to a fixed Input X-BAR channel. Route a GPIO
   to that channel and names the entry:

   .. code-block:: devicetree

      &eqep0 {
              pinctrl-0 = <&gpio_pb11>;
              pinctrl-names = "default";

              /* PB11 (GPIO43) -> INPUTXBAR17 (ch 16) -> EQEP0A */
              mux-states = <&xbar1 16 AM13E2_XBAR_IN_STATE(43, 1)>;
              mux-state-names = "qepa";
      };

   A signal can instead come from an on-chip comparator or the PWM X-BAR, with no pinctrl or mux
   entry:

   .. code-block:: devicetree

      &eqep0 {
              ti,qepa-src = <TI_EQEP_SRC_CMPSS1>;
      };

Measuring Position and Speed
----------------------------

Configure the decoder once, then read the counter. The decoder call also resets the position
counter:

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/counter.h>
   #include <zephyr/drivers/counter/ti_am3352_eqep.h>
   #include <zephyr/sys/printk.h>

   static const struct device *const eqep = DEVICE_DT_GET(DT_NODELABEL(main_eqep0));

   int main(void)
   {
           struct ti_eqep_dec_cfg dec = {
                   .source = TI_EQEP_SRC_QUADRATURE,
                   .rising_edge_only = false,
                   .swap_inputs = false,
           };
           uint32_t pos;

           if (!device_is_ready(eqep)) {
                   return -ENODEV;
           }

           ti_eqep_configure_decoder(eqep, &dec);
           counter_start(eqep);

           counter_get_value(eqep, &pos);
           printk("position %u\n", pos);

           return 0;
   }

Speed follows from two positions taken a known time apart. The index alarm reports the position at
the encoder index pulse, which marks one turn:

.. code-block:: c

   static void index_cb(const struct device *dev, uint8_t chan, uint32_t ticks,
                        void *user_data)
   {
           /* ticks is the position latched on the index event */
   }

   static int index_alarm_start(void)
   {
           struct counter_alarm_cfg alarm = {
                   .callback = index_cb,
           };

           return counter_set_channel_alarm(eqep, TI_EQEP_ALARM_CHAN_INDEX, &alarm);
   }

The alarm fires once. Set it again in the callback to measure every turn.

For low speeds, enable the capture unit and read the latched values on a unit position event or the
unit time-out:

.. code-block:: c

   struct ti_eqep_cap_cfg cap = {
           .enable = true,
           .clock_prescaler = TI_EQEP_CAP_CLK_DIV_64,
           .unit_position_prescaler = TI_EQEP_UNIT_POS_DIV_4,
   };
   uint32_t timer, period;

   ti_eqep_configure_capture(eqep, &cap);
   /* ... later, maybe in an ISR callback ... */
   ti_eqep_get_latched_capture_values(eqep, &timer, &period, true);

The capture unit must be disabled before its prescaler changes; :c:func:`ti_eqep_configure_capture`
does this itself.

Hardware Notes
--------------

Samples
-------

- ``samples/drivers/counter/eqep_position_speed``: measures position and speed of a simulated
  encoder. One eHRPWM instance generates the two quadrature phases and a GPIO generates the index
  pulse; jumper wires connect them to ``main_eqep0``.
- :ws-doc:`Counter alarm sample <zephyr/samples/drivers/counter/alarm/README.html>`

Devicetree Bindings
===================

- :ws-doc:`ti,am3352-eqep <zephyr/build/dts/api/bindings/counter/ti,am3352-eqep.html>`
