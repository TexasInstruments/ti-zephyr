OMAP I2C
********

Features Supported
==================

- Controller mode through the Zephyr I2C API (``<zephyr/drivers/i2c.h>``): multi-message
  :c:func:`i2c_transfer` and the ``i2c_write``, ``i2c_read`` and ``i2c_write_read`` helpers
- Standard (100 kHz) and fast (400 kHz) speeds, set from ``clock-frequency`` at start-up and
  changeable with :c:func:`i2c_configure`
- 7-bit addressing
- ``I2C_MSG_STOP`` honored per message
- Bus recovery with :c:func:`i2c_recover_bus` when ``CONFIG_I2C_OMAP_BUS_RECOVERY`` is enabled; also
  tried automatically when the bus stays busy before a transfer

Features NOT Supported
======================

.. note::

   Interrupt-driven transfers, I2C target mode and RTIO are being added to the upstream Zephyr
   driver in `zephyrproject-rtos/zephyr#121477
   <https://github.com/zephyrproject-rtos/zephyr/pull/121477>`_ (``CONFIG_I2C_OMAP_INTERRUPT``,
   ``CONFIG_I2C_TARGET`` and ``CONFIG_I2C_RTIO``).

- Hardware features the driver does not expose:

  - target mode, general call and the additional own addresses
  - 10-bit addressing
  - high-speed mode (3.4 Mbps)
  - interrupt-driven operation; all transfers are polled
  - Auto Idle, idle request handshake and wake-up

- Device power management.

Usage
=====

Enabling the Driver
-------------------

``CONFIG_I2C_OMAP`` is enabled by default once an enabled ``ti,omap-i2c`` node is present, and
selects ``PINCTRL``. The application must set ``CONFIG_I2C``.

.. code-block:: kconfig

   CONFIG_I2C=y

Bus recovery is optional (it selects ``CONFIG_I2C_BITBANG``):

.. code-block:: kconfig

   CONFIG_I2C_OMAP_BUS_RECOVERY=y

The SoC devicetree defines each controller as follows (shown for ``main_i2c0``):

.. code-block:: devicetree

   main_i2c0: i2c@20000000 {
           compatible = "ti,omap-i2c";
           reg = <0x20000000 0x100>;
           clock-frequency = <100000>;
           clocks = <&scmi_clk 246>;
           power-domains = <&main_i2c0_pd>;
           #address-cells = <1>;
           #size-cells = <0>;
   };

Enable the controller in the board devicetree or an overlay. The binding requires the pin control
properties.

.. code-block:: devicetree

   &main_i2c0 {
           pinctrl-0 = <&main_i2c0_scl &main_i2c0_sda>;
           pinctrl-names = "default";
           clock-frequency = <I2C_BITRATE_FAST>;
           status = "okay";
   };

Reading a Register
------------------

.. code-block:: c

   #include <errno.h>
   #include <zephyr/device.h>
   #include <zephyr/drivers/i2c.h>

   #define TARGET_ADDR 0x40U /* 7-bit address of the target; placeholder */

   static const struct device *const i2c_dev = DEVICE_DT_GET(DT_NODELABEL(main_i2c0));

   int main(void)
   {
           uint8_t reg = 0x00;
           uint8_t val;
           int ret;

           if (!device_is_ready(i2c_dev)) {
                   return -ENODEV;
           }

           ret = i2c_configure(i2c_dev, I2C_MODE_CONTROLLER |
                                        I2C_SPEED_SET(I2C_SPEED_FAST));
           if (ret < 0) {
                   return ret;
           }

           /* Write the register pointer, then read one byte back. */
           return i2c_write_read(i2c_dev, TARGET_ADDR, &reg, 1, &val, 1);
   }

Samples
-------

- :ws-doc:`EEPROM sample <zephyr/samples/drivers/eeprom/README.html>`

Devicetree Bindings
===================

- :ws-doc:`ti,omap-i2c <zephyr/build/dts/api/bindings/i2c/ti,omap-i2c.html>`
