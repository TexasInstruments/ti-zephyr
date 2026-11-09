.. _ti_smart_home:

TI Smart Home LVGL UI
######################

Overview
********

A smart-home dashboard user interface built with LVGL 9.5, targeting the
AM62L EVM with the Raspberry Pi Touch Display 2 (7", ILI9881C MIPI-DSI
panel) and GT911 capacitive touch controller.

The UI has four screens, reachable from the navigation bar:

* **Home** — hero image, weather summary, and quick-access cards for the
  Smart Home, EV Charging, and Messages screens.
* **Smart Home** — thermostat arc control for the temperature setpoint,
  kettle/vacuum/wifi toggle switches, and a music player card.
* **EV Charging** — state-of-charge bar and a range/power chart for the
  vehicle.
* **Messages** — contact list and message list views.

All data shown (temperature, EV state of charge, contacts, music track,
etc.) is static demo data initialized at startup — this application does
not read live sensors. It is intended to showcase LVGL widgets, styling,
and screen navigation on TI hardware rather than to model real smart-home
telemetry.

Hardware requirements
**********************

* **AM62L TMDS62LEVM** evaluation module (EVM).
* **Raspberry Pi Touch Display 2** (7") — ILI9881C MIPI-DSI panel (720x1280)
  with GT911 capacitive touch over I2C.

Building and running
*********************

From a west workspace with ti-zephyr as an extra module::

  west build -p -b am62l_evm/am62l3/a53 applications/ti_smart_home

Run the generated binaries tiboot3.bin and tispl.bin:

Using USB-DFU::
  sudo dfu-util -R -a bootloader -D build/zephyr/tiboot3.bin
  sudo dfu-util -R -a bootloader -D build/zephyr/tispl.bin

Using SD::
  Keep the binaries tiboot3.bin and tispl.bin in the SD card's boot partition.

The home screen appears after reset. Tap the navigation bar to switch
between the Home, Smart Home, EV Charging, and Messages screens.
