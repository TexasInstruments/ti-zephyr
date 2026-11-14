.. _board-support-cc27:

Board Support
#############

Overview
********

CC27 is a family of TI SimpleLink wireless MCUs. The CC2745 and CC2755 devices
are Arm Cortex-M33 based, with an integrated 2.4 GHz transceiver supporting
Bluetooth® Low Energy and IEEE® 802.15.4. The CC2755 devices additionally
support Matter, Thread and Zigbee.

Supported Boards
****************

.. list-table::
   :header-rows: 1
   :widths: 20 20 40 20

   * - Board ID
     - Device
     - CPU
     - Product page
   * - ``lp_em_cc2745r10_q1``
     - CC2745R10-Q1 / CC2755R10
     - Arm Cortex-M33 @ 96 MHz
     - `CC2745R10-Q1 <https://www.ti.com/product/CC2745R10-Q1>`_
   * - ``lp_em_cc2755p10``
     - CC2755P10
     - Arm Cortex-M33 @ 96 MHz
     - `CC2755P10 <https://www.ti.com/product/CC2755P10>`_
   * - ``lp_em_cc2755p20``
     - CC2755P20
     - Arm Cortex-M33 @ 96 MHz
     - `CC2755P20 <https://www.ti.com/product/CC2755P20>`_

The ``lp_em_cc2745r10_q1`` board has two configurations: build for
``lp_em_cc2745r10_q1/cc2745r10_q1`` by default, or for
``lp_em_cc2745r10_q1/cc2755r10`` if a CC2755R10 device is mounted on the board.

Board Documentation
*******************

The hardware features, connections and flash/debug instructions of each board
are documented on its page in the main Zephyr documentation. Each link below
opens the matching section of that page.

.. list-table::
   :header-rows: 1
   :widths: 30 35 35

   * - Board ID
     - Board page
     - Hardware features
   * - ``lp_em_cc2745r10_q1``
     - :ws-doc:`CC2745R10-Q1 LaunchPad <zephyr/boards/ti/lp_em_cc2745r10_q1/doc/index.html>`
     - :ws-doc:`Supported features <zephyr/boards/ti/lp_em_cc2745r10_q1/doc/index.html#supported-features>`
   * - ``lp_em_cc2755p10``
     - :ws-doc:`CC2755P10 LaunchPad <zephyr/boards/ti/lp_em_cc2755p10/doc/index.html>`
     - :ws-doc:`Supported features <zephyr/boards/ti/lp_em_cc2755p10/doc/index.html#supported-features>`
   * - ``lp_em_cc2755p20``
     - :ws-doc:`CC2755P20 LaunchPad <zephyr/boards/ti/lp_em_cc2755p20/doc/index.html>`
     - :ws-doc:`Supported features <zephyr/boards/ti/lp_em_cc2755p20/doc/index.html#supported-features>`

Resources
*********

- `CC2745R10-Q1 product page <https://www.ti.com/product/CC2745R10-Q1>`_
- `CC2755P10 product page <https://www.ti.com/product/CC2755P10>`_
- `CC2755P20 product page <https://www.ti.com/product/CC2755P20>`_
- `LP-EM-CC2745R10-Q1 LaunchPad <https://www.ti.com/tool/LP-EM-CC2745R10-Q1>`_
