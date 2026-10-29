Overview
========

MCAN controllers are exposed through Zephyr's CAN API (``<zephyr/drivers/can.h>``). The driver
(``ti,k3-mcan``) only supplies the TI specific parts: register and message RAM access, pin
configuration, clock control and interrupt routing. It leaves the CAN protocol handling to the
upstream Bosch M_CAN subsystem (``CONFIG_CAN_MCAN``).

Features Supported
==================

- Classic CAN with 11-bit and 29-bit identifiers: :c:func:`can_send`,
  :c:func:`can_add_rx_filter` and :c:func:`can_remove_rx_filter`
- CAN FD (``CONFIG_CAN_FD_MODE``), with a separate data phase bit rate (``bitrate-data``)
- Normal, listen-only, loopback and one-shot modes with :c:func:`can_set_mode`
- Bit rate and sample point from devicetree (``bitrate``, ``sample-point``), or at run time with
  :c:func:`can_set_bitrate` and :c:func:`can_set_timing`
- Bus state reporting with :c:func:`can_get_state` and state change callbacks
- Manual bus-off recovery with :c:func:`can_recover` (``CONFIG_CAN_MANUAL_RECOVERY_MODE``)
- Clock control through the SCMI clock controller.
- Pin configuration through pinctrl
- Message RAM layout from devicetree (``bosch,mram-cfg``)

Features NOT Supported
======================

- Device power management

Usage
=====

Enabling the Driver
-------------------

The application must set ``CONFIG_CAN``. ``CONFIG_CAN_TI_K3_MCAN`` is enabled by default once an
enabled ``ti,k3-mcan`` node is present, and selects ``CONFIG_CAN_MCAN`` and ``CONFIG_PINCTRL``.

.. code-block:: kconfig

   CONFIG_CAN=y
   CONFIG_CAN_FD_MODE=y # optional

The SoC devicetree defines each controller as follows (shown for ``main_mcan0``).

.. code-block:: devicetree

   main_mcan0: can@20701000 {
           compatible = "ti,k3-mcan";
           reg = <0x20701000 0x200>, <0x20708000 0x8000>;
           reg-names = "m_can", "message_ram";
           interrupts = <GIC_SPI 113 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>,
                        <GIC_SPI 114 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-names = "int0", "int1";
           interrupt-parent = <&gic>;
           power-domains = <&main_mcan0_pd>;
           clocks = <&scmi_clk 178>;
           bosch,mram-cfg = <0 128 64 64 64 64 32 32>;
           status = "disabled";
   };

``bosch,mram-cfg`` splits the message RAM. Its cells are the offset, followed by the number of
elements for the 11-bit filters, the 29-bit filters, RX FIFO 0, RX FIFO 1, the RX buffers, the TX
event FIFO and the TX buffers. The message RAM region is 32 KiB (``0x8000``), and the layout
must fit in it.

.. code-block:: devicetree

   / {
           chosen {
                   zephyr,canbus = &main_mcan0;
           };
   };

   &main_mcan0 {
           pinctrl-0 = <&main_mcan0_rx>, <&main_mcan0_tx>;
           pinctrl-names = "default";
           bitrate = <500000>;
           status = "okay";
   };

Sending and Receiving Frames
----------------------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/can.h>
   #include <zephyr/kernel.h>

   static const struct device *const can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

   static void rx_cb(const struct device *dev, struct can_frame *frame, void *user_data)
   {
           printk("id 0x%x, %u bytes\n", frame->id, can_dlc_to_bytes(frame->dlc));
   }

   int main(void)
   {
           const struct can_filter filter = {.id = 0x123, .mask = CAN_STD_ID_MASK, .flags = 0};
           struct can_frame frame = {.id = 0x123, .dlc = 2, .data = {0xaa, 0x55}};
           int ret;

           if (!device_is_ready(can_dev)) {
                   return -ENODEV;
           }

           ret = can_add_rx_filter(can_dev, rx_cb, NULL, &filter);
           if (ret < 0) {
                   return ret;
           }

           ret = can_start(can_dev);
           if (ret < 0) {
                   return ret;
           }

           return can_send(can_dev, &frame, K_MSEC(100), NULL, NULL);
   }

Loopback
--------

In loopback mode the controller receives its own frames without driving the transceiver, so no
bus is needed. Set the mode while the controller is stopped:

.. code-block:: c

   ret = can_set_mode(can_dev, CAN_MODE_LOOPBACK);
   if (ret < 0) {
           return ret;
   }

   ret = can_start(can_dev);

Call :c:func:`can_stop` before changing the mode again.

CAN FD
------

With ``CONFIG_CAN_FD_MODE=y`` set ``bitrate-data`` in devicetree, or call
:c:func:`can_set_timing_data`, then switch to FD mode and send frames with the ``CAN_FRAME_FDF``
and ``CAN_FRAME_BRS`` flags:

.. code-block:: c

   struct can_frame frame = {
           .id = 0x123,
           .flags = CAN_FRAME_FDF | CAN_FRAME_BRS,
           .dlc = can_bytes_to_dlc(12),
   };

   can_set_mode(can_dev, CAN_MODE_FD);
   can_start(can_dev);
   can_send(can_dev, &frame, K_MSEC(100), NULL, NULL);

Samples
=======

- :ws-doc:`CAN Counter Sample <zephyr/samples/drivers/can/counter/README.html>`
  (``samples/drivers/can/counter``), which uses ``DT_CHOSEN(zephyr_canbus)``
- :ws-doc:`CAN Babbling Sample <zephyr/samples/drivers/can/babbling/README.html>`
  (``samples/drivers/can/babbling``)

Tests
=====

- ``tests/drivers/can/api`` (``drivers.can.api``)

Devicetree Bindings
===================

- :ws-doc:`ti,k3-mcan <zephyr/build/dts/api/bindings/can/ti,k3-mcan.html>`
- :ws-doc:`bosch,m_can-base <zephyr/build/dts/api/bindings/can/bosch,m_can-base.html>`
