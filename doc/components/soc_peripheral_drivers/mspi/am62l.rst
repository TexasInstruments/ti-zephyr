Cadence OSPI
************

Features Supported
==================

Applications use the flash API on the flash node. The Cadence OSPI controller (``cdns,xspi-nor``) is
an MSPI controller underneath it.

- MSPI API (``<zephyr/drivers/mspi.h>``) for other devices: polled :c:func:`mspi_transceive`
- IO modes single, dual, quad and octal, including the 1-1-2, 1-2-2, 1-1-4, 1-4-4, 1-1-8 and 1-8-8
  variants
- Data rates single, dual (DTR) and S_D_D (DDR)
- Command length of 1 or 2 bytes, address length of 1 to 4 bytes and up to 31 dummy cycles
- Clock from 6.25 MHz to 100 MHz (200 MHz reference divided in steps)
- Chip selects 0 to 3, SPI modes 0 to 3, little endian, active-low chip select
- Read data capture delay tuning

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - memory-mapped (direct access) mode and execute in place (only indirect transfers)
  - data strobe (DQS) and loopback clock capture (the driver ignores them)
  - interrupt-driven operation; completion is polled

- DMA transfers (``MSPI_DMA``) return ``-ENOTSUP``; only polled PIO is available.
- Big endian, active-high chip select, chip select above 3 and DQS mode return ``-ENOSYS``.
- A clock below 6.25 MHz returns ``-ENOTSUP``. A request timeout above 200 ms
  (``CONFIG_MSPI_COMPLETION_TIMEOUT_TOLERANCE``) returns ``-EINVAL``.
- Software multi-peripheral mode fails the build.

Usage
=====

For flash access, see :ref:`Using the Flash <ti-zephyr-flash-s28hx512t-usage>`.

.. note::

   The flash API is not required. Applications can use the controller directly through the MSPI API
   (:c:func:`mspi_dev_config` and :c:func:`mspi_transceive`) to send arbitrary commands to a device
   on the bus, bypassing the flash driver.

Enabling the Driver
-------------------

Set the flash subsystem in ``prj.conf``:

.. code-block:: kconfig

   CONFIG_FLASH=y

With ``CONFIG_FLASH`` set, ``CONFIG_FLASH_MSPI_NOR`` defaults to on for a ``jedec,nor`` node on an
MSPI bus. It selects ``CONFIG_FLASH_MSPI`` and ``CONFIG_MSPI``. ``CONFIG_MSPI_CDNS`` defaults to on
for the ``cdns,xspi-nor`` node.

The SoC devicetree defines the controller as:

.. code-block:: devicetree

   main_ospi0: ospi@fc40000 {
           compatible = "cdns,xspi-nor";
           reg = <0x0fc40000 0x100 0x60000000 0x4>;
           reg-names = "base", "fifo";
           power-domains = <&main_fss0_pd>;
           clock-frequency = <DT_FREQ_M(200)>;
           clocks = <&scmi_clk 136>;
           #address-cells = <1>;
           #size-cells = <0>;
           read-buffer-size = <0x80>;
           cdns,nss-delay-ns = <60>;
           cdns,btwn-delay-ns = <60>;
           cdns,after-delay-ns = <60>;
           cdns,init-delay-ns = <60>;
   };

To use the flash, set ``status = "okay"`` on the controller, apply the pin settings with
``pinctrl-0`` (clock, chip select, data lines D0 to D7, data strobe and loopback clock) and add the
flash child:

.. code-block:: devicetree

   &main_ospi0 {
           pinctrl-0 = <&main_ospi0_clk
                        &main_ospi0_d0
                        &main_ospi0_d1
                        &main_ospi0_d2
                        &main_ospi0_d3
                        &main_ospi0_d4
                        &main_ospi0_d5
                        &main_ospi0_d6
                        &main_ospi0_d7
                        &main_ospi0_dqs
                        &main_ospi0_lbclko
                        &main_ospi0_csn0>;
           pinctrl-names = "default";
           status = "okay";

           flash0: flash@0 {
                   compatible = "infineon,s28hx512t", "jedec,nor";
                   reg = <0>;
                   jedec-id = [34 5b 1a];
                   size = <DT_SIZE_M(512)>;
                   erase-block-size = <DT_SIZE_K(256)>;
                   mspi-max-frequency = <DT_FREQ_M(25)>;
                   mspi-io-mode = "MSPI_IO_MODE_OCTAL";
                   mspi-data-rate = "MSPI_DATA_RATE_DUAL";
                   mspi-endian = "MSPI_LITTLE_ENDIAN";
                   mspi-cpp-mode = "MSPI_CPP_MODE_0";
                   mspi-ce-polarity = "MSPI_CE_ACTIVE_LOW";
                   mspi-hardware-ce-num = <0>;
                   address-length = "ADDR_4_BYTE";
                   command-length = "INSTR_2_BYTE";
                   initial-soft-reset;
                   t-reset-recovery = <90000>;
           };
   };

Devicetree Bindings
===================

- :ws-doc:`cdns,xspi-nor <zephyr/build/dts/api/bindings/mspi/cdns,xspi-nor.html>`
- :ws-doc:`infineon,s28hx512t-mspi <zephyr/build/dts/api/bindings/mtd/infineon,s28hx512t-mspi.html>`
- :ws-doc:`jedec,nor-mspi <zephyr/build/dts/api/bindings/mtd/jedec,nor-mspi.html>`
