NS16550 UART
************

Features Supported
==================

- UART instances are exposed through the generic NS16550 driver (``ns16550``) and the Zephyr UART API
  (``<zephyr/drivers/uart.h>``)
- Polling API: :c:func:`uart_poll_in` and :c:func:`uart_poll_out`
- Interrupt-driven API (``CONFIG_UART_INTERRUPT_DRIVEN``), using the interrupt in the SoC devicetree
- Runtime configuration (``CONFIG_UART_USE_RUNTIME_CONFIGURE``) of the baud rate, 5 to 8 data bits,
  1 or 2 stop bits and parity (none, odd or even)
- Error reporting through :c:func:`uart_err_check`
- Hardware flow control (RTS/CTS) with the 16750 variant (``UART_CFG_FLOW_CTRL_RTS_CTS``)
- Line control of RTS and DTR with ``CONFIG_UART_NS16550_LINE_CTRL``
- Internal loopback with the ``loopback`` devicetree property
- The TI K3 register extension (``CONFIG_UART_NS16550_TI_K3``), which puts the mode definition
  register in standard UART mode when the port is initialized
- Pin configuration through pinctrl

Features NOT Supported
======================

- DMA support

Usage
=====

Enabling the Driver
-------------------

The application must set ``CONFIG_SERIAL``. ``CONFIG_UART_NS16550`` is enabled by default once an
enabled ``ns16550`` node is present, and selects ``PINCTRL`` when the node has a ``pinctrl-0``
property.
For TI K3 the SoC configuration defaults the following to ``y``, so no extra options are
needed for the console:

.. code-block:: kconfig

   CONFIG_UART_NS16550_TI_K3=y
   CONFIG_UART_NS16550_VARIANT_NS16750=y

The 16750 variant has 64-byte FIFOs and automatic hardware flow control. Add the following for the
interrupt-driven API and runtime configuration:

.. code-block:: kconfig

   CONFIG_SERIAL=y
   CONFIG_UART_INTERRUPT_DRIVEN=y
   CONFIG_UART_USE_RUNTIME_CONFIGURE=y

.. code-block:: devicetree

   main_uart0: serial@2800000 {
           compatible = "ns16550";
           reg = <0x02800000 0x100>;
           interrupts = <GIC_SPI 96 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-parent = <&gic>;
           clock-frequency = <DT_FREQ_M(48)>;
           clocks = <&scmi_clk 358>;
           power-domains = <&main_uart0_pd>;
           current-speed = <115200>;
           reg-shift = <2>;
           status = "disabled";
   };

Because ``clock-frequency`` is set, the driver uses it as the input clock of the baud rate divider
and does not ask the clock controller. Enable the port, set the speed and apply the pins.

.. code-block:: devicetree

   &main_uart0 {
           current-speed = <115200>;
           pinctrl-0 = <&uart0_rx_default>, <&uart0_tx_default>;
           pinctrl-names = "default";
           status = "okay";
   };

Polling
-------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/uart.h>
   #include <zephyr/kernel.h>

   static const struct device *const uart_dev = DEVICE_DT_GET(DT_NODELABEL(main_uart1));

   int main(void)
   {
           uint8_t c;

           if (!device_is_ready(uart_dev)) {
                   return -ENODEV;
           }

           uart_poll_out(uart_dev, 'H');

           while (uart_poll_in(uart_dev, &c) < 0) {
                   k_msleep(10);
           }

           return 0;
   }

Interrupt-Driven
----------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/uart.h>

   static const struct device *const uart_dev = DEVICE_DT_GET(DT_NODELABEL(main_uart1));

   static void uart_cb(const struct device *dev, void *user_data)
   {
           uint8_t c;

           if (!uart_irq_update(dev) || !uart_irq_rx_ready(dev)) {
                   return;
           }

           while (uart_fifo_read(dev, &c, 1) == 1) {
                   uart_poll_out(dev, c); /* Echo back */
           }
   }

   int main(void)
   {
           if (!device_is_ready(uart_dev)) {
                   return -ENODEV;
           }

           uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
           uart_irq_rx_enable(uart_dev);

           return 0;
   }

Runtime Configuration and Flow Control
--------------------------------------

.. code-block:: c

   #include <zephyr/drivers/uart.h>

   int set_921600_with_flow_control(const struct device *dev)
   {
           const struct uart_config cfg = {
                   .baudrate = 921600,
                   .parity = UART_CFG_PARITY_NONE,
                   .stop_bits = UART_CFG_STOP_BITS_1,
                   .data_bits = UART_CFG_DATA_BITS_8,
                   .flow_ctrl = UART_CFG_FLOW_CTRL_RTS_CTS,
           };

           return uart_configure(dev, &cfg);
   }

Hardware flow control also needs the RTS and CTS pins of the instance in ``pinctrl-0``.

Samples
-------

- :ws-doc:`UART Echo Sample <zephyr/samples/drivers/uart/echo_bot/README.html>`
- :ws-doc:`UART Passthrough Sample <zephyr/samples/drivers/uart/passthrough/README.html>`
  (``samples/drivers/uart/passthrough``)

Devicetree Bindings
===================

- :ws-doc:`ns16550 <zephyr/build/dts/api/bindings/serial/ns16550.html>`
