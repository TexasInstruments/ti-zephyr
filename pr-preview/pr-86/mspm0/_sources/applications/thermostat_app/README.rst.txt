.. _ti_thermostat:

TI Thermostat LVGL UI
#####################

Overview
********

A smart-thermostat user interface built with LVGL 9.x, targeting the
LP-MSPM0G3519 LaunchPad with the Waveshare 3.5" RPi LCD (F) display and the
BP-BASSENSORSMKII BoosterPack sensors plug-in module.

The UI has two screens:

* **Home** — arc dial for temperature setpoint, heating/cooling mode pills,
  current temperature and humidity from the HDC2080 sensor.
* **Schedule** — weekly setpoint chart (``wd_schedule_chart`` widget) with
  day-selector chips.

Live data is fed by:

* **HDC2080** — temperature and humidity at 1 Hz via I2C2 (from the
  BP-BASSENSORSMKII BoosterPack).
* **MSPM0 RTC** — current time displayed in the top bar.

Hardware requirements
*********************

* **LP-MSPM0G3519** LaunchPad development kit.
* **Waveshare 3.5" RPi LCD (F)** — ST7796S SPI display (320×480) and
  GT911 capacitive touch over I2C.
* **BP-BASSENSORSMKII** BoosterPack — provides the HDC2080
  temperature/humidity sensor used for live ambient readings.

Wiring — LP-MSPM0G3519
======================

Waveshare 3.5" RPi LCD (F)
---------------------------

Connect the panel's 40-pin RPi header to the LP-MSPM0G3519 as follows.
The display is powered from 3.3V.

+--------------------+------------------------------------------+
| Signal             | LP-MSPM0G3519 Pin                        |
+====================+==========================================+
| VCC                | 3.3V                                     |
+--------------------+------------------------------------------+
| GND                | GND                                      |
+--------------------+------------------------------------------+
| Backlight EN       | PA16 (3.3V / GPIO active-high)           |
+--------------------+------------------------------------------+
| SPI MOSI           | PB22 (SPI1 PICO)                         |
+--------------------+------------------------------------------+
| SPI CLK            | PB23 (SPI1 SCLK)                         |
+--------------------+------------------------------------------+
| SPI CS             | PA18 (GPIO, active-low)                  |
+--------------------+------------------------------------------+
| SPI MISO (opt.)    | PB7  (SPI1 POCI)                         |
+--------------------+------------------------------------------+
| ST7796S D/C        | PB1  (GPIO, active-high)                 |
+--------------------+------------------------------------------+
| ST7796S RST        | PB18 (GPIO, active-low)                  |
+--------------------+------------------------------------------+
| GT911 SDA (I2C0)   | PA0  (pull-up via jumper J16)            |
+--------------------+------------------------------------------+
| GT911 SCL (I2C0)   | PA1  (pull-up via jumper J17)            |
+--------------------+------------------------------------------+
| GT911 INT          | PB28 (GPIO, active-high)                 |
+--------------------+------------------------------------------+
| GT911 RST          | PB20 (GPIO, active-low)                  |
+--------------------+------------------------------------------+

BP-BASSENSORSMKII BoosterPack
------------------------------

The HDC2080 sensor sits at I2C address ``0x40`` on the BoosterPack.
Connect the BoosterPack to the LP-MSPM0G3519 BoosterPack header and
route I2C2 (PC2/PC3) to the BoosterPack I2C lines.

+--------------------+------------------------------------------+
| Signal             | LP-MSPM0G3519 Pin                        |
+====================+==========================================+
| HDC2080 SDA (I2C2) | PC3                                      |
+--------------------+------------------------------------------+
| HDC2080 SCL (I2C2) | PC2                                      |
+--------------------+------------------------------------------+
| HDC2080 INT        | PA27 (GPIO, active-low)                  |
+--------------------+------------------------------------------+
| 3.3V               | 3.3V                                     |
+--------------------+------------------------------------------+
| GND                | GND                                      |
+--------------------+------------------------------------------+

More information on the BP-BASSENSORSMKII BoosterPack can be found at the
`BP-BASSENSORSMKII product page`_.

Building and running
********************

From a west workspace with ti-zephyr as an extra module::

  west build -b lp_mspm0g3519 --shield waveshare_3_5_rpi_lcd_f applications/ti_thermostat

Flash and run::

  west flash

The home screen appears after reset showing live temperature/humidity and
the current time. Tap the tab bar at the bottom to switch to the schedule
screen.

References
**********

.. target-notes::

.. _BP-BASSENSORSMKII product page:
   https://www.ti.com/tool/BP-BASSENSORSMKII
