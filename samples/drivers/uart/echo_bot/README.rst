.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. zephyr:code-sample:: ti-uart-echo
   :name: TI UART Echo
   :relevant-api: uart_interface

   Echo characters received over UART on TI LaunchPad boards.

Overview
********

Interrupt-driven UART echo sample for TI LaunchPad boards. Every character
received on the console UART is immediately echoed back.

Demonstrates:

* Getting the console UART device from devicetree (``zephyr,console`` chosen node)
* Setting up an interrupt-driven UART callback
* Reading and writing bytes in the IRQ handler

Requirements
************

A TI LaunchPad board with UART connected to USB-to-serial (standard on all
LaunchPads via the XDS110 virtual COM port).

Connect a serial terminal (115200 8N1) to the LaunchPad's COM port.

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/drivers/uart/echo_bot
   :board: lp_mspm0g3519
   :goals: build flash
   :compact:

Sample Output
=============

.. code-block:: none

   UART echo ready
   Hello!        ← typed by user
   Hello!        ← echoed back

References
**********

- :ref:`uart_api`
- :dtcompatible:`ti,mspm0-uart`
- `LP-MSPM0G3519 product page <https://www.ti.com/tool/LP-MSPM0G3519>`_
