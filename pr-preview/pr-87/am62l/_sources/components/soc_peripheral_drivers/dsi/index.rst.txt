.. ti-socs: am62l

.. _ti-zephyr-component-dsi:

DSI
###

.. important::

   The MIPI DSI host is present on the **AM62L** SoC family
   (``am62l_evm``), as a Cadence DSI TX core wrapped by a TI-specific
   control block.

Overview
========

DSI (Display Serial Interface) carries pixel data from the :ref:`DSS
<ti-zephyr-component-dss>` to a panel over a serial MIPI link instead of a
parallel RGB bus. Two drivers are involved:

- **``dsi_cdns_ti`` host driver** (``compatible = "ti,j721e-dsi"``) — drives
  the Cadence DSI TX core and its embedded D-PHY. It programs the D-PHY
  PLL and HS clock band, brings up the clock/data lanes, converts the
  DSS's DPI timing into DSI video-mode timing, and connects the DSS's DPI
  output into the DSI TX input (``DSI_WRAP_DPI_CONTROL``). It also
  provides the generic Zephyr ``mipi_dsi`` transfer API
  (:c:func:`mipi_dsi_transfer`, :c:func:`mipi_dsi_dcs_write`,
  :c:func:`mipi_dsi_dcs_read`) used to send/receive DCS commands to a
  panel in low-power mode.
- **Panel driver** — a separate ``display`` driver for the specific panel
  attached to the DSI bus. On AM62L, the Raspberry Pi Touch Display 2 uses
  the ``ilitek,ili9881c`` panel (``display_ili98xx_dsi`` /
  ``display_ili9881c_dsi``), which issues its DCS init sequence over the
  host's ``mipi_dsi_transfer()`` and exposes the standard Zephyr
  ``display`` API (blanking, pixel format) up to the application —
  independent of whichever DSI host or DSS instance it is wired to.

``CONFIG_MIPI_DSI_CDNS_TI`` is selected automatically when an enabled
``ti,j721e-dsi`` node is present; ``CONFIG_ILI9881C_DSI`` is selected
automatically when an enabled ``ilitek,ili9881c`` node is present.

DSI vs. DPI/HDMI output
------------------------

The DSS video port can drive either a parallel DPI bus or a DSI host —
these are two mutually exclusive, alternative sinks for the *same* DSS
pipeline, selected entirely in devicetree:

- **DSI route**: the DSS's DPI output feeds the Cadence DSI TX core
  (via ``DSI_WRAP_DPI_CONTROL``), which serializes it onto 1–4 MIPI DSI
  data lanes to a panel such as the ILI9881C.
- **DPI/HDMI route**: the DSS's DPI output drives external pins directly
  (parallel RGB + sync signals), typically into an HDMI bridge chip such
  as the SII9022A.

In both cases ``zephyr,display`` in the chosen node stays pointed at
``&dss`` — the application always renders into an ARGB8888 framebuffer
handed to the ``tidss`` driver. Neither the DSI host nor a downstream
bridge/panel is a separate ``zephyr,display`` target; see
:ref:`ti-zephyr-component-dss` for the DSS side of this pipeline.

Features Supported
===================

DSI Host (``dsi_cdns_ti``)
---------------------------

- Video mode output: converts the attached panel's DPI timings
  (``mipi_dsi_timings``) to DSI horizontal/vertical blanking packet
  sizes, in both sync-pulse (burst) and sync-event (non-burst) modes,
  selected by ``MIPI_DSI_MODE_VIDEO_SYNC_PULSE``.
- RGB888, RGB666 (loose and packed), and RGB565 pixel formats
  (:c:macro:`MIPI_DSI_PIXFMT_RGB888` etc.) — selects the matching DSI
  packed-pixel-stream data type.
- 1–4 data lanes (``data-lanes`` devicetree property).
- Low-power mode (LP) DCS command transfer
  (:c:func:`mipi_dsi_transfer`, :c:func:`mipi_dsi_dcs_write`,
  :c:func:`mipi_dsi_dcs_read`, :c:func:`mipi_dsi_generic_write`,
  :c:func:`mipi_dsi_generic_read`) for panel initialization and control,
  independent of (and before) video mode is enabled.
- Short and long DCS/generic write packets, and short DCS/generic reads up
  to 16 bytes (longer reads return ``-ENOTSUP``).
- Clean attach/detach (:c:func:`mipi_dsi_attach` / implicit detach):
  tears down video output, the TI wrapper, and D-PHY power on detach, and
  disables the per-instance clocks.

Features NOT Supported
=======================

DSI Host
--------

- Non-continuous HS clock (``MIPI_DSI_CLOCK_NON_CONTINUOUS``) — the clock
  lane is kept continuously running; the mode flag is not interpreted.
- Video burst mode timing optimizations beyond basic sync-pulse support
  (``MIPI_DSI_MODE_VIDEO_BURST``) — the driver computes blanking packet
  sizes for sync-pulse vs. sync-event framing, but does not implement the
  burst-specific bandwidth reclaiming some hosts support.
