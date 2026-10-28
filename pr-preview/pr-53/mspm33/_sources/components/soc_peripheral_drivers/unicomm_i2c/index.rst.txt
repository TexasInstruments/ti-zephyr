.. ti-socs: am13 mspm33

.. _ti-zephyr-component-i2c-unicomm:

UNICOMM I2C
###########

Features Supported
******************

- I2C Controller and target modes mode (Standard, Fast, and Fast+ modes up to 1 MHz)
- I2C Target mode (7-bit and 10-bit addressing) when ``CONFIG_I2C_TARGET`` is enabled
- Advanced mode with automatic STOP generation (when ``unicomm-advanced-i2c`` is set)
- Clock stretching support (Controller and Target)
- Hardware FIFO with configurable TX/RX thresholds
- 7-bit and 10-bit addressing modes
- Interrupt-driven operation

Features NOT Supported
**********************

- High-speed (3.4 MHz) and Ultra-fast (5MHz) modes
- SMBus-specific features (PEC, ARP, etc.)

Usage
*****

The driver implements Zephyr's standard I2C API (``<zephyr/drivers/i2c.h>``).
Applications use :c:func:`i2c_transfer` for controller transactions and
:c:func:`i2c_target_register` for target operation.

The driver has two operating modes (transparent to the user) for controller transfers:

- **Advanced mode** (``unicomm-advanced-i2c``): Uses hardware BLEN (byte length)
  register to specify transfer length. Hardware automatically handles ACK/NACK
  and STOP generation for multi-byte transfers.
- **Legacy mode** (default): Transfers one byte per ``FRM_START`` trigger.
  Software manages START, STOP, and ACK/NACK per byte.

Enabling the Driver
===================

Enable the I2C node in the application's board overlay, and the
I2C subsystem in ``prj.conf``. ``CONFIG_I2C_TI_UNICOMM`` is selected automatically
once an enabled ``ti,unicomm-i2c`` node is present.

.. code-block:: devicetree

   &unicomm0_i2c {
           status = "okay";
           clock-frequency = <I2C_BITRATE_STANDARD>;
           pinctrl-0 = <&i2c0_default>;
           pinctrl-names = "default";
   };

.. code-block:: kconfig

   CONFIG_I2C=y

Devicetree Properties
=====================

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Property
     - Description
   * - ``clock-frequency``
     - Desired I2C bus frequency in Hz. Standard values: 100000 (Standard),
       400000 (Fast), 1000000 (Fast+). The driver selects the nearest
       achievable frequency not exceeding the requested value.
   * - ``pinctrl-0`` / ``pinctrl-names``
     - Pin control configuration for SCL/SDA pins. Required.
   * - ``rxfifo-threshold``
     - Target RX FIFO trigger threshold. This is the raw value of the relevant
       IFLS register field. Check TRM to see what value corresponds to the desired
       level.
   * - ``txfifo-threshold``
     - Target TX FIFO trigger threshold. This is the raw value of the relevant
       IFLS register field. Check TRM to see what value corresponds to the desired
       level.
   * - ``unicomm-advanced-i2c``
     - Boolean. Enables advanced controller mode with automatic STOP generation
       and BLEN-based multi-byte transfers. Refer to device TRM for support.

Speed Configuration
-------------------

The driver supports three standard I2C speeds, selected via the
``I2C_SPEED_GET(dev_config)`` macro in :c:func:`i2c_configure`:

- ``I2C_SPEED_STANDARD`` — 100 kHz
- ``I2C_SPEED_FAST`` — 400 kHz
- ``I2C_SPEED_FAST_PLUS`` — 1 MHz
- ``I2C_SPEED_DT`` — Use the ``clock-frequency`` devicetree property

The actual frequency achieved depends on the peripheral clock source (BUSCLK)
and the timer period register (TPR). The driver calculates TPR to produce a
frequency at or below the requested rate.

Functional clock = BUSCLK / (clkdiv + 1)
SCL period = (TPR + 1) * (SCL_LP + SCL_HP) * CLK_PRD

Where SCL_LP = 6, SCL_HP = 4, so SCL_LP_HP = 10.

FIFO Operation
--------------

The UNICOMM I2C hardware provides TX and RX FIFOs. The driver
configures FIFO thresholds at init time via devicetree properties
``rxfifo-threshold`` and ``txfifo-threshold``. **FIFO depth can vary
across devices (16 bytes on AM13, 4 on MSPM33). Check your device TRM
to select the correct threshold values based on the FIFO depth.**.

Controller Mode Operation
------------------------------

The controller API is the standard Zephyr :c:func:`i2c_transfer` function.
Messages are described by ``struct i2c_msg`` arrays.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/i2c.h>

   static const struct device *const i2c_dev = DEVICE_DT_GET_ONE(ti_unicomm_i2c);
   uint8_t tx_buf[] = {0x00, 0x01}; /* register address + data */
   uint8_t rx_buf[2];

   struct i2c_msg msgs[] = {
       {
           .buf = tx_buf,
           .len = sizeof(tx_buf),
           .flags = I2C_MSG_WRITE,
       },
       {
           .buf = rx_buf,
           .len = sizeof(rx_buf),
           .flags = I2C_MSG_READ | I2C_MSG_STOP,
       },
   };

   int ret = i2c_transfer(i2c_dev, msgs, ARRAY_SIZE(msgs), 0x50);

Target Mode Operation
=====================

When ``CONFIG_I2C_TARGET`` is enabled, the driver supports I2C target mode
operation. Register the target callbacks with :c:func:`i2c_target_register`:

.. code-block:: c

   static int write_requested(struct i2c_target_config *cfg)
   {
       /* Prepare for receiving data */
       return 0;
   }

   static int write_received(struct i2c_target_config *cfg, uint8_t val)
   {
       /* Process received byte */
       return 0;
   }

   static int read_requested(struct i2c_target_config *cfg, uint8_t *val)
   {
       *val = my_data[0];
       return 0;
   }

   static int read_processed(struct i2c_target_config *cfg, uint8_t *val)
   {
       static int idx = 1;
       if (idx < ARRAY_SIZE(my_data)) {
           *val = my_data[idx++];
           return 0;
       }
       return -ENODATA;
   }

   static void stop(struct i2c_target_config *cfg)
   {
       /* Transaction complete */
   }

   static void error(struct i2c_target_config *cfg, int err)
   {
       /* Handle error */
   }

   static const struct i2c_target_callbacks target_callbacks = {
       .write_requested = write_requested,
       .write_received = write_received,
       .read_requested = read_requested,
       .read_processed = read_processed,
       .stop = stop,
       .error = error,
   };

   static struct i2c_target_config target_cfg = {
       .address = 0x50,
       .flags = I2C_TARGET_FLAGS_ADDR_10_BITS, /* optional */
       .callbacks = &target_callbacks,
   };

   int main(void)
   {
       const struct device *i2c_dev = DEVICE_DT_GET_ONE(ti_unicomm_i2c);
       i2c_target_register(i2c_dev, &target_cfg);
       /* ... */
   }

Target Addressing
-----------------

The driver supports both 7-bit and 10-bit target addresses, configured via
``I2C_TARGET_FLAGS_ADDR_10_BITS`` in the target config flags.

Tests
=====

- ``i2c_target_api test``: Tests I2C target callbacks (write, read, stop, error)

Zephyr Documentation
====================

:ws-doc:`Zephyr I2C Documentation <zephyr/hardware/peripherals/i2c.html>`
