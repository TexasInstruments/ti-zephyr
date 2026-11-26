.. _motor_control_app:

TI Motor Control LVGL UI
########################

Overview
********

A four-screen LVGL 9.x motor-control user interface targeting the LP-AM13E230
LaunchPad with the Waveshare 3.5" RPi LCD (F) display shield.

Screens:

* **Run** — start/stop, direction, speed setpoint dial and live RPM odometer.
* **Trend** — live chart of speed, current and temperature over time.
* **Faults** — fault log table with clear action.
* **Config** — drive configuration (current limit, ramp rate, etc.).
                                                                                               
Hardware requirements
*********************

* **LP-AM13E230** LaunchPad development kit.
* **Waveshare 3.5" RPi LCD (F)** — ST7796S SPI display (320×480) and GT911
  capacitive touch, connected via the LP-AM13E230 BoosterPack headers.

Wiring (BoosterPack header → AM13E23019 pin):

+------------------+--------+--------+----------------+
| Signal           | RPi pin| Header | AM13 pin       |
+==================+========+========+================+
| SPI MOSI         | 19     | J2-15  | PB10 (UC3 TX)  |
+------------------+--------+--------+----------------+
| SPI SCLK         | 23     | J1-7   | PB5  (UC3 SCLK)|
+------------------+--------+--------+----------------+
| SPI CS           | 24     | J2-19  | PA21 (GPIO CS) |
+------------------+--------+--------+----------------+
| ST7796S DC       | 15     | J4-35  | PA25           |
+------------------+--------+--------+----------------+
| ST7796S RST      | 13     | J3-26  | PA17           |
+------------------+--------+--------+----------------+
| Backlight EN     | 12     | J3-24  | PA18           |
+------------------+--------+--------+----------------+
| GT911 SDA        | 3      | J1-10  | PA22 (UC2 SDA) |
+------------------+--------+--------+----------------+
| GT911 SCL        | 5      | J1-9   | PA23 (UC2 SCL) |
+------------------+--------+--------+----------------+
| GT911 INT        | 7      | J2-11  | PB11           |
+------------------+--------+--------+----------------+
| GT911 RST        | 11     | J3-23  | PA16           |
+------------------+--------+--------+----------------+

Jumpers J17 and J18 must be installed (default) for I2C pull-ups.

Building
********

From the Zephyr workspace root:

.. code-block:: console

   west build -b lp_am13e230 applications/motor_control_app \
       --shield waveshare_3_5_rpi_lcd_f

The shield is mandatory; the build will fail without it (display and touch
nodes are defined by the shield overlay).

Flashing
********

.. code-block:: console

   west flash
