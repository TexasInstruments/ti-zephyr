.. ti-socs: mspm0 mspm33 am13

.. _ti-zephyr-component-crc:

CRC
###

Overview
********

The CRC peripheral computes a checksum over a block of data in hardware. The
driver implements Zephyr's CRC driver API (``<zephyr/drivers/crc.h>``): a
calculation is started with :c:func:`crc_begin`, fed with one or more calls to
:c:func:`crc_update`, and completed with :c:func:`crc_finish`.

.. only:: mspm0

   There are two variants of the peripheral, each with its own devicetree
   compatible and Kconfig symbol:

   .. list-table::
      :header-rows: 1
      :widths: 20 20 30 30

      * - Variant
        - Compatible
        - Kconfig
        - Boards
      * - CRC
        - ``ti,mspm0-crc``
        - ``CONFIG_CRC_TI_MSPM0``
        - ``lp_mspm0g3507``
      * - CRC-P
        - ``ti,mspm0-crcp``
        - ``CONFIG_CRC_TI_MSPM0_CRCP``
        - ``lp_mspm0g3519``, ``lp_mspm0l2228``

   CRC has a fixed set of polynomials (CRC16-CCITT and CRC32-IEEE). CRC-P has
   a programmable polynomial register, which lets it support more 16-bit and
   32-bit algorithms.

.. only:: mspm33 or am13

   The |soc_name| device has the CRC-P variant of the peripheral
   (``ti,mspm0-crcp``, ``CONFIG_CRC_TI_MSPM0_CRCP``). It has a programmable
   polynomial register, which lets it support several 16-bit and 32-bit
   algorithms.

Features Supported
******************

- Hardware CRC calculation through the Zephyr CRC API
- Incremental calculation: the data can be passed in several
  :c:func:`crc_update` calls, from non-contiguous buffers
- Configurable initial seed value
- Independent control of input and output bit reversal
  (``CRC_FLAG_REVERSE_INPUT`` and ``CRC_FLAG_REVERSE_OUTPUT`` can be set
  separately)

.. only:: mspm0

   - CRC: CRC16-CCITT and CRC32-IEEE
   - CRC-P: CRC16, CRC16-ANSI, CRC16-CCITT, CRC16-ITU-T, CRC32-C, CRC32-IEEE,
     CRC32-MPEG2 and CRC32-K/4.2

.. only:: mspm33 or am13

   - CRC16, CRC16-ANSI, CRC16-CCITT, CRC16-ITU-T, CRC32-C, CRC32-IEEE,
     CRC32-MPEG2 and CRC32-K/4.2

Features NOT Supported
***********************

- DMA based data transfer. The CPU copies the data into the CRC input
  registers during :c:func:`crc_update`.
- Algorithms other than the ones listed above (for example CRC8).
  :c:func:`crc_begin` returns ``-EINVAL`` for them.

.. only:: mspm0

   - CRC: algorithms other than CRC16-CCITT and CRC32-IEEE, such as CRC32-C.

Usage
*****

Enabling the Driver
===================

Enable the CRC node in the board devicetree (or an application overlay) and
the CRC subsystem in ``prj.conf``. The driver is selected automatically once
an enabled CRC node is present. The LaunchPad board devicetrees already enable
the node and choose it as ``zephyr,crc``, so no overlay is needed to run the
sample.

.. code-block:: devicetree

   / {
           chosen {
                   zephyr,crc = &crc;
           };
   };

   &crc {
           status = "okay";
   };

.. code-block:: kconfig

   CONFIG_CRC=y

Devicetree Bindings
===================

The node properties are described in
:ws-doc:`ti,mspm0-crc <zephyr/build/dts/api/bindings/crc/ti,mspm0-crc.html>`
and
:ws-doc:`ti,mspm0-crcp <zephyr/build/dts/api/bindings/crc/ti,mspm0-crcp.html>`
devicetree binding.

.. only:: mspm0

   The MSPM0 SoC devicetrees describe the plain CRC (``ti,mspm0-crc``). A board
   whose device has a CRC-P peripheral selects the CRC-P driver by overriding
   the compatible, as ``lp_mspm0g3519`` does:

   .. code-block:: devicetree

      &crc {
              compatible = "ti,mspm0-crcp";
              status = "okay";
      };

.. only:: mspm0

   CRC Algorithms
   ==============

   For ``ti,mspm0-crc``:

   .. list-table::
      :header-rows: 1
      :widths: 25 25 25 25

      * - ``type``
        - ``polynomial``
        - Polynomial value
        - ``seed``
      * - ``CRC16_CCITT``
        - ``CRC16_CCITT_POLY``
        - ``0x1021``
        - ``CRC16_CCITT_INIT_VAL``
      * - ``CRC32_IEEE``
        - ``CRC32_IEEE_POLY``
        - ``0x04C11DB7``
        - ``CRC32_IEEE_INIT_VAL``

CRC-P Algorithms
================

.. only:: mspm0

   For ``ti,mspm0-crcp``:

.. list-table::
   :header-rows: 1
   :widths: 25 25 25 25

   * - ``type``
     - ``polynomial``
     - Polynomial value
     - ``seed``
   * - ``CRC16``, ``CRC16_ANSI``
     - ``CRC16_POLY``
     - ``0x8005``
     - ``CRC16_INIT_VAL``, ``CRC16_ANSI_INIT_VAL``
   * - ``CRC16_CCITT``, ``CRC16_ITU_T``
     - ``CRC16_CCITT_POLY``
     - ``0x1021``
     - ``CRC16_CCITT_INIT_VAL``, ``CRC16_ITU_T_INIT_VAL``
   * - ``CRC32_C``
     - ``CRC32C_POLY``
     - ``0x1EDC6F41``
     - ``CRC32_C_INIT_VAL``
   * - ``CRC32_IEEE``, ``CRC32_MPEG2``
     - ``CRC32_IEEE_POLY``
     - ``0x04C11DB7``
     - ``CRC32_IEEE_INIT_VAL``, ``CRC32_MPEG2_INIT_VAL``
   * - ``CRC32_K_4_2``
     - ``CRC32K_4_2_POLY``
     - ``0x93A409EB``
     - ``CRC32_K_4_2_INIT_VAL``

For ``CRC32_IEEE`` the driver inverts the hardware result in
:c:func:`crc_finish`, as required by the IEEE 802.3 definition of the CRC.

Samples and Tests
*****************

Samples
=======

- :ws-doc:`CRC Sample <zephyr/samples/drivers/crc/README.html>`

  - Computes one CRC over a fixed 8-byte buffer, logs the result and verifies
    it against the expected value.
  - The algorithm is selected with ``SAMPLE_CRC_VARIANT_*`` in the sample
    Kconfig; the default is CRC16-CCITT, and ``SAMPLE_CRC_VARIANT_CRC32_IEEE``
    is also available.

Tests
=====

- ``drivers.crc`` (``tests/drivers/crc``) - Computes CRCs with the algorithms
  the driver supports (the others are skipped), including inputs split across
  several :c:func:`crc_update` calls, discontinuous buffers and access from
  more than one thread.

Zephyr Documentation
********************

:ws-doc:`CRC Zephyr Documentation <zephyr/hardware/peripherals/crc.html>`
