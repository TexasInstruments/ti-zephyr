McSPI
*****

Features Supported
==================

- Controller mode of the McSPI (``ti,omap-mcspi``) through the Zephyr SPI API
  (``<zephyr/drivers/spi.h>``): :c:func:`spi_transceive`, its devicetree variants and
  :c:func:`spi_release`
- Full duplex, TX-only and RX-only transfers, with several buffers per call
- Word sizes from 4 to 32 bits
- SPI modes 0 to 3 (``SPI_MODE_CPOL`` / ``SPI_MODE_CPHA``) and ``SPI_CS_ACTIVE_HIGH``
- Up to four hardware chip selects (``ti,spi-num-cs``), or GPIO chip selects from ``cs-gpios``
  (initialized at driver start)
- Chip select asserted at the start and deasserted at the end of each :c:func:`spi_transceive` call,
  for hardware and GPIO chip selects, so devices that need CS to toggle between calls work
- Internal loopback (``SPI_MODE_LOOP``) and swapped data lines with ``ti,d1-miso-d0-mosi``
- Bus locking with ``SPI_LOCK_ON``
- Experimental peripheral mode with ``CONFIG_SPI_SLAVE`` (not verified on hardware)

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - DMA; all transfers are programmed I/O
  - interrupt-driven operation; the driver polls the channel status
  - multi-channel controller mode (only one channel is active at a time)
  - start-bit mode and the programmable chip select timing control

- Device power management.

Usage
=====

Enabling the Driver
-------------------

The application must set ``CONFIG_SPI``; it is not selected by the devicetree.

.. code-block:: kconfig

   CONFIG_SPI=y

``CONFIG_SPI_OMAP_MCSPI`` is enabled by default once an enabled ``ti,omap-mcspi`` node is present,
and selects ``PINCTRL``. Set ``CONFIG_SPI_SLAVE=y`` only for peripheral mode.

The SoC devicetree defines each controller as follows (shown for ``main_spi0``):

.. code-block:: devicetree

   main_spi0: spi@20100000 {
           compatible = "ti,omap-mcspi";
           reg = <0x20100000 0x400>;
           interrupts = <GIC_SPI 80 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-parent = <&gic>;
           #address-cells = <1>;
           #size-cells = <0>;
           power-domains = <&main_spi0_pd>;
           clock-frequency = <DT_FREQ_M(50)>;
           clocks = <&scmi_clk 299>;
           ti,spi-num-cs = <4>;
   };

Enable a controller and add one child node per device, with ``reg`` set to the chip select index:

.. code-block:: devicetree

   &main_spi0 {
           pinctrl-0 = <&main_spi0_clk &main_spi0_cs0 &main_spi0_d0 &main_spi0_d1>;
           pinctrl-names = "default";
           status = "okay";

           my_spi_dev: spi-device@0 {
                   compatible = "vnd,spi-device";
                   reg = <0>;
                   spi-max-frequency = <DT_FREQ_M(10)>;
           };
   };

Transferring Data
-----------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/spi.h>

   static const struct spi_dt_spec spi_dev = SPI_DT_SPEC_GET(
           DT_NODELABEL(my_spi_dev), SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB);

   int main(void)
   {
           uint8_t tx_data[] = {0x9f, 0x00, 0x00, 0x00};
           uint8_t rx_data[sizeof(tx_data)] = {0};
           const struct spi_buf tx_buf = {.buf = tx_data, .len = sizeof(tx_data)};
           const struct spi_buf rx_buf = {.buf = rx_data, .len = sizeof(rx_data)};
           const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};
           const struct spi_buf_set rx = {.buffers = &rx_buf, .count = 1};
           int ret;

           if (!spi_is_ready_dt(&spi_dev)) {
                   return -ENODEV;
           }

           /* Full duplex: the reply bytes arrive while the command and padding are sent */
           ret = spi_transceive_dt(&spi_dev, &tx, &rx);
           if (ret < 0) {
                   return ret;
           }

           printk("ID: %02x %02x %02x\n", rx_data[1], rx_data[2], rx_data[3]);

           return 0;
   }

Devicetree Bindings
===================

- :ws-doc:`ti,omap-mcspi <zephyr/build/dts/api/bindings/spi/ti,omap-mcspi.html>`
