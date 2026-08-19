.. _temp_pwm_supervisor:

Temperature-Controlled PWM with Watchdog Supervision
#####################################################

Overview
********

This application demonstrates a five-peripheral control loop on MSPM0
microcontrollers using only Zephyr RTOS APIs:

* **ADC** — reads the on-chip die temperature sensor (channel 11, internal
  1.4 V VREF) once per second
* **Counter** — provides a precise 1-second tick via ``counter_set_top_value``
* **PWM** — drives the on-board LED at a duty cycle proportional to temperature
  (center-aligned, 1 kHz)
* **WWDT** — window watchdog supervises the control loop; if the loop stalls the
  device resets
* **HWINFO** — reports on every boot whether the previous reset was caused by a
  watchdog violation

The pattern — *sense → compute → actuate → supervise* — mirrors standard
industrial and automotive firmware practice. No upstream Zephyr sample combines
all five peripherals in a single supervised control loop.

SDK analogues: ``adc12_internal_temp_sensor_rts``,
``timx_timer_mode_pwm_center_stop``, ``wwdt_window_mode_periodic_reset``

Hardware Requirements
*********************

No external components are needed. The application uses:

* On-chip ADC temperature sensor (channel 11, all MSPM0 variants)
* On-board LED connected to a PWM-capable pin (TIMA0 CC0)

Supported Boards
****************

+------------------------------+------+--------------+---------+--------+
| Board                        | ADC  | Counter      | PWM pin | WDT    |
+==============================+======+==============+=========+========+
| ``lp_mspm0l2228``            | adc0 | counterg0    | PA0     | wdt0   |
+------------------------------+------+--------------+---------+--------+
| ``lp_mspm0g3507``            | adc0 | counterg0    | PA0     | wdt0   |
+------------------------------+------+--------------+---------+--------+
| ``lp_mspm0g3519``            | adc0 | counterg0    | PA0     | wdt0   |
+------------------------------+------+--------------+---------+--------+

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: ti-zephyr/applications/temp_pwm_supervisor
   :board: lp_mspm0l2228
   :goals: build flash
   :compact:

Flash with TI OpenOCD::

   west flash --runner openocd \
       --openocd <path-to-ti-openocd>/src/openocd \
       --openocd-search <path-to-ti-openocd>/tcl

Open a serial terminal at 115200 baud.

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0 ***
   TI MSPM0 Temperature-Controlled PWM with WDT Supervision
     ADC ch11 (internal temp) → PWM LED brightness
     WWDT window [500..2000 ms] supervises control loop
     Clean boot.

   Counter: 15625 Hz → period = 15625 ticks (1 s)
   WWDT armed: window [500..2000 ms]

   Feed   Temp(C)   Duty%   PWM ns
   ------------------------------------------
   1       49.0 C     47%    470000
   2       49.0 C     47%    470000
   3       49.0 C     47%    470000

The LED brightness changes as the die temperature changes. Warming the MCU
(e.g., a heat source near the package) increases brightness; allowing it to
cool reduces brightness.

Temperature-to-Duty Mapping
****************************

The linear mapping is:

* Below ``TEMP_LOW_C`` (35 °C): ``MIN_DUTY_PCT`` (5%)
* Above ``TEMP_HIGH_C`` (65 °C): ``MAX_DUTY_PCT`` (95%)
* Between: linearly interpolated

These thresholds are defined as compile-time constants in ``src/main.c``
and can be adjusted for different thermal environments.

Watchdog Supervision
********************

The WWDT window is configured as [500 ms .. 2000 ms]. The control loop fires
every 1 second — safely within the open window. If the counter stalls or the
ADC read hangs, the watchdog fires and resets the device. On the next boot,
``hwinfo_get_reset_cause`` detects ``RESET_WATCHDOG`` and prints a safety-reset
notice.

References
**********

* MSPM0 Technical Reference Manual — ADC12 Temperature Sensor (§16)
* MSPM0 Technical Reference Manual — Window Watchdog Timer (§18)
* :ref:`adc_api`
* :ref:`pwm_api`
* :ref:`counter_api`
* :ref:`watchdog_api`
* :ref:`hwinfo_api`
