.. ti-socs: am62l

.. _ti-zephyr-component-ecap:

eCAP
####

Features Supported
******************

- APWM-mode PWM output through the Zephyr PWM API (``<zephyr/drivers/pwm.h>``):
  :c:func:`pwm_set_cycles` and :c:func:`pwm_set`
- Clock rate query with :c:func:`pwm_get_cycles_per_sec`
- Normal and inverted polarity (``PWM_POLARITY_NORMAL``, ``PWM_POLARITY_INVERTED``)
- With ``CONFIG_PWM_CAPTURE``: :c:func:`pwm_configure_capture`, :c:func:`pwm_enable_capture` and
  :c:func:`pwm_disable_capture`, in single-shot or continuous mode
- Capture of period, pulse width or both (``PWM_CAPTURE_TYPE_PERIOD``, ``PWM_CAPTURE_TYPE_PULSE``),
  on either input polarity
- Capture callback receives ``status = -ERANGE`` on counter overflow

Features NOT Supported
**********************

- Hardware features the driver does not expose:

  - absolute time-stamp mode; only delta (difference) mode is used
  - one-shot capture of one to three events, or interrupts on capture events other than the fourth
  - synchronization input and output, and counter phase control

- Stopping the PWM output; once started it cannot be turned off.
- Capture and PWM output at the same time on one instance. :c:func:`pwm_set_cycles` is not rejected
  while a capture is enabled and disturbs the capture.
- Device power management and runtime power management.

Usage
*****

Enabling the Driver
===================

``CONFIG_PWM_TI_AM3352_ECAP`` is enabled by default once a ``ti,am3352-ecap`` node has ``status =
"okay"``. The application must enable the PWM subsystem; ``CONFIG_PWM_CAPTURE`` is needed only for
the capture API:

.. code-block:: kconfig

   CONFIG_PWM=y
   CONFIG_PWM_CAPTURE=y

The SoC devicetree defines each instance as follows (shown for ``main_ecap0``):

.. code-block:: devicetree

   main_ecap0: pwm@23100000 {
           compatible = "ti,am3352-ecap";
           reg = <0x23100000 0x100>;
           #pwm-cells = <3>;
           power-domains = <&main_ecap0_pd>;
           clocks = <&scmi_clk 99>;
   };

The instance needs a ``default`` pinctrl state; initialization fails if it cannot be applied:

.. code-block:: devicetree

   &main_ecap0 {
           pinctrl-0 = <&main_ecap0_in>;
           pinctrl-names = "default";
           status = "okay";
   };

PWM Output
==========

:c:func:`pwm_set` converts nanoseconds to cycles with :c:func:`pwm_get_cycles_per_sec`. The API
rejects a pulse wider than the period with ``-EINVAL``.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/devicetree.h>
   #include <zephyr/drivers/pwm.h>

   static const struct device *const ecap0 = DEVICE_DT_GET(DT_NODELABEL(main_ecap0));

   int main(void)
   {
           if (!device_is_ready(ecap0)) {
                   return -ENODEV;
           }

           /* 1 kHz period, 25 % duty cycle, normal polarity */
           return pwm_set(ecap0, 0, PWM_USEC(1000), PWM_USEC(250),
                          PWM_POLARITY_NORMAL);
   }

Capture
=======

The callback receives the period and pulse width in timer cycles:

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/devicetree.h>
   #include <zephyr/drivers/pwm.h>

   static const struct device *const ecap0 = DEVICE_DT_GET(DT_NODELABEL(main_ecap0));
   static volatile uint32_t last_period;
   static volatile uint32_t last_pulse;

   static void capture_cb(const struct device *dev, uint32_t channel,
                          uint32_t period_cycles, uint32_t pulse_cycles,
                          int status, void *user_data)
   {
           if (status == 0) {
                   last_period = period_cycles;
                   last_pulse = pulse_cycles;
           }
   }

   int main(void)
   {
           int ret;

           if (!device_is_ready(ecap0)) {
                   return -ENODEV;
           }

           ret = pwm_configure_capture(ecap0, 0,
                                       PWM_CAPTURE_TYPE_BOTH |
                                       PWM_CAPTURE_MODE_CONTINUOUS |
                                       PWM_POLARITY_NORMAL,
                                       capture_cb, NULL);
           if (ret < 0) {
                   return ret;
           }

           return pwm_enable_capture(ecap0, 0);
   }

- The callback runs after four captured edges. ``PWM_POLARITY_NORMAL`` starts on a rising edge,
  ``PWM_POLARITY_INVERTED`` on a falling edge.
- A value not requested through ``PWM_CAPTURE_TYPE_PERIOD`` or ``PWM_CAPTURE_TYPE_PULSE`` is
  reported as ``0``.
- On counter overflow the callback gets ``status = -ERANGE`` and zero values.

Samples
=======

- :ws-doc:`PWM capture sample <zephyr/samples/drivers/pwm/capture/README.html>`
  (``samples/drivers/pwm/capture``)

Devicetree Bindings
*******************

- :ws-doc:`ti,am3352-ecap <zephyr/build/dts/api/bindings/pwm/ti,am3352-ecap.html>`

Subsystem Documentation
***********************

- :ws-doc:`PWM Zephyr Documentation <zephyr/hardware/peripherals/pwm.html>`
