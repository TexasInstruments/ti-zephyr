.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. zephyr:code-sample:: ti-adc-read
   :name: TI ADC Read
   :relevant-api: adc_interface

   Read ADC channel 0 every 500 ms on TI LaunchPad boards.

Overview
********

Polling ADC sample for TI LaunchPad boards. Reads ADC channel 0 at
500 ms intervals and prints the raw value to the console.

Demonstrates:

* Obtaining the ADC device from devicetree using the ``adc0`` alias
* Configuring a channel with :c:func:`adc_channel_setup`
* Reading a single sample with :c:func:`adc_read`
* Printing raw ADC counts

Requirements
************

A TI LaunchPad board with an ADC peripheral and ``adc0`` devicetree alias.
On the LP-MSPM0G3519 the ADC input is connected to the external OPA2365
buffer (RC filter, unpopulated by default) or directly to the analog pin.

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: zephyr_ti_sdk_v1.0.0/samples/drivers/adc/adc_read
   :board: lp_mspm0g3519
   :goals: build flash
   :compact:

Sample Output
=============

.. code-block:: none

   ADC ch0: 2048
   ADC ch0: 2051
   ADC ch0: 2047

.. note::

   For more comprehensive ADC testing (multi-channel, sequence, error cases),
   use the upstream Zephyr samples: ``samples/drivers/adc/adc_dt`` and
   ``samples/drivers/adc/adc_sequence``.

References
**********

- :ref:`adc_api`
- :dtcompatible:`ti,mspm0-adc`
- `LP-MSPM0G3519 product page <https://www.ti.com/tool/LP-MSPM0G3519>`_
