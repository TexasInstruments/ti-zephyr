.. zephyr:code-sample:: gui_composer_demo
   :name: LP-MSPM0G3507 Out-of-Box demo
   :relevant-api: adc_interface dac_interface pwm_interface gpio_interface

   Recreate TI's LP-MSPM0G3507 LaunchPad out-of-box GUI Composer demo on Zephyr.

Overview
********

Five demo modes, cycled by pressing button S2 on the LaunchPad or by
flipping the matching toggle switch in the GUI Composer app:

- **Blink LED** -- green LED blinks at a GUI-adjustable rate.
- **Light sensor** -- reads a photodiode, dims/brightens an RGB LED to
  match ambient light.
- **Thermistor** -- reads the on-board thermistor, colors an RGB LED by
  temperature delta from the reading at mode entry.
- **Function generator** -- outputs a selectable waveform (sine,
  square, sawtooth, triangle) on the DAC, plottable live in the GUI.
- **Idle** -- default state, LED1 toggles every 500 ms.

This board's OPA (operational amplifier) module has no Zephyr driver
yet. Every mode that would otherwise depend on it is still wired up in
full, with the specific OPA-only step skipped and logged/documented
rather than faked:

- Thermistor mode has a physical jumper (J9, position [1:2]) that
  routes the thermistor divider straight to the ADC, bypassing the
  amplifier in hardware -- no software workaround needed.
- Light sensor mode has no such bypass: the ADC channel it reads
  shares a physical pin with the amplifier's output pin.
  ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR_TIER_MINIMAL``
  (default) reads it anyway -- real (the amplifier's passive feedback
  resistor still conducts even unpowered) but unamplified and running
  backwards from the active circuit -- brighter light currently
  produces a lower reading and a dimmer LED. A
  ``..._TIER_FULL`` choice exists for the OPA0-amplified reading but
  is unselectable until a Zephyr OPA driver is pinned in.
- Function generator mode's reference firmware routes the DAC output
  through the amplifier before the GUI-plotted ADC readback --
  unconditionally, not just when gain/inversion are being actively
  adjusted.
  ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_MINIMAL``
  (default) reads an internal DAC-to-ADC loopback instead, with no
  amplifier involvement at all, and doesn't register the setGain/
  inversion RX commands (nothing real for them to attach to). A
  ``..._TIER_FULL`` choice exists for the real amplifier signal path
  (readback and both commands) but is unselectable until a Zephyr OPA
  driver is pinned in.

Configuring Demo Modes
***********************

Each mode is its own Kconfig option and its own source file, gated in
both ``CMakeLists.txt`` and ``src/main.c`` -- disabling one drops its
code from the build entirely rather than just skipping it at runtime:

- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK``
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR``
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR``
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR`` -- hidden by
  ``depends on DT_HAS_TI_MSPM0_DAC_ENABLED`` on boards whose SoC has no
  DAC node (all of MSPM0's L-series parts).
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_MINIMAL``/
  ``_FULL`` -- not just a controls toggle: this selects which ADC
  channel the DAC readback itself reads (MINIMAL: the DAC's own
  internal loopback, no amplifier in the path at all; FULL: the
  amplifier's real output, once a Zephyr OPA1 driver is pinned in --
  unselectable until then, ``DT_HAS_TI_MSPM0_OPA_ENABLED``). MINIMAL
  doesn't register the setGain/inversion RX commands at all (nothing
  real for them to attach to); FULL registers both -- gain via a real
  ``opamp_set_gain()`` call, inversion as a build-time overlay choice
  even under FULL, since Zephyr's generic opamp API has no runtime
  functional-mode setter, by design, permanently.

All default to ``y`` where the underlying hardware exists. Disable any
of them with ``west build -- -DCONFIG_SAMPLE_GUI_COMPOSER_DEMO_<MODE>=n`` or
via ``menuconfig``/``guiconfig``. Button S2 skips a disabled mode's
slot when cycling through modes rather than landing on a dead press.

``lp_mspm0g3507`` is the only board this sample currently targets; all
four modes default to ``y`` on it.

Every enabled mode also exposes its own tunable timing/threshold
constants as Kconfig ints, following the same pattern -- override any
of them the same way (``menuconfig``/``guiconfig`` or
``-DCONFIG_<NAME>=<value>``):

- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK_DEFAULT_PERIOD_MS`` (default 500)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR_SAMPLE_PERIOD_MS`` (default 20)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_SAMPLE_PERIOD_MS`` (default 50)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_DELTA_THRESHOLD_C`` (default 2)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_REPORT_PERIOD_MS``
  (default 50)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_DEFAULT_AMPLITUDE_PCT``
  (default 100)

Each is only visible/settable while its parent mode is enabled (e.g.
the thermistor options disappear entirely if
``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR=n``).

Requirements
************

- A TI LP-MSPM0G3507 LaunchPad. GUI Composer talks over UART0 -- the
  onboard XDS110 debug probe's own USB-serial backchannel -- so no
  extra USB-to-UART adapter is needed for that link.
- A second, external USB-to-UART adapter (e.g. CP2102, FT232, CH340)
  for the debug console, wired to header pins PA8 (TX, adapter RX) /
  PA9 (RX, adapter TX) / GND at **115200 baud** -- this sample moves
  Zephyr's console/shell to UART1 so printf() output stays visible on
  its own wire without colliding with GUI Composer's mpack frames on
  UART0.
- The GUI Composer web app at ``dev.ti.com/gc`` (Chrome; uses the Web
  Serial API, hence the browser requirement), pointed at the COM port
  the XDS110's backchannel registers, 115200 baud.

Building and Running
*********************

.. zephyr-app-commands::
   :zephyr-app: applications/gui_composer_demo
   :board: lp_mspm0g3507
   :goals: build flash

Sample Output
*************

On the console UART (UART1, via the external USB-to-UART adapter)::

   [gui_composer_demo] boot

Once GUI Composer is connected on UART0, each mode's readings/toggles
stream live in the app; flipping widgets there (or pressing S2 on the
board) switches modes on the LaunchPad in real time.
