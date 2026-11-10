.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _simple_gui_demo:

Simple GUI Demo
###############

Overview
********

A minimal GUI Composer sample: four counters, an enable switch and a slider
exchanged with the GUI over UART, with no sensors required.

This sample is a minimal reference for the GUI Composer middleware library. It
is modeled on the ``gc_simple_messagepack`` and ``gc_simple_json`` examples in
the MSPM0 SDK and exercises each part of the middleware without any sensor
hardware:

- the transport, by streaming values to the GUI over UART0,
- the receive (RX) command table, by acting on commands sent from the GUI,
- periodic and event-driven updates, using a kernel timer and GPIO interrupts.

The sample streams four counters, which the GUI plots or displays:

.. list-table::
   :header-rows: 1

   * - Token
     - Type
     - Updated by
   * - ``c1``
     - 8-bit
     - Button press (see `Buttons`_), step
       ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U8_STEP``
   * - ``c2``
     - 16-bit
     - Button press (see `Buttons`_), step
       ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U16_STEP``
   * - ``c3``
     - 32-bit
     - Periodic timer, step
       ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U32_STEP``
   * - ``c4``
     - Q8 fixed point (256 = 1.0)
     - Periodic timer, by the increment set from the GUI; wraps to 0 above
       100.0. Only sent while ``bEnable`` is set.

It handles two RX commands from the GUI:

- ``bEnable`` (switch): turns the board LED (``led0``) on or off, and
  enables or disables updates of ``c4``.
- ``u16Data`` (slider): sets the raw Q8 increment that ``c4`` advances by on
  each timer tick.

The step sizes (50, 5000 and 10000) are deliberately different so that the
effect of the variable width is visible in the GUI. Button presses are not
debounced, so a single press may increment a counter more than once.

The GUI Composer project this sample is modeled on is available in the TI
Gallery, one per wire protocol (see `Wire Protocol`_):

- MessagePack (default):
  https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_mpack/ver/1.0.0/
- JSON:
  https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_JSON/ver/1.0.0/

For more on the GUI side, see the `GUI Composer User's Guide
<https://dev.ti.com/gc/v2/help/GC_UserGuide_v2/index.html>`_. For a sample
with sensor-backed modes on the same transport, see
``applications/gui_composer_demo``.

Requirements
************

- One of the supported boards:

  - LP-MSPM0G3507 (``lp_mspm0g3507``)
  - LP-MSPM0G3519 (``lp_mspm0g3519``)
  - LP-MSPM0L2228 (``lp_mspm0l2228``)

  GUI Composer communicates over UART0 at 115200 baud, which is the
  USB-serial backchannel of the on-board XDS110 debug probe. No extra wiring
  is needed.
- A user LED (``led0``) and two user buttons (``sw0`` and ``sw1``) defined in
  the board devicetree. The mapping used on each board is set in
  ``boards/<board>.overlay``.
- The GUI Composer web application at https://dev.ti.com/gc, in a browser
  that supports the Web Serial API (such as Chrome), connected to the COM port
  of the XDS110 backchannel.

.. note::
   UART0 is dedicated to GUI Composer, so the Zephyr console is disabled on
   all three boards (``CONFIG_CONSOLE=n`` and ``CONFIG_UART_CONSOLE=n`` in
   ``boards/<board>.conf``). Output from ``printf()`` is discarded and the
   sample prints nothing.

Buttons
=======

``src/counters.c`` increments ``c2`` on ``sw0`` and ``c1`` on ``sw1``. Each
board overlay maps those aliases to the following user buttons:

.. list-table::
   :header-rows: 1

   * - Board
     - Increments ``c1`` (8-bit)
     - Increments ``c2`` (16-bit)
   * - ``lp_mspm0g3507``
     - PB21 (S3 User Switch)
     - PA18 (S2 User Switch)
   * - ``lp_mspm0g3519``
     - PB3 (Switch 2)
     - PA18 (Switch 1)
   * - ``lp_mspm0l2228``
     - PB8 (S3 User Switch)
     - PA18 (S2 User Switch)

The button names are the labels in each board's devicetree.

Configuration
*************

``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS`` (default ``y``) builds the counters
(``src/counters.c``). Disabling it removes that file from the build and leaves
only the ``bEnable`` command driving the LED, which is the smallest possible
test of the RX path.

While enabled, the counters are tuned with these options, which can be changed
with ``menuconfig``/``guiconfig`` or on the command line with
``-DCONFIG_<NAME>=<value>``:

- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS`` (default 500)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U8_STEP`` (default 50)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U16_STEP`` (default 5000)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U32_STEP`` (default 10000)
- ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_Q8_DEFAULT_INCREMENT`` (default 0)

Wire Protocol
=============

The sample uses MessagePack by default. To use JSON instead, together with the
matching GUI project listed above, build with:

.. code-block:: console

   -DCONFIG_GUI_COMPOSER_PROTOCOL_JSON=y -DCONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK=n

Both variants are built by the two test scenarios in ``sample.yaml``.

Building and Running
********************

Replace ``<board>`` with the ID of your board. The supported boards are listed
under `Requirements`_.

.. code-block:: console

   west build -p always -b <board> ti-zephyr/applications/simple_gui_demo
   west flash

Sample Output
*************

The sample produces no console output (see the note under `Requirements`_).
Behavior is observed through GUI Composer connected to UART0:

#. The counters ``c1``, ``c2`` and ``c3`` are sent to the GUI every
   ``CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS``. Only ``c3`` changes
   on its own.
#. Toggling the switch bound to ``bEnable`` turns the board LED on or off, and
   starts or stops updates of ``c4``.
#. Moving the slider bound to ``u16Data`` changes how fast ``c4`` advances.
#. Pressing the buttons listed under `Buttons`_ steps ``c1`` and ``c2``.
