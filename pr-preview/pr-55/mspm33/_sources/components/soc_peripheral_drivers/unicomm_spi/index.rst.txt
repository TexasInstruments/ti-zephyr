.. ti-socs: mspm33 am13

.. _ti-zephyr-component-spi-unicomm:

UNICOMM SPI
###########

.. important::

   The UNICOMM SPI driver supports Texas Instruments microcontrollers ``am13e23019`` and ``mspm33c321a``.

Features Supported
******************

- Controller and Peripheral mode operation
- Motorola SPI frame format (3-wire and 4-wire)
- TI frame format (SSI)
- 8-bit data frame size
- Configurable clock polarity (CPOL) and phase (CPHA)
- Internal loopback mode for testing
- Interrupt-driven operation with TX/RX FIFOs (check TRM for FIFO depth)
- Configurable TX/RX FIFO interrupt thresholds
- Synchronous and asynchronous (callback-based) transfers
- Runtime clock frequency configuration
- GPIO based flexible chip-select management (controller mode)

Features NOT Supported
**********************

- Data frame sizes other than 8-bit
- Hardware DMA support

Usage
*****

The driver implements Zephyr's standard SPI API (``<zephyr/drivers/spi.h>``).
Applications use :c:func:`spi_transceive` for synchronous transfers and
:c:func:`spi_transceive_async` for asynchronous transfers.

Enabling the Driver
===================

Enable the UNICOMM SPI node in the board devicetree (or an application overlay)
and the SPI subsystem in ``prj.conf``. ``CONFIG_SPI_TI_UNICOMM`` is selected
automatically once an enabled ``ti,unicomm-spi`` node is present.

.. code-block:: devicetree

   &unicommX_spi {
       status = "okay";
       pinctrl-0 = <&ucspi0_pins>;
       pinctrl-names = "default";
       cs-gpios = <&gpio0 10 GPIO_ACTIVE_LOW>;
   };

.. code-block:: kconfig

   CONFIG_SPI=y

Devicetree Properties
=====================

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Property
     - Description
   * - ``tx-fifo-threshold``
     - TX FIFO interrupt threshold (0-5, default: 4). See `FIFO Threshold Encoding`_.
   * - ``rx-fifo-threshold``
     - RX FIFO interrupt threshold (0-5, default: 0). See `FIFO Threshold Encoding`_.
   * - ``fifo-depth``
     - FIFO depth in bytes (default: 16). Pre-configured in devicetrees.
   * - ``cs-gpios``
     - GPIO chip-select pins for controller mode.

.. _ti-zephyr-spi-unicomm-fifo-thresholds:

FIFO Threshold Encoding
-----------------------

The TX and RX FIFO thresholds use hardware-defined encodings for a 16-entry FIFO. The ``tx-fifo-threshold``
and ``rx-fifo-threshold`` properties accept the raw IFLS register values. Check TRM for supported values based
on the FIFO depth for your device's UNICOMM peripheral.


SPI Modes and Configuration
===========================

Controller Mode
------------------------

In controller mode, the UNICOMM SPI generates the serial clock and initiates
transactions.

.. code-block:: c

   const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(ucspi0));
   struct spi_config spi_cfg = {
       .frequency = 1000000,
       .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
                    SPI_MODE_CPOL | SPI_MODE_CPHA,
       .slave = 0,
   };

   uint8_t tx_buf[] = {0x01, 0x02, 0x03};
   uint8_t rx_buf[3];

   const struct spi_buf tx_spi_buf = {.buf = tx_buf, .len = sizeof(tx_buf)};
   const struct spi_buf rx_spi_buf = {.buf = rx_buf, .len = sizeof(rx_buf)};
   const struct spi_buf_set tx = {.buffers = &tx_spi_buf, .count = 1};
   const struct spi_buf_set rx = {.buffers = &rx_spi_buf, .count = 1};

   int ret = spi_transceive(spi_dev, &spi_cfg, &tx, &rx);

Supported controller mode configurations:

* Motorola 4-wire (standard SPI with separate CS)
* Motorola 3-wire
* TI frame format
* CPOL/CPHA modes 0, 1, 2, 3 (Motorola format only)
* MSB-first or LSB-first bit ordering
* Clock frequencies up to BUSCLK/2

Peripheral Mode
-----------------------

In peripheral mode, the UNICOMM SPI responds to an external controller.

.. code-block:: c

   struct spi_config spi_cfg = {
       .frequency = 1000000,
       .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_SLAVE,
       .slave = 0,  /* Not used in peripheral mode */
   };

Supported peripheral mode configurations:

* Motorola 4-wire and 3-wire formats
* TI frame format
* CPOL/CPHA configuration must match the controller
* MSB-first or LSB-first bit ordering

Clock Configuration
===================

The SPI clock is derived from BUSCLK using the formula:

.. math::

   f_{SPI} = \frac{BUSCLK}{2 \times (1 + SCR)}

Where SCR (Serial Clock Rate) is a 10-bit value (0-1023) configured at runtime
based on the requested frequency. The driver automatically computes the
appropriate SCR value during configuration.

Error Handling
==============

The driver reports the following error conditions:

* ``-EINVAL``: Invalid configuration (unsupported mode, frequency too high, etc.)
* ``-ENOTSUP``: Unsupported feature (non-8-bit word size, half-duplex with TI format)
* ``-EIO``: RX FIFO overflow during transfer (data loss)
* ``-EBUSY``: Attempt to release while transfer in progress
* ``-ENODEV``: Clock device not ready

Interrupt Handling
==================

The driver uses a single interrupt handler that dispatches to either
controller or peripheral mode ISRs based on the current operating mode.

**Controller Mode Interrupts:**

* RX FIFO threshold reached
* RX FIFO full
* TX FIFO threshold reached
* TX FIFO empty
* RX FIFO overflow (error)
* SPI idle (transfer complete)

**Peripheral Mode Interrupts:**

* RX FIFO threshold reached
* RX FIFO full
* TX FIFO threshold reached
* SPI idle (controller deasserted CS)

Samples and Tests
*****************

Samples
=======

- `SPI Loopback Sample <../../../../../zephyr/samples/drivers/spi/README.html>`_

Tests
=====

- ``spi_loopback`` - Internal loopback test

References
==========

* ``drivers/spi/spi_ti_unicomm.c``
* ``dts/bindings/spi/ti,unicomm-spi.yaml``

Zephyr Documentation
====================

:ws-doc:`Zephyr SPI Documentation <zephyr/hardware/peripherals/spi.html>`
