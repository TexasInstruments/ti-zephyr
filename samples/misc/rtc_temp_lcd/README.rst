.. _rtc_temp_lcd:

RTC-Timestamped Temperature Logger with LCD Display
####################################################

Overview
********

This application combines four MSPM0 peripherals into a live temperature
monitor with no external components:

* **ADC** — reads the on-chip die temperature sensor every second using the
  internal 1.4 V bandgap VREF (3-sample average for noise reduction)
* **RTC** — provides an elapsed ``HH:MM:SS`` timestamp since boot
* **auxdisplay / segment LCD** — shows temperature live on the onboard
  14-segment glass display
* **HWINFO** — reports whether the previous reset was caused by a watchdog

On every boot the application lights all display segments for 600 ms
(hardware verification), reads the boot temperature, resets the RTC to
``00:00:00``, then enters a 1-second loop that updates the display and
prints a timestamped temperature line to the console.

WARMING and COOLING events are logged when the temperature changes by
more than 3 °C in one second. A running min/max/current summary is
printed every 10 seconds.

SDK analogs: ``adc12_internal_temp_sensor_rts``, ``rtc_alarm``

Hardware Requirements
*********************

No external components are needed. The application uses:

* On-chip ADC temperature sensor (channel 11, all MSPM0 variants)
* On-chip LFSS RTC (always-on VBAT domain — survives software resets)
* Onboard segment LCD glass

Supported Boards
****************

+----------------------+----------------------------+
| Board                | Display                    |
+======================+============================+
| ``lp_mspm0l2228``    | 6-digit 14-segment LCD     |
+----------------------+----------------------------+

Adding a new board requires a board overlay that defines three aliases:

.. code-block:: devicetree

   / {
       aliases {
           temp-adc    = &adc0;
           rtc         = &rtc;
           auxdisplay0 = &<your-lcd-node>;
       };
   };

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: ti-zephyr/applications/rtc_temp_lcd
   :board: lp_mspm0l2228
   :goals: build flash
   :compact:

Open a serial terminal at 115200 baud.

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0 ***
   TI MSPM0 RTC + Temperature + LCD Display
     ADC ch11 -> auxdisplay + UART
     RTC elapsed time: HH:MM:SS since boot
     Clean boot.

   Boot temp: 49.0 C  RTC started at 00:00:00

   Time          Temp(C)   Event
   --------------------------------------------
   [00:00:01]   49.0 C
   [00:00:03]   31.3 C    v COOLING
   [00:00:07]   49.0 C    ^ WARMING
     -- 10 s  min=31.3 C  max=49.0 C  cur=49.0 C --

RTC Behaviour
*************

The LFSS RTC is in the always-on VBAT domain and continues counting
across software resets. ``rtc_set_time()`` takes effect at the next
LFOSC tick boundary (~31 µs at 32768 Hz); the application waits 50 ms
after the call before reading back the new time.

Temperature Calibration
***********************

The conversion uses typical constants from TRM §16:

* ~2.7 mV/°C sensitivity
* 750 mV offset at 25 °C

For production accuracy, read the factory trim value from
``SYSCTL_TEMP_SENSE_CAL`` (``0x41C40170`` on G-series,
``0x41C00170`` on L-series) and substitute the device-specific
calibration coefficient and reference point.

References
**********

* :ref:`adc_api`
* :ref:`rtc_api`
* :ref:`auxdisplay_api`
* :ref:`hwinfo_api`
