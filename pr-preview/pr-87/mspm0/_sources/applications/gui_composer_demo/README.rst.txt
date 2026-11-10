.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _gui_composer_demo:

MSPM0 LaunchPad Out-of-Box Demo
###############################

Overview
********

Recreate TI's MSPM0 LaunchPad out-of-box GUI Composer demos on Zephyr.

Up to six demo modes, cycled by pressing the mode button on the LaunchPad or by
flipping the matching toggle switch in the GUI Composer app (S2 on
LP-MSPM0G3507 and LP-MSPM0G3519, S3 on LP-MSPM0L2228). Which modes
are available depends on the board (see `Supported Boards and Modes`_):

- **Blink LED** -- green LED blinks at a GUI-adjustable rate (up to 2.5 s
  period).
- **Light sensor** -- reads a photodiode, dims/brightens an RGB LED to
  match ambient light.
- **Thermistor** -- reads the on-board thermistor, colors an RGB LED by
  temperature delta from the reading at mode entry.
- **Function generator** -- outputs a selectable waveform (sine,
  square, sawtooth, triangle) on the DAC, plottable live in the GUI.
- **Temperature** -- reads the SoC's internal die-temperature sensor and
  plots it in the GUI, in degrees C or F. The units button (the ``sw1`` alias:
  Switch 1 on LP-MSPM0G3519, PA18 on LP-MSPM0L2228) or the GUI units
  switch toggles the unit.
- **Idle** -- default state, LED1 toggles every 500 ms.

The GUI Composer projects this sample is modeled on:

- LP-MSPM0G3507:
  https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0G3507_OOBE/ver/1.0.4/
- LP-MSPM0L2228:
  https://dev.ti.com/gallery/view/6687478/LP_MSPM0L2228_OOB/ver/0.0.2/

Supported Boards and Modes
**************************

.. list-table::
   :header-rows: 1

   * - Mode
     - ``lp_mspm0g3507``
     - ``lp_mspm0g3519``
     - ``lp_mspm0l2228``
   * - Blink LED
     - yes
     - yes
     - yes
   * - Light sensor
     - yes
     - no
     - no
   * - Thermistor
     - yes
     - no
     - no
   * - Function generator
     - yes
     - yes
     - no (SoC has no DAC)
   * - Temperature
     - no
     - yes
     - yes
   * - Tamper detection
     - no
     - no
     - not implemented
   * - Debug console
     - UART1 (PA8/PA9)
     - UART1 (PA8/PA9)
     - disabled

Each "no" is a deliberate exclusion (hardware absent, or no Zephyr driver),
not a bug. The per-board defaults live
in ``boards/<board>.conf``; a mode that is off there is not compiled in and
is skipped when cycling with the mode button.

Known Limitations
*****************

- **Tamper detection is not implemented.** The LP-MSPM0L2228 reference
  firmware has a tamper mode: a rising edge on the tamper I/O pin (PA7)
  latches an RTC time stamp. Zephyr has no tamper driver or API for MSPM0,
  and this sample does not poke the registers directly, so the mode is
  absent. The RTC node is enabled in the overlay but unused by the sample.
- **On-board LCD is not driven.** TI's LP-MSPM0L2228 firmware also
  shows the temperature and tamper time stamp on the LaunchPad LCD. This
  sample reports to the GUI only.
- **Light sensor and thermistor are LP-MSPM0G3507 only.** TI's
  reference firmware for the other boards does not demo them, and the
  light sensor additionally needs the ``pwma1`` PWM node for RGB dimming,
  which only the G3507 overlay provides.
- **Function generator needs a DAC.** MSPM0 L-series parts have none, so
  the mode is hidden on ``lp_mspm0l2228``
  (``depends on DT_HAS_TI_MSPM0_DAC_ENABLED``).
- **No Zephyr OPA (operational amplifier) driver yet.** Every mode that
  would use it is still wired up, with the OPA-only step skipped rather
  than faked:

  - Thermistor mode has a physical jumper (J9, position [1:2]) that
    routes the thermistor divider straight to the ADC, bypassing the
    amplifier in hardware -- no software workaround needed.
  - Light sensor mode has no such bypass: the ADC channel it reads
    shares a physical pin with the amplifier's output pin.
    ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR_TIER_MINIMAL``
    (default) reads it anyway -- real (the amplifier's passive feedback
    resistor still conducts even unpowered) but unamplified and running
    backwards from the active circuit -- brighter light currently
    produces a lower reading and a dimmer LED. A ``..._TIER_FULL``
    choice exists for the OPA0-amplified reading but is unselectable
    until a Zephyr OPA driver is pinned in
    (``DT_HAS_TI_MSPM0_OPA_ENABLED``).
  - Function generator mode's reference firmware routes the DAC output
    through the amplifier before the GUI-plotted ADC readback.
    ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_MINIMAL``
    (default) reads an internal DAC-to-ADC loopback instead, with no
    amplifier involvement, and does not register the setGain/inversion
    RX commands (nothing real for them to attach to). A
    ``..._TIER_FULL`` choice exists for the real amplifier signal path
    (readback and both commands) but is unselectable until a Zephyr OPA
    driver is pinned in. Even under FULL, inversion is a build-time
    overlay choice, since Zephyr's generic opamp API has no runtime
    functional-mode setter.

