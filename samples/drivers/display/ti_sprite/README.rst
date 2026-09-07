.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

.. _display_tidss_sprite:

TI DSS Sprite Animation
########################

Overview
********

Renders a TI logo sprite that slides in small pixel steps in a serpentine
(boustrophedon) pattern across the full display window: each row is swept in
the opposite direction of the row before it.

The animation loop is entirely callback-driven, exercising the non-blocking
path of the TI DSS (``tidss``) display driver:

1. ``display_write()`` is non-blocking: it enqueues the buffer into the
   driver's request queue and returns immediately.
2. At each VSYNC the driver ISR dequeues the next buffer, latches it into the
   DSS shadow registers, and fires the registered VSYNC callback from ISR
   context.
3. The callback gives a semaphore that the main thread is blocked on.
4. Once unblocked, the main thread advances the sprite position, renders the
   next frame, and calls ``display_write()`` again.

Requirements
************

A board with a ``ti,tidss`` display controller as the ``zephyr,display``
chosen node, reporting ``PIXEL_FORMAT_ARGB_8888``. Two framebuffers are
double-buffered on the heap; :kconfig:option:`CONFIG_HEAP_MEM_POOL_ADD_SIZE_SAMPLE`
must cover at least ``2 x width x height x 4`` bytes (the default, 16 MiB,
covers up to 1920x1080).

``zephyr,display`` stays pointed at the DSS itself in both routes below, so
the sample always gets an ARGB8888 framebuffer straight from ``tidss`` — the
DPI/HDMI bridge and the DSI panel are both downstream sinks that the DSS
output is routed to, not separate ``zephyr,display`` targets.

Supported boards
*****************

- ``am62l_evm/am62l3/a53`` — DSS output can be routed to either of two
  physical sinks:

  - **DSI + RPi panel** (default — ``boards/am62l_evm_am62l3_a53.overlay``,
    applied automatically for this board target) — 720x1280 over MIPI DSI
    (2 lanes) to a Raspberry Pi Touch Display 2 (ILI9881C), whose on-board
    MCU (backlight + reset) is reached over ``main_i2c0``. This matches the
    board's default DSS timings, so only the DSI host, panel node, and MCU
    need enabling.
  - **DPI + HDMI** (``boards/am62l_evm_am62l3_a53_hdmi.overlay``, selected
    explicitly) — 1920x1080@60 Hz over 24-bit parallel DPI to an SII9022A
    HDMI bridge on ``main_i2c1``. Setting ``DTC_OVERLAY_FILE`` replaces the
    default overlay lookup, so the DSI route above is not applied when this
    one is selected.

Building and Running
*********************

DSI + Raspberry Pi panel route (default):

.. code-block:: console

   west build -p always -b am62l_evm/am62l3/a53 ti-zephyr/samples/drivers/display/ti_sprite
   west flash

DPI + HDMI route:

.. code-block:: console

   west build -p always -b am62l_evm/am62l3/a53 ti-zephyr/samples/drivers/display/ti_sprite \
     -- -DDTC_OVERLAY_FILE=boards/am62l_evm_am62l3_a53_hdmi.overlay
   west flash

Sample Output
*************

.. code-block:: console

   display@30200000 720x1280 ARGB8888 — 128x128 sprite stride-sweep
   FPS: 60
   FPS: 60
   FPS: 60

   <repeats endlessly, sprite sliding across the display>
