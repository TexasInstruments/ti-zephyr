.. ti-socs: am62l

.. _ti-zephyr-component-ehrpwm:

eHRPWM
######

Features Supported
******************

- PWM output on channel A (``0``) and B (``1``) of each instance via :c:func:`pwm_set_cycles`,
  :c:func:`pwm_set_dt` and :c:func:`pwm_set_pulse_dt`
- Clock rate query with :c:func:`pwm_get_cycles_per_sec`; the driver selects a time-base prescaler
  automatically so the period fits the 16-bit period register
- Normal and inverted polarity (``PWM_POLARITY_NORMAL``, ``PWM_POLARITY_INVERTED``)
- Symmetric (up-down count) PWM with the ``symmetric`` devicetree property; the requested period is
  still the full output period
- TI flag ``TI_EHRPWM_USE_AQ_SET_ZRO_PRD`` (bit 8, from
  ``<zephyr/dt-bindings/pwm/ti_am3352_ehrpwm.h>``) drives the output from the counter zero and
  period events instead of the compare events
- Outputs turned off by setting a period of ``0``
- Experimental period and compare-match event callbacks via :c:func:`pwm_add_event_callback` with
  ``CONFIG_PWM_EVENT``: ``PWM_EVENT_TYPE_PERIOD`` and ``PWM_EVENT_TYPE_COMPARE_CAPTURE``

Features NOT Supported
**********************

- Hardware features the driver does not expose:

  - dead-band generation
  - trip-zone and fault handling
  - PWM chopper
  - time-base synchronization and phase control between modules

- Independent start and stop of the channels. A period of ``0`` stops the time base of the whole
  instance.
- Device power management and runtime power management.

Usage
*****

Enabling the Driver
===================

``CONFIG_PWM_TI_AM3352_EHRPWM`` is enabled by default once a ``ti,am3352-ehrpwm`` node has ``status
= "okay"``. The application must enable the PWM subsystem:

.. code-block:: kconfig

   CONFIG_PWM=y
   # Optional: event callbacks (experimental)
   CONFIG_PWM_EVENT=y

``CONFIG_PWM_EVENT`` needs the ``interrupts`` property on the instance.

The SoC devicetree defines each instance as follows (shown for ``main_epwm0``):

.. code-block:: devicetree

   main_epwm0: pwm@23000000 {
           compatible = "ti,am3352-ehrpwm";
           reg = <0x23000000 0x100>;
           tbclk = <&main_mmr_cfg3 0x69100 0>;
           #pwm-cells = <3>;
           power-domains = <&main_epwm0_pd>;
           clocks = <&scmi_clk 164>;
   };

Enable the instance, give it a ``default`` pinctrl state and reference its channels from a ``pwms``
property:

.. code-block:: devicetree

   #include <zephyr/dt-bindings/pwm/pwm.h>

   &main_epwm0 {
           pinctrl-0 = <&main_epwm0_a &main_epwm0_b>;
           pinctrl-names = "default";
           status = "okay";
   };

   / {
           epwm_out: epwm-out {
                   /* channel, period (ns), flags */
                   pwms = <&main_epwm0 0 1000000 PWM_POLARITY_NORMAL>;
           };
   };

PWM Output
==========

The ``channel`` cell of the ``pwms`` specifier selects the channel. :c:func:`pwm_set_dt` takes
period and pulse in nanoseconds; ``pulse`` must not exceed ``period``.

.. code-block:: c

   #include <errno.h>
   #include <zephyr/kernel.h>
   #include <zephyr/drivers/pwm.h>
   #include <zephyr/sys/printk.h>

   static const struct pwm_dt_spec epwm_out = PWM_DT_SPEC_GET(DT_NODELABEL(epwm_out));

   int main(void)
   {
           int ret;

           if (!pwm_is_ready_dt(&epwm_out)) {
                   printk("eHRPWM not ready\n");
                   return -ENODEV;
           }

           /* 1 kHz period (1000000 ns), 25% duty cycle (250000 ns) */
           ret = pwm_set_dt(&epwm_out, 1000000, 250000);
           if (ret < 0) {
                   printk("pwm_set_dt failed: %d\n", ret);
                   return ret;
           }

           return 0;
   }

For values in module clock cycles, call :c:func:`pwm_set_cycles` on
``DEVICE_DT_GET(DT_NODELABEL(main_epwm0))``, using channel ``1`` for output B.

Devicetree Bindings
*******************

- :ws-doc:`ti,am3352-ehrpwm <zephyr/build/dts/api/bindings/pwm/ti,am3352-ehrpwm.html>`

Subsystem Documentation
***********************

- :ws-doc:`PWM Zephyr Documentation <zephyr/hardware/peripherals/pwm.html>`
