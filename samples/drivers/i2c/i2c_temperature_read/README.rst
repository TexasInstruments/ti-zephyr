.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _i2c_temperature_read:

I2C Temperature Read
####################

Overview
********

Reads the temperature from a TMP10x-compatible I2C temperature sensor using
raw I2C transfers (``i2c_transfer()``) and prints the result in degrees
Celsius.

The test sequence is:

1. Configure the ``main_i2c1`` controller as an I2C controller at fast speed
   (400 kHz) using ``i2c_configure()``.
2. Check that the I2C controller is ready.
3. Write the temperature result register address (``0x00``) to the sensor at
   I2C address ``0x48``.
4. Read the 2-byte temperature result 10 times.
5. Convert each result: the 12-bit value is right-aligned (the 4 LSBs of the
   16-bit result are zero), sign-extended if negative, and scaled by
   1/16 °C per LSB.
6. Log each sample and, if all reads succeeded, report that the test passed.

The sample stops and logs an error at the first step that fails. If any
temperature read fails, the remaining reads are skipped and the pass message
is not printed.

Requirements
************

- A board with an I2C controller labelled ``main_i2c1`` in devicetree.
- A TMP10x-compatible temperature sensor connected to ``main_i2c1`` at I2C
  address ``0x48``.
- ``CONFIG_CBPRINTF_FP_SUPPORT=y`` is needed for the temperature to be
  printed as a floating-point value; without it the log shows a placeholder
  instead of the number.

Supported boards
****************

- ``am62l_evm/am62l3/a53``

Building and Running
********************

.. code-block:: console

   west build -p always -b am62l_evm/am62l3/a53 ti-zephyr/samples/drivers/i2c/i2c_temperature_read
   west flash

Sample Output
*************

On success:

.. code-block:: console

   [00:00:00.000,000] <dbg> main: main: I2C temperature read
   [00:00:00.001,000] <dbg> main: main: [I2C] Sample 1: 25.375000 (celcius)
   [00:00:00.002,000] <dbg> main: main: [I2C] Sample 2: 25.375000 (celcius)
   ...
   [00:00:00.010,000] <dbg> main: main: [I2C] Sample 10: 25.437500 (celcius)
   [00:00:00.010,000] <dbg> main: main: All test passed

On failure, an error message is logged and the sample exits, for example:

.. code-block:: console

   <err> main: Device register not accessible
   <err> main: Failed to read temperature
