.. zephyr:code-sample:: simple_gui_demo
   :name: Simple GUI demo
   :relevant-api: gpio_interface

   Minimal reference for the gui_composer module -- counters and two
   switches streamed over UART, no sensors.

Overview
********

Ports msp-sdk's ``gc_simple_messagepack``/``gc_simple_json`` reference
examples: proves the transport, the RX-dispatch table, and a periodic
TX update all work, without any sensor hardware.

- ``main.c`` owns the onboard LED and the ``bEnable`` RX command,
  which both toggles that LED (the pass criterion for the middleware
  port's RX round trip) and enables/disables ``counters.c``'s
  periodic update.
- ``counters.c`` sends four counters (``c1``/``c2``/``c3``/``c4``, an
  8-/16-/32-bit and a Q8 fixed-point value) via a periodic ``k_timer``,
  and reads SW1/SW2 button presses to bump the 8-bit and 16-bit
  counters directly. The ``u16Data`` RX command sets the Q8 counter's
  per-tick increment.

No polling loop otherwise -- every counter update and button press is
event-driven. See ``applications/gui_composer_demo`` for sensor-backed modes
built on the same transport.

Configuring
***********

``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS`` (default ``y``) gates
``counters.c`` entirely -- disabling it drops the whole file from the
build, leaving just the ``bEnable``-toggles-the-LED acceptance test.
While enabled, its timing and step sizes are their own Kconfig ints,
same pattern as ``gui_composer_demo``:

- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS`` (default 500)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U8_STEP`` (default 50)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U16_STEP`` (default 5000)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U32_STEP`` (default 10000)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_Q8_DEFAULT_INCREMENT``
  (default 0)

Requirements
************

- A TI LP-MSPM0G3507, LP-MSPM0G3519, or LP-MSPM0L2228 LaunchPad (the
  three boards with board overlays in this sample). GUI Composer runs
  on UART0 -- the onboard XDS110 debug probe's own USB-serial
  backchannel, already configured at the board level, no extra wiring
  needed -- on every one of them. The console driver is disabled on
  all three (see each board's own ``boards/<board>.conf``) so it
  doesn't fight over the same wire; **printf() output is unavailable
  on every board this sample targets** as a result.
- The GUI Composer web app at ``dev.ti.com/gc`` (Chrome; uses the Web
  Serial API, hence the browser requirement), pointed at the COM port
  the XDS110's backchannel registers, 115200 baud on every board (the
  board-level default for UART0 on all three).

Building and Running
*********************

.. zephyr-app-commands::
   :zephyr-app: applications/simple_gui_demo
   :board: lp_mspm0g3507
   :goals: build flash

Sample Output
*************

No console output on any board target (see Requirements above) -- the
only observable behavior is over GUI Composer's own UART0 link.

With GUI Composer connected: flip the widget bound to ``bEnable`` --
the board's onboard LED follows it immediately, and ``c1``/``c2``/
``c3`` (and ``c4`` while ``bEnable`` is set) start updating every
``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS`` (500ms default).
Press SW1/SW2 on the board to bump ``c1``/``c2`` directly, and set the
widget bound to ``u16Data`` to change how fast ``c4`` moves.
