.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-samples:

TI Samples
##########

The TI Zephyr SDK provides out-of-box (OOB) samples targeting TI LaunchPad
boards. All samples follow the same directory structure as upstream Zephyr
samples and can be used as starting points for application development.

.. note::

   These samples supplement (and do not duplicate) the extensive sample
   library already available in ``zephyr/samples/`` after ``west update``.
   Upstream samples are fully supported on TI boards.

Basic Samples
*************

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Sample
     - Description
   * - :ref:`ti-sample-blinky`
     - LED blink at 1 Hz using GPIO API
   * - `Zephyr blinky <https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html>`_
     - LED blink (upstream Zephyr version)
   * - `hello_world <https://docs.zephyrproject.org/latest/samples/hello_world/README.html>`_
     - Serial hello world (upstream)

Driver Samples
**************

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Sample
     - Description
   * - :ref:`ti-sample-uart-echo`
     - UART interrupt-driven echo
   * - :ref:`ti-sample-adc-read`
     - ADC single-channel read (DT-based)
   * - `adc_dt <https://docs.zephyrproject.org/latest/samples/drivers/adc/adc_dt/README.html>`_
     - ADC multi-channel (upstream, full TI test coverage)
   * - `counter/alarm <https://docs.zephyrproject.org/latest/samples/drivers/counter/alarm/README.html>`_
     - Counter alarm with exponential backoff (upstream)

Supported Boards
================

All TI samples support the following boards:

* ``lp_mspm0g3519`` (primary test board)
* ``lp_mspm0g3507``
* ``lp_mspm0l1306``
* ``lp_mspm0l2228``
* ``lp_mspm33c321a`` (when SoC support is added)

.. toctree::
   :hidden:

   blinky
   uart_echo
   adc_read
