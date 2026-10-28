AM654 SDHCI
***********

Features Supported
==================

- SD and eMMC host controller (``ti,am654-sdhci``) through the Zephyr SDHC API
  (``<zephyr/drivers/sdhc.h>``): :c:func:`sdhc_request`, :c:func:`sdhc_set_io`,
  :c:func:`sdhc_card_busy`, :c:func:`sdhc_get_card_present`, :c:func:`sdhc_execute_tuning` and
  :c:func:`sdhc_hw_reset`
- SD card subsystem and disk access API, with ``mmc`` (eMMC) and ``sdmmc`` (SD card) disks
- Interrupt-driven transfers with timeouts, using ADMA2 or programmed I/O
  (``CONFIG_SDHC_TI_AM654_ENABLE_ADMA``, default ``y``)
- Multi-block transfers with automatic stop commands (CMD12, or CMD23 for SDR104 with ADMA) via
  ``CONFIG_SDHC_TI_AM654_ENABLE_AUTO_STOP``
- Bus widths of 1, 4 and 8 bits (8 bits on the eMMC controller)
- Bus voltages of 3.3 V, 3.0 V and 1.8 V, with optional ``vmmc-supply`` and ``vqmmc-supply``
- Timing modes from legacy up to SDR104 and HS200, including SDR12, SDR25, SDR50, DDR50 and eMMC
  DDR52
- Tap delay tuning for SDR104, HS200, and SDR50 when the card requires it
- Card insertion and removal interrupts (``SDHC_INT_INSERTED``, ``SDHC_INT_REMOVED``)

Features NOT Supported
======================

- 1.2 V signalling (``SD_VOL_1_2_V``) returns ``-ENOTSUP``
- Open-drain bus mode (``SDHC_BUSMODE_OPENDRAIN``) returns ``-ENOTSUP``
- A clock above ``max-bus-freq`` makes :c:func:`sdhc_set_io` return ``-EINVAL``
- Tuning in modes other than SDR104, HS200 and SDR50 (when the card needs it) returns ``-ENOTSUP``.
  HS400 has not been tested.

Usage
=====

Applications normally use the SD card subsystem or the disk access API
(``<zephyr/storage/disk_access.h>``) rather than the SDHC API directly. Disks are addressed by the
devicetree ``disk-name`` (for example ``emmc`` and ``sd``).

Use the ``disk-name`` of the devicetree child as the disk name. The disk is initialized once and
then addressed in sectors:

.. code-block:: c

   #include <zephyr/kernel.h>
   #include <zephyr/storage/disk_access.h>
   #include <zephyr/sys/printk.h>

   #define DISK_NAME "emmc"

   static uint8_t sector[512];

   int main(void)
   {
           uint32_t sector_count;
           uint32_t sector_size;
           int ret;

           ret = disk_access_init(DISK_NAME);
           if (ret != 0) {
                   printk("disk init failed: %d\n", ret);
                   return ret;
           }

           disk_access_ioctl(DISK_NAME, DISK_IOCTL_GET_SECTOR_COUNT, &sector_count);
           disk_access_ioctl(DISK_NAME, DISK_IOCTL_GET_SECTOR_SIZE, &sector_size);
           printk("%u sectors of %u bytes\n", sector_count, sector_size);

           if (sector_size > sizeof(sector)) {
                   return -ENOMEM;
           }

           ret = disk_access_read(DISK_NAME, sector, 0, 1);
           if (ret != 0) {
                   return ret;
           }

           return 0;
   }

Writes use :c:func:`disk_access_write` with the same arguments. To mount a file system on the disk
instead, see the filesystem sample below.

Enabling the Driver
-------------------

``CONFIG_SDHC_TI_AM654`` is enabled by default once a ``ti,am654-sdhci`` node has ``status =
"okay"``. The SD stack selects ``CONFIG_SDHC``. For the disk access API, add:

.. code-block:: kconfig

   CONFIG_DISK_ACCESS=y

``CONFIG_DISK_DRIVER_MMC`` (``zephyr,mmc-disk`` children) and ``CONFIG_DISK_DRIVER_SDMMC``
(``zephyr,sdmmc-disk`` children) default to ``y`` when the matching child is enabled.

The SoC devicetree defines the eMMC controller as follows:

.. code-block:: devicetree

   main_sdhci0: mmc@fa10000 {
           compatible = "ti,am654-sdhci";
           reg = <0xfa10000 DT_SIZE_K(4)>, <0xfa18000 DT_SIZE_K(1)>;
           reg-names = "host", "subsys";
           power-domains = <&main_mmcsd0_pd>;
           max-bus-freq = <DT_FREQ_M(200)>;
           min-bus-freq = <DT_FREQ_M(0)>;
           ti,is-embedded;
           ti,driver-strength-ohm = <50>;
           ti,clkbuf-sel = <0x7>;
           mmc-hs200-1_8v;
           ti,otap-del-sel-sdr12 = <0xf>;
           ti,otap-del-sel-sdr25 = <0xf>;
           ti,otap-del-sel-ddr50 = <0x15>;
           ti,otap-del-sel-hs200 = <0x6>;
           ti,itap-del-sel-legacy = <0x0>;
           ti,itap-del-sel-hs = <0x0>;
           ti,itap-del-sel-sdr12 = <0x0>;
           ti,itap-del-sel-sdr25 = <0x0>;
           ti,itap-del-sel-ddr50 = <0x2>;
   };

Enable the controller and attach a disk child in devicetree (or an overlay):

.. code-block:: devicetree

   &main_sdhci0 {
           status = "okay";

           mmc {
                   compatible = "zephyr,mmc-disk";
                   bus-width = <8>;
                   disk-name = "emmc";
           };
   };

Samples
-------

- :ws-doc:`Filesystems sample <zephyr/samples/subsys/fs/fs_sample/README.html>`
  (``samples/subsys/fs/fs_sample``)

Devicetree Bindings
===================

- :ws-doc:`ti,am654-sdhci <zephyr/build/dts/api/bindings/sdhc/ti,am654-sdhci.html>`
- :ws-doc:`zephyr,mmc-disk <zephyr/build/dts/api/bindings/sd/zephyr,mmc-disk.html>`
- :ws-doc:`zephyr,sdmmc-disk <zephyr/build/dts/api/bindings/sd/zephyr,sdmmc-disk.html>`
