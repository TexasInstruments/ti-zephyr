S28HX512T Flash
***************

.. important::

   The Infineon S28HX512T is a 512 Mbit (64 MB) Semper octal flash. It connects to the :ref:`Cadence
   OSPI <ti-zephyr-component-mspi>` controller. The ``infineon,s28hx512t`` compatible makes the
   generic MSPI NOR flash driver apply the device-specific setup described here.

Features Supported
==================

- Flash API (``<zephyr/drivers/flash.h>``) through the generic MSPI NOR driver (``jedec,nor``):
  :c:func:`flash_read`, :c:func:`flash_write`, :c:func:`flash_erase`, :c:func:`flash_get_size`, the
  flash parameters (write block size 1 byte, erased value 0xFF, explicit erase required) and, with
  ``CONFIG_FLASH_PAGE_LAYOUT``, the page layout
- JEDEC ID read with ``CONFIG_FLASH_JESD216_API``, and a check against the ``jedec-id`` property at
  start-up
- Erase in 256 KB sectors
- Uniform sector layout i.e, all sectors are 256KB each
- Single-line operation (1S-1S-1S) with 3-byte or 4-byte addresses.
- Octal operation, SDR (8S-8S-8S) or DDR (8D-8D-8D).
- Read data delay calibration on the Cadence OSPI controller: the driver steps the read delay from
  15 to 0, reads the JEDEC ID at each value and programs the middle of the range that returns the
  expected ID
- Optional soft reset (``initial-soft-reset``) and hardware reset (``reset-gpios``) at start-up,
  followed by the ``t-reset-recovery`` wait
- Board option ``CONFIG_BOARD_TI_MSPI_FLASH_BOOT_MODE_RESET`` returns the flash from 8D-8D-8D to
  1S-1S-1S mode when the boot ROM left it in octal mode. It defaults to on when MSPI is enabled.

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - Advanced Sector Protection, password protection and the one-time programmable (secure silicon)
    region
  - Hybrid Sector mode
  - execute in place (not provided by the OSPI controller driver)

- Erase sizes are multiples of the 256 KB sector; the 4 KB sector erase opcode is not used. Set
  ``erase-block-size`` to 256 KB.
- Page programming in single-line mode needs 4-byte addressing; with 3-byte addresses the driver
  logs a warning.
- The SFDP tables are not used. Parameters come from the driver quirks and devicetree.
- Device power management of the flash is not tested, although it is implemented by the jedec,nor
  MSPI flash driver.

Usage
=====

Enabling the Driver
-------------------

The flash subsystem is enabled by the application:

.. code-block:: kconfig

   CONFIG_FLASH=y

``CONFIG_FLASH_MSPI_NOR`` defaults to on for a ``jedec,nor`` node on an MSPI bus. Set the page
layout size to the sector size:

.. code-block:: kconfig

   CONFIG_FLASH_PAGE_LAYOUT=y
   CONFIG_FLASH_MSPI_NOR_LAYOUT_PAGE_SIZE=262144

Devicetree Configuration
------------------------

The flash is a child of the OSPI controller.

.. code-block:: devicetree

   &main_ospi0 {
           status = "okay";

           flash0: flash@0 {
                   compatible = "infineon,s28hx512t", "jedec,nor";
                   status = "okay";
                   reg = <0>;
                   jedec-id = [34 5b 1a];
                   size = <DT_SIZE_M(512)>;     /* 512 Mbits (64 MB), in bits */
                   erase-block-size = <DT_SIZE_K(256)>;
                   mspi-max-frequency = <DT_FREQ_M(25)>;
                   mspi-io-mode = "MSPI_IO_MODE_OCTAL";
                   mspi-endian = "MSPI_LITTLE_ENDIAN";
                   mspi-data-rate = "MSPI_DATA_RATE_DUAL";
                   address-length = "ADDR_4_BYTE";
                   command-length = "INSTR_2_BYTE";
                   mspi-hardware-ce-num = <0>;
                   mspi-cpp-mode = "MSPI_CPP_MODE_0";
                   mspi-ce-polarity = "MSPI_CE_ACTIVE_LOW";
                   initial-soft-reset;
                   t-reset-recovery = <90000>;   /* 90 us */
                   #address-cells = <1>;
                   #size-cells = <1>;
                   ranges = <0x0 0x60000000 DT_SIZE_M(64)>;

                   flash0_memory: flash@0 {
                           compatible = "soc-nv-flash";
                           reg = <0x0 DT_SIZE_M(64)>;
                           #address-cells = <1>;
                           #size-cells = <1>;
                           ranges;
                   };
           };
   };

The compatible list must start with ``infineon,s28hx512t``, followed by ``jedec,nor``, so that both
the binding and the quirks are found. The node accepts the properties of the ``jedec,nor`` MSPI
binding:

.. _ti-zephyr-flash-s28hx512t-usage:

Using the Flash
---------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/flash.h>

   #define FLASH_NODE DT_NODELABEL(flash0)
   #define SECTOR     (256 * 1024)

   int main(void)
   {
           const struct device *flash = DEVICE_DT_GET(FLASH_NODE);
           uint8_t buf[16] = "hello s28hx512t";
           uint8_t rd[16];

           if (!device_is_ready(flash)) {
                   return -ENODEV;
           }

           /* A 256 KB sector must be erased before it is written */
           flash_erase(flash, 0, SECTOR);
           flash_write(flash, 0, buf, sizeof(buf));
           flash_read(flash, 0, rd, sizeof(rd));

           return 0;
   }

Hardware Notes
--------------
 The boot ROM can leave the flash in 8D-8D-8D mode. ``CONFIG_BOARD_TI_MSPI_FLASH_BOOT_MODE_RESET``
  sends the octal reset enable and reset commands (0x6666, 0x9999) to every child of the chosen
  flash controller, waits for ``t-reset-recovery`` and returns the part to single-line mode
  before the flash driver initializes.

Samples
-------

- ``samples/drivers/flash_shell``: shell commands to read, erase and write the flash
- :ws-doc:`JESD216 sample <zephyr/samples/drivers/jesd216/README.html>`

Devicetree Bindings
===================

- :ws-doc:`infineon,s28hx512t-mspi <zephyr/build/dts/api/bindings/mtd/infineon,s28hx512t-mspi.html>`
- :ws-doc:`jedec,nor-mspi <zephyr/build/dts/api/bindings/mtd/jedec,nor-mspi.html>`
