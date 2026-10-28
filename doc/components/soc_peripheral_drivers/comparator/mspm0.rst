Features Supported
******************

- Zephyr comparator API (``<zephyr/drivers/comparator.h>``): :c:func:`comparator_get_output`,
  :c:func:`comparator_set_trigger`, :c:func:`comparator_set_trigger_callback` and
  :c:func:`comparator_trigger_is_pending`
- Rising, falling and both-edge triggers on the output. The callback runs in interrupt context.
- Positive and negative input channel (0 to 7) selected from devicetree
- Fast and ultra-low-power (``ulp``) operating modes
- Programmable hysteresis of none, 10, 20 or 30 mV
- Analog output filter with a delay of 70, 500, 1200 or 2700 ns (fast mode only)
- Internal 8-bit reference DAC on the positive or negative terminal, fed from VDDA, the internal
  VREF or an external VREF
- Two DAC codes (``DACCODE0`` and ``DACCODE1``), either switched by the comparator output
  (hysteresis without external parts) or selected statically
- Internal VREF: if the node has a ``vref`` phandle and an ``int_vref`` source is chosen, the driver
  enables the VREF regulator at init

.. only:: mspm0

   - MSPM0G only (``CONFIG_COMPARATOR_MSPM0_WINDOW_MODE``, selected automatically): window
     comparator mode. Two comparators (main plus ``ti,window-companion``) check an input against a
     lower and an upper threshold

.. only:: mspm33

   - MSPM33C only (``CONFIG_COMPARATOR_MSPM0_ANALOG_COMPARISON``, selected automatically): analog
     comparison sequencer. It steps through four VMON inputs, each with its own 8-bit DAC code, and
     can wake the device from STOP mode. It runs with blanking and comparison periods, or is locked
     to one input (window input)

- Several instances can share an interrupt line (``CONFIG_SHARED_INTERRUPTS`` is selected)

Features NOT Supported
**********************

- Hardware features the driver does not expose:

  - sampled output mode (``REFMODE`` is always static)
  - output blanking from a timer source
  - input terminal exchange (``EXCH``) and the input ``SHORT`` switch
  - output polarity inversion (polarity is always non-inverted)
  - the comparator event publisher and subscriber ports

Usage
*****

Enabling the Driver
===================

Set ``status = "okay"`` on the comparator node in the board devicetree or an application overlay,
route the output pin through ``pinctrl-0`` if needed, and select the input channels and reference.
``CONFIG_COMPARATOR_MSPM0`` defaults to on when an enabled ``ti,mspm0-comparator`` node exists, so
``prj.conf`` only needs the subsystem:

.. code-block:: kconfig

   CONFIG_COMPARATOR=y

The loopback test overlay uses positive input channel 2, a 128-code DAC level derived from VDDA on
the negative terminal, output on PA7 and a 2700 ns filter:

.. code-block:: devicetree

   &comp0 {
           status = "okay";
           positive-inputs = <2>;
           pinctrl-0 = <&comp0_out_pa7>;
           pinctrl-names = "default";
           ti,mode = "fast";
           ti,reference-source = "vdda_dac";
           ti,reference-terminal = "neg";
           ti,reference-dac-code0 = <128>;
           ti,filter-enable;
           ti,filter-delay = <2700>;
   };

When ``ti,reference-source`` is not ``none``, channel selection is turned off for the terminal that
receives the reference, so only the other terminal needs an input pin.

For the internal VREF (``int_vref`` or ``int_vref_dac``), enable the ``vref0`` regulator, point the
comparator at it and set ``CONFIG_REGULATOR=y``:

.. code-block:: devicetree

   &comp0 {
           vref = <&vref0>;
           ti,reference-source = "int_vref_dac";
   };

Reading the Output and Using Events
===================================

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/comparator.h>
   #include <zephyr/kernel.h>

   static const struct device *const comp = DEVICE_DT_GET(DT_NODELABEL(comp0));

   static void comp_cb(const struct device *dev, void *user_data)
   {
           printk("comparator output changed: %d\n", comparator_get_output(dev));
   }

   int main(void)
   {
           if (!device_is_ready(comp)) {
                   return -1;
           }

           comparator_set_trigger_callback(comp, comp_cb, NULL);
           comparator_set_trigger(comp, COMPARATOR_TRIGGER_BOTH_EDGES);

           while (1) {
                   printk("output = %d\n", comparator_get_output(comp));
                   k_msleep(1000);
           }
   }

Notes:

- :c:func:`comparator_get_output` returns 1 when the positive terminal is higher than the negative
  terminal, 0 otherwise.
- :c:func:`comparator_set_trigger` with ``COMPARATOR_TRIGGER_NONE`` masks the interrupt and clears
  any pending event.
- Without a callback, poll :c:func:`comparator_trigger_is_pending`; it returns 1 once per event.

Devicetree Bindings
*******************

- :ws-doc:`ti,mspm0-comparator <zephyr/build/dts/api/bindings/comparator/ti,mspm0-comparator.html>`