- DSI command mode (as opposed to video mode) for the main pixel path —
  DCS transfers are only used for panel configuration, not for
  frame-by-frame pixel push.
- Reads longer than 16 bytes (returns ``-ENOTSUP``).
- Multiple virtual channels in practice — the API accepts a ``channel``
  parameter, but only a single panel per host instance has been
  exercised.

Enabling the Driver
=====================

The ``dsi0`` host node is defined (but disabled) in the AM62L SoC
devicetree, with the panel as its child node. Enable both, along with any
GPIO expander or regulator the panel's ``reset-gpios`` depend on:

.. code-block:: devicetree

   &main_i2c0 {
           status = "okay";
   };

   &display_mcu {
           status = "okay";
   };

   &dsi0 {
           status = "okay";
           data-lanes = <2>;

           rpi_td2: panel@0 {
                   status = "okay";
           };
   };

.. code-block:: kconfig

   CONFIG_DISPLAY=y
   CONFIG_MIPI_DSI=y
   CONFIG_ILI9881C_DSI_RPI_TOUCH_DISPLAY=y

.. important::

   The DSS node (``&dss``) must also be enabled with timings matching the
   panel's native resolution and refresh — see
   :ref:`ti-zephyr-component-dss`. The panel's ``reset-gpios`` on the
   Raspberry Pi Touch Display 2 route through the on-board
   ``raspberrypi,touchscreen-panel-regulator-v2`` device (``display_mcu``)
   reached over I2C, not a SoC-native GPIO pin — that I2C bus and the
   regulator node must both be enabled, or the panel reset sequence will
   fail with ``-ENODEV``.

Devicetree Properties
=======================

DSI host node (``compatible = "ti,j721e-dsi"``)
--------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Property
     - Description
   * - ``reg`` / ``reg-names``
     - Three MMIO regions, in order ``"dsi"`` (Cadence DSI TX core),
       ``"wrap"`` (TI DSI wrapper), ``"dphy"`` (Cadence D-PHY TX).
   * - ``clocks`` / ``clock-names``
     - Four clocks named ``"dsi_p_clk"`` (peripheral, always-on),
       ``"dsi_sys_clk"`` (functional, always-on), ``"psm_clk"`` (D-PHY
       state-machine clock), and ``"pll_ref_clk"`` (D-PHY PLL reference,
       enabled only while the link is up).
   * - ``data-lanes``
     - Number of active MIPI DSI data lanes, 1–4.
   * - ``ti,pll-ref-clk-rate-hz``
     - D-PHY PLL reference clock rate in Hz. Must match the rate actually
       configured on the ``pll_ref_clk`` clock (25 MHz on AM62L/AM62P).
   * - ``interrupts``
     - DSI TX interrupt line (declared by the binding; not currently used
       by the driver's synchronous, polling-based transfer path).

Panel node (``compatible = "ilitek,ili9881c"``), child of the DSI host
--------------------------------------------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Property
     - Description
   * - ``reg``
     - MIPI DSI virtual channel number (``0`` for a single panel).
   * - ``data-lanes``
     - Number of data lanes this panel uses (must not exceed the host's
       ``data-lanes``).
   * - ``pixel-format``
     - ``MIPI_DSI_PIXFMT_RGB888`` or a 565/666 variant from
       ``<zephyr/dt-bindings/mipi_dsi/mipi_dsi.h>``.
   * - ``reset-gpios``
     - Active-high reset pin.
   * - ``bl-gpios``
     - Active-high backlight enable pin.
   * - ``width`` / ``height``
     - Panel resolution in pixels (inherited from
       ``display-controller.yaml``).

Usage
======

Panels are consumed through the standard Zephyr ``display`` API; no
application code calls the ``mipi_dsi_*`` functions directly for a panel
driven this way — the host attach/detach and DCS init sequence happen
automatically at boot inside the panel driver. See
:ref:`ti-zephyr-component-dss` for the ``display_write`` /
VSYNC-callback rendering loop used once the panel is up.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/display.h>

   static const struct device *const panel = DEVICE_DT_GET(DT_NODELABEL(rpi_td2));

   if (!device_is_ready(panel)) {
           return;
   }

   /* Backlight on, DCS display-on already issued by the panel driver at init */
   display_blanking_off(panel);

Samples
=======

ti-zephyr
---------

- :ref:`TI DSS Sprite Animation <display_tidss_sprite>` — the default
  build for ``am62l_evm/am62l3/a53`` exercises this exact DSI + ILI9881C
  path (720x1280, 2 data lanes) to drive the Raspberry Pi Touch Display 2.
  The alternate DPI + HDMI overlay for the same sample bypasses DSI
  entirely; see :ref:`ti-zephyr-component-dss` for both routes.

Zephyr Documentation
======================

- :ws-doc:`Display Zephyr Documentation <zephyr/hardware/peripherals/display/index.html>`