- **Thermistor first sample.** The first ADC conversion after channel
  setup reads high on hardware, so mode entry discards one conversion
  before taking the baseline.
- **Temperature mode prerequisites.** It needs the internal VREF
  regulator (``vref = <&vref0>`` on ``&adc0``, since the internal-reference
  ADC channel is rejected without it) and the factory trim constant, read
  through the ``cpuss_syscon``/``factoryregion_syscon`` nodes with the
  instruction cache disabled for the duration of the read. The Kconfig
  option selects ``REGULATOR`` and ``SYSCON`` automatically; both are
  already set up in the board overlays.
- **LP-MSPM0L2228 has no debug console.** GUI Composer owns UART0 there,
  which is also the board's only console UART, so ``CONFIG_CONSOLE`` and
  ``CONFIG_UART_CONSOLE`` are off and ``printf()`` output is discarded.

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
  unselectable until then). MINIMAL doesn't register the
  setGain/inversion RX commands at all; FULL registers both -- gain via
  a real ``opamp_set_gain()`` call.
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE``

The modes are enabled per board in ``boards/<board>.conf`` (see the table
above). Override with
``west build -- -DCONFIG_SAMPLE_GUI_COMPOSER_DEMO_<MODE>=n`` or via
``menuconfig``/``guiconfig``. The mode button skips a disabled mode's slot when
cycling through modes rather than landing on a dead press.

Every enabled mode also exposes its own tunable timing/threshold
constants as Kconfig ints, following the same pattern -- override any
of them the same way (``menuconfig``/``guiconfig`` or
``-DCONFIG_<NAME>=<value>``):

- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK_DEFAULT_PERIOD_MS`` (default 500)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR_SAMPLE_PERIOD_MS`` (default 20)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_SAMPLE_PERIOD_MS`` (default 50)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_DELTA_THRESHOLD_C`` (default 3)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_REPORT_PERIOD_MS``
  (default 25)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_DEFAULT_AMPLITUDE_PCT``
  (default 100)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE_SAMPLE_PERIOD_MS`` (default 50)
- ``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE_DELTA_THRESHOLD_C`` (default 2)

Each is only visible/settable while its parent mode is enabled (e.g.
the thermistor options disappear entirely if
``CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR=n``).

Requirements
************

- A supported LaunchPad: LP-MSPM0G3507, LP-MSPM0G3519 or LP-MSPM0L2228.
  GUI Composer talks over UART0 -- the onboard XDS110 debug probe's own
  USB-serial backchannel -- so no extra USB-to-UART adapter is needed for
  that link.
- LP-MSPM0G3507 and LP-MSPM0G3519 only: a second, external USB-to-UART
  adapter (e.g. CP2102, FT232, CH340) for the debug console, wired to
  header pins PA8 (TX, adapter RX) / PA9 (RX, adapter TX) / GND at
  **115200 baud**. This sample moves Zephyr's console to UART1 so
  ``printf()`` output stays visible on its own wire without colliding with
  GUI Composer frames on UART0. LP-MSPM0L2228 has no console (see above).
- The GUI Composer web app at ``dev.ti.com/gc`` (Chrome; uses the Web
  Serial API, hence the browser requirement), pointed at the COM port
  the XDS110's backchannel registers, 115200 baud.

Building and Running
********************

Replace ``<board>`` with ``lp_mspm0g3507``, ``lp_mspm0g3519`` or
``lp_mspm0l2228``.

.. code-block:: console

   west build -p always -b <board> ti-zephyr/applications/gui_composer_demo
   west flash

Sample Output
*************

On the console UART (UART1, via the external USB-to-UART adapter; not
available on LP-MSPM0L2228)::

   [gui_composer_demo] boot

Once GUI Composer is connected on UART0, each mode's readings/toggles
stream live in the app; flipping widgets there (or pressing the mode
button on the board) switches modes on the LaunchPad in real time.
