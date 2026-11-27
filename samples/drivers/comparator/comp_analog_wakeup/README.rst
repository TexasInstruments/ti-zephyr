.. zephyr:code-sample:: comp_analog_wakeup
   :name: Analog Comparator Wakeup

   Wake a device from a low-power sleep state using an analog comparator.

Overview
********

This sample demonstrates how to wake a microcontroller from a low-power sleep state using a
comparator.

The application sets up a rising-edge trigger callback on the comparator and blocks on a kernel
semaphore. When the target input voltage crosses the designated threshold, the comparator's hardware
interrupt fires, executes the callback, and releases the semaphore to wake the main thread.

Requirements
************

This sample requires compatible hardware and devicetree configuration.

Supported Hardware
==================

Currently, this sample explicitly supports the **LP-MSPM33C321A LaunchPad**.

* **Mechanism:** The peripheral's wakeup sequencer supports two primary modes:
  1. *Normal Mode:* The hardware automatically cycles through 4 VMON inputs.
  2. *Window Mode:* Comparing a single selected channel against an 8-bit DAC reference code.

* **Configuration:** This sample configures **VMON0** (**PA18**) against a DAC code of 192 ~(VREF * 3 / 4)
  in window mode.

Building, Flashing and Running
*******************************

.. zephyr-app-commands::
   :zephyr-app: samples/drivers/comparator/comp_analog_wakeup
   :board: lp_mspm33c321a
   :goals: build flash
   :compact:

Sample Output
=============

.. code-block:: console

        going to sleep
        woke up (count=1)
        going to sleep
