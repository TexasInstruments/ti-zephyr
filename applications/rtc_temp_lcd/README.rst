.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _rtc_temp_lcd:

RTC Temperature LCD — Multi-State Monitor
##########################################

Overview
********

Multi-state temperature monitor for the LP-MSPM0L2228 using five
on-chip peripherals and no external components:

* **ADC** — samples the die temperature sensor every second (internal
  1.4 V bandgap VREF, 3-sample average)
* **RTC** — tracks date and time (seeded from build timestamp at boot)
* **auxdisplay / segment LCD** — 6-digit 14-segment onboard glass
* **GPIO** — S2 (PA18) and S3 (PB8) push-buttons for navigation
* **HWINFO** — detects watchdog reset at boot

Three display states are cycled with the buttons:

+------------+-----------------------+-------------------------------+
| State      | LCD shows             | Notes                         |
+============+=======================+===============================+
| ``TEMP``   | ``  49.0 °C``         | °C indicator (COL4) lit       |
+------------+-----------------------+-------------------------------+
| ``TIME``   | ``HH:MM:SS``          | Colon decorators (COL2/COL3)  |
+------------+-----------------------+-------------------------------+
| ``DATE``   | ``DD.MM.YY``          | Dot decorators (P-dots)       |
+------------+-----------------------+-------------------------------+

* **S2 (sw0, PA18)** — step backward through states
* **S3 (sw1, PB8)** — step forward through states
* Auto-returns to ``TEMP`` after 10 s of inactivity

The application runs two threads:

* **adc_thread** — reads temperature every 1 s, logs timestamped
  readings to UART with WARMING/COOLING events (>3 °C change) and a
  min/max/current summary every 10 s
* **main** — display loop at 250 ms refresh; owns the LCD

SDK analogs: ``adc12_internal_temp_sensor_rts``, ``rtc_alarm``

Hardware Requirements
*********************

No external components needed. Uses:

* On-chip ADC temperature sensor (channel 11, all MSPM0 variants)
* On-chip LFSS RTC (always-on VBAT domain — survives software resets)
* Onboard 6-digit 14-segment LCD glass
* S2 (PA18) and S3 (PB8) onboard push-buttons

Supported Boards
****************

+----------------------+-----------------------------------+
| Board                | Display                           |
+======================+===================================+
| ``lp_mspm0l2228``    | 6-digit 14-segment LCD            |
+----------------------+-----------------------------------+

Adding a new board requires a board overlay defining five aliases:

.. code-block:: devicetree

   / {
       aliases {
           temp-adc    = &adc0;
           rtc         = &rtc;
           auxdisplay0 = &<your-lcd-node>;
           sw0         = &<backward-button>;
           sw1         = &<forward-button>;
       };
   };

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: ti-zephyr/applications/rtc_temp_lcd
   :board: lp_mspm0l2228
   :goals: build flash
   :compact:

Open a serial terminal at 115200 baud. Flash immediately after build
for best RTC timestamp accuracy.

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0 ***
   TI MSPM0 RTC + Temperature + LCD
     S2 (PB8) = forward   S1 (PA18) = backward
     TEMP -> TIME -> DATE  (auto-return in 10 s)

   Boot temp: 31.3 °C  16/10/2026 09:30:00

   Time          Temp(°C)   Event
   --------------------------------------------
   [09:30:01]  31.3 °C
   [09:30:02]  49.0 °C    ^ WARMING
   [09:30:05]  31.4 °C    v COOLING
   >> TIME
   >> DATE
   >> TEMP
     -- 10 s  min=31.3  max=49.0  cur=31.4 °C --

RTC Behaviour
*************

The LFSS RTC is in the always-on VBAT domain and survives software
resets. The RTC is seeded from the build Unix timestamp (UTC, from CMake
``string(TIMESTAMP)``) at every cold boot. The timestamp is taken when CMake
configures the project, so re-run CMake (e.g. ``west build -p``) for a fresh
value. Flash immediately after building for
best accuracy.

``rtc_set_time()`` takes effect at the next LFOSC tick boundary
(~31 µs at 32768 Hz); the application waits 50 ms after the call
before reading back the new time.

RTC Crystal Calibration
***********************

The driver supports ``CONFIG_RTC_CALIBRATION``. Adjust the
``RTC_CAL_PPB`` constant in ``src/main.c`` to compensate for LFOSC
drift:

.. code-block:: c

   /* Example: RTC gains 1 s/day → apply −11574 PPB */
   #define RTC_CAL_PPB  -11574

Temperature Calibration
***********************

The conversion uses typical constants from TRM §16:

* ~2.7 mV/°C sensitivity
* 750 mV offset at 25 °C

For production accuracy, substitute device-specific factory trim values
from ``SYSCTL_TEMP_SENSE_CAL`` (``0x41C40170`` on G-series,
``0x41C00170`` on L-series).

References
**********

* :ref:`adc_api`
* :ref:`rtc_api`
* :ref:`auxdisplay_api`
* :ref:`gpio_api`
* :ref:`hwinfo_api`
