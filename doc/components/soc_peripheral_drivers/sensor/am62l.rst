VTM
***

Features Supported
==================

- Die temperature sensors of the Voltage and Thermal Manager (``ti,vtm``) through the Zephyr sensor
  API (``<zephyr/drivers/sensor.h>``): :c:func:`sensor_sample_fetch` and
  :c:func:`sensor_channel_get` on ``SENSOR_CHAN_DIE_TEMP``. The value is returned in degrees
  Celsius, with the fractional part in ``val2`` (micro degrees)
- One sensor device per child node of the VTM node, selected by ``reg`` (0 to 7). The AM62L has a
  single sensor, which is next to the DDR and the A53 cluster
- Continuous conversion, started when the sensor device is initialized
- Three temperature thresholds per sensor with interrupts, through :c:func:`sensor_attr_set`,
  :c:func:`sensor_attr_get` and :c:func:`sensor_trigger_set` using the attributes and trigger types
  of ``<zephyr/drivers/sensor/ti_vtm.h>``:

  - ``TI_VTM_TRIG_TH0`` fires when the temperature falls below the cold threshold
  - ``TI_VTM_TRIG_TH1`` fires when the temperature rises above the hot threshold
  - ``TI_VTM_TRIG_TH2`` fires when the temperature rises above the critical threshold

Features NOT Supported
======================

- Device power management.

Usage
=====

Enabling the Driver
-------------------

The application must set ``CONFIG_SENSOR``. ``CONFIG_TI_VTM`` is enabled by default once an enabled
``ti,vtm`` node is present.

.. code-block:: kconfig

   CONFIG_SENSOR=y

.. code-block:: devicetree

   wkup_vtm0: vtm@b00000 {
           compatible = "ti,vtm";
           reg = <0xb00000 0x1000>, <0xb01000 0x1000>;
           reg-names = "cfg1", "cfg2";
           power-domains = <&wkup_vtm0_pd>;
           interrupts = <GIC_SPI 23 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>,
                        <GIC_SPI 24 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>,
                        <GIC_SPI 25 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-names = "th1", "th2", "th0";
           #address-cells = <1>;
           #size-cells = <0>;
           status = "disabled";
   };

Enable the VTM, set the two out of range limits in millidegrees Celsius and add a child node for
each sensor. The ``ti,voltage-domain`` property is the voltage domain whose interrupt line the
sensor reports through.

.. code-block:: devicetree

   &wkup_vtm0 {
           ti,max-reset-mc = <120000>;
           ti,max-recovery-mc = <105000>;
           status = "okay";

           sensor0: sensor@0 {
                   reg = <0>;
                   ti,voltage-domain = <0>;
           };
   };

The driver refuses to start if the devicetree lists more sensors than the hardware reports, or if
one of the limits is outside the conversion table.

Reading the Temperature
-----------------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/sensor.h>
   #include <zephyr/sys/printk.h>

   static const struct device *const vtm = DEVICE_DT_GET(DT_NODELABEL(sensor0));

   int main(void)
   {
           struct sensor_value temp;
           int ret;

           if (!device_is_ready(vtm)) {
                   return -ENODEV;
           }

           ret = sensor_sample_fetch(vtm);
           if (ret < 0) {
                   return ret;
           }

           ret = sensor_channel_get(vtm, SENSOR_CHAN_DIE_TEMP, &temp);
           if (ret < 0) {
                   return ret;
           }

           printk("Die temperature: %.2f C\n", sensor_value_to_double(&temp));

           return 0;
   }

Thresholds and Triggers
-----------------------

.. code-block:: c

   #include <zephyr/drivers/sensor.h>
   #include <zephyr/drivers/sensor/ti_vtm.h>

   static const struct sensor_trigger hot_trig = {
           .type = TI_VTM_TRIG_TH1,
           .chan = SENSOR_CHAN_DIE_TEMP,
   };

   static void hot_handler(const struct device *dev, const struct sensor_trigger *trig)
   {
           /* The temperature went above the hot threshold */
   }

   int arm_hot_threshold(const struct device *vtm)
   {
           /* 85.0 degrees Celsius */
           struct sensor_value limit = {.val1 = 85, .val2 = 0};
           int ret;

           ret = sensor_attr_set(vtm, SENSOR_CHAN_DIE_TEMP, TI_VTM_ATTR_TH1_THRESH, &limit);
           if (ret < 0) {
                   return ret;
           }

           return sensor_trigger_set(vtm, &hot_trig, hot_handler);
   }

Passing ``NULL`` as the handler disables the trigger. The thresholds are ``TI_VTM_ATTR_TH0_THRESH``
(cold), ``TI_VTM_ATTR_TH1_THRESH`` (hot) and ``TI_VTM_ATTR_TH2_THRESH`` (critical), matching the
three trigger types.

Samples
-------

- ``samples/sensor/ti_vtm``, which reads the temperature and uses the three threshold triggers.

Devicetree Bindings
===================

- :ws-doc:`ti,vtm <zephyr/build/dts/api/bindings/sensor/ti,vtm.html>`
