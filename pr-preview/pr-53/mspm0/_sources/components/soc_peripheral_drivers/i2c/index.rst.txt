.. ti-socs: mspm0

.. _ti-zephyr-component-i2c-mspm0:

I2C
#########

Features Supported
******************

- I2C Controller mode (Standard, Fast, and Fast+ modes up to 1 MHz)
- I2C Target mode (7-bit and 10-bit addressing)
- Dual-role operation (simultaneous Controller and Target)
- Hardware FIFO with configurable TX/RX thresholds
- Clock stretching support (Controller and Target)
- SCL low timeout detection
- Power management (PM_DEVICE runtime suspend/resume)
- 7-bit and 10-bit addressing modes
- Multi-message transfers with RESTART/STOP control
- Interrupt-driven operation

Features NOT Supported
**********************

- High-speed (3.4 MHz) and Ultra-fast (5 MHz) modes
- SMBus-specific features (PEC, ARP, etc.)

Usage
*****

The driver implements Zephyr's standard I2C API (``<zephyr/drivers/i2c.h>``).
Applications use :c:func:`i2c_transfer` for controller transactions and
:c:func:`i2c_target_register` for target operation.

The driver supports both controller and target roles simultaneously (dual-role).
When ``CONFIG_I2C_TARGET`` is enabled, the device can act as an I2C target while
still being able to initiate controller transfers.

Enabling the Driver
===================

Enable the I2C node in the board devicetree (or an application overlay) and the
I2C subsystem in ``prj.conf``. ``CONFIG_I2C_MSPM0`` is selected automatically
once an enabled ``ti,mspm0-i2c`` node is present.

.. code-block:: devicetree

   &i2c0 {
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
   * - ``controller-tx-fifo-threshold``
     - Controller TX FIFO trigger threshold (1–7). Interrupt fires when FIFO
       contains this many or fewer bytes. Default: 1.
   * - ``controller-rx-fifo-threshold``
     - Controller RX FIFO trigger threshold (1–8). Interrupt fires when FIFO
       contains this many or more bytes. Default: 1.

Kconfig Options
===============

The following Kconfig options are available under ``CONFIG_I2C_MSPM0``:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Option
     - Description
   * - ``MSPM0_I2C_SCL_LOW_TIMEOUT_MS``
     - SCL low timeout in milliseconds (default: 1000). Detects stuck bus
       conditions. Set to 0 to disable.
   * - ``MSPM0_I2C_FIFO_DEPTH``
     - Hardware TX/RX FIFO depth (default: 8). SoC-specific; can be overridden
       in SoC defconfig.
   * - ``MSPM0_I2C_MAX_DISPATCH_MSGS``
     - Maximum number of combined dispatchable messages (default: 8). Controls
       internal memory allocation for message merging.

Speed Configuration
-------------------

The driver supports three standard I2C speeds, selected via the
``I2C_SPEED_GET(dev_config)`` macro in :c:func:`i2c_configure`:

- ``I2C_SPEED_STANDARD`` — 100 kHz
- ``I2C_SPEED_FAST`` — 400 kHz
- ``I2C_SPEED_FAST_PLUS`` — 1 MHz
- ``I2C_SPEED_DT`` — Use the ``clock-frequency`` devicetree property

The actual frequency achieved depends on the peripheral clock source and the
timer period register (TPR). The driver calculates TPR to produce a frequency
at or below the requested rate.

FIFO Thresholds
---------------

TX and RX FIFO thresholds are configured at init time via devicetree properties
``controller-tx-fifo-threshold`` and ``controller-rx-fifo-threshold``. During a
transfer:

- TX: When the FIFO level drops to or below the threshold, the ``CTXFIFOTRG``
  interrupt fires and the ISR refills the FIFO.
- RX: When the FIFO level reaches or exceeds the threshold, the ``CRXFIFOTRG``
  interrupt fires and the ISR drains the FIFO.

Timeout Handling
----------------

When ``MSPM0_I2C_SCL_LOW_TIMEOUT_MS`` is non-zero, the driver configures the
hardware timeout counter (Timeout A). Each timeout tick corresponds to
``(TPR + 1) * 12`` functional clock cycles. If SCL is held low continuously
for longer than the configured timeout, the ``TIMEOUTA`` interrupt fires, the
transfer is aborted with ``-ETIMEDOUT``, and the controller is reset.

Error Handling
--------------

The ISR handles the following controller error conditions:

- **NACK received** (``CNACKFG``): Target did not acknowledge. Transfer fails
  with ``-EIO``.
- **Arbitration lost** (``CARBLOSTFG``): Another controller won the bus.
  Transfer fails with ``-EIO``.
- **SCL low timeout** (``TIMEOUTA``): SCL stuck low. Transfer fails with
  ``-ETIMEDOUT``.

In all cases the driver resets the controller hardware and restores the
configuration before returning the error code.

Controller Mode Operation
=========================

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/i2c.h>

   static const struct device *const i2c_dev = DEVICE_DT_GET_ONE(ti_mspm0_i2c);
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

When ``CONFIG_I2C_TARGET`` is enabled, the driver supports I2C target operation.
Register the target callbacks with :c:func:`i2c_target_register`:

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
       const struct device *i2c_dev = DEVICE_DT_GET_ONE(ti_mspm0_i2c);
       i2c_target_register(i2c_dev, &target_cfg);
       /* ... */
   }

Target Addressing
-----------------

The driver supports both 7-bit and 10-bit target addresses, configured via
``I2C_TARGET_FLAGS_ADDR_10_BITS`` in the target config flags.

Dual-Role Operation
-------------------

The MSPM0 I2C hardware supports simultaneous controller and target operation on
the same bus. The driver enables both controller and target interrupts when
a target is registered. The ISR uses the MIS (masked interrupt status) register
to distinguish controller vs. target interrupts and dispatches to the
appropriate handler.

Tests
=====

- ``i2c_target_api test``: Tests I2C target callbacks (write, read, stop, error)

Zephyr Documentation
====================

:ws-doc:`I2C Zephyr Documentation <zephyr/hardware/peripherals/i2c.html>`
