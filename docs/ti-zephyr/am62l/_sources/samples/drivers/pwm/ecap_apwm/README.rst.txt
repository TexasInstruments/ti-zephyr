.. _am62l_ecap_apwm:

AM62L ECAP APWM Mode
#####################

Overview
********

This sample demonstrates the eCAP peripheral's Auxiliary PWM (APWM) mode on
the TI's SoC, in which the capture unit is repurposed as a simple PWM
output.

The sample configures ``main_ecap0`` (pin 24 of J2 on AM62L EVM) for a 1000 Hz,
50% duty cycle output using the standard :ws-doc:`PWM API <zephyr/hardware/peripherals/pwm.html#api-reference>`
(``pwm_set_dt``), leaves it running for 10 seconds, then drives the pulse width to zero
to stop toggling.

Requirements
************

This sample runs on the ``am62l_evm/am62l3/a53`` board. ``main_ecap0`` is
already enabled with its APWM output pin routed in the board's devicetree; a
board overlay adds the ``zephyr,user`` / ``pwms`` property the PWM API needs
to build a ``struct pwm_dt_spec`` for it.

Building and Running
*******************************

.. code-block:: console

   west build -p always -b am62l_evm/am62l3/a53 samples/drivers/pwm/ecap_apwm
   
Use the generated tiboot3.bin and tispl.bin to boot.

Sample Output
=============

.. code-block:: console

   ECAP APWM mode sample started
   Generating 1000 Hz, 50% duty cycle on ecap@23100000 channel 0 for 10 s
   ECAP APWM mode sample done
