.. _washing_machine_app:

TI MSPM33 LVGL Washing Machine UI
##################################

Overview
********

A four-screen LVGL washing-machine user interface targeting the
LP-MSPM33C321A LaunchPad with the Waveshare 3.5" RPi LCD (F) display shield.

Screens:

* **Home** — program selector with wash cycle overview.
* **Cycle** — active cycle progress with phase steps and drum animation.
* **Setup** — temperature, spin speed, and wash option configuration.
* **Service** — diagnostic I/O rows and metric readouts.

Hardware requirements
*********************

* **LP-MSPM33C321A** LaunchPad development kit.
* **Waveshare 3.5" RPi LCD (F)** — ST7796S SPI display (320×480) and GT911
  capacitive touch, connected via the LP-MSPM33C321A BoosterPack headers.

Building and Running
********************

.. code-block:: console

   west build -b lp_mspm33c321a applications/washing_machine_app

The CMakeLists.txt defaults ``SHIELD`` to ``waveshare_3_5_rpi_lcd_f`` when not
specified, so no ``--shield`` flag is required.

To pass the shield explicitly:

.. code-block:: console

   west build -b lp_mspm33c321a applications/washing_machine_app \
     -- -DSHIELD=waveshare_3_5_rpi_lcd_f

Flash with:

.. code-block:: console

   west flash

UI Architecture
***************

The ``ui/`` directory is the vendored LVGL Editor project tree containing:

* ``washing_machine.c/.h`` — top-level UI init entry point (``washing_machine_init``).
* ``screens/`` — four screen definitions (home, cycle, setup, service).
* ``components/`` — reusable LVGL component widgets.
* ``custom/`` — application logic (``wash_logic.c``) and hardware simulator (``wash_sim.c``).
* ``fonts/`` — Gabarito font in multiple weights, pre-rendered as C arrays.
* ``images/`` — UI assets pre-rendered as C arrays.
* ``file_list_gen.cmake`` — auto-generated source list included by CMakeLists.txt.
* ``user_config.cmake`` — LVGL configuration options.

Shield overlay split
********************

The Waveshare shield lives in the Zephyr tree under
``boards/shields/waveshare_3_5_rpi_lcd_f/``. The board-specific pin
assignments for LP-MSPM33C321A are in:

* ``boards/lp_mspm33c321a.overlay`` — SPI/I2C pin routing, GT911 node.
* ``boards/lp_mspm33c321a.conf`` — LVGL VDB sizing, flush thread, GT911 poll.

The application's own ``boards/lp_mspm33c321a.overlay`` adds the landscape
rotation (``madctl=0xE8``, 480×320 dimensions) and touch-axis correction on
top of the shield's portrait defaults.
