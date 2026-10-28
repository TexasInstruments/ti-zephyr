K3 Pinctrl
**********

Features Supported
==================

- Pad configuration of the TI K3 pin controller (``ti,k3-pinctrl``) through the Zephyr pinctrl API
  (``pinctrl-0`` and ``pinctrl-names`` in consumer nodes)
- Per pin: pad register offset, pull and input-enable setting, and mux mode
- Any number of pins per pin group and any number of groups per state

Features NOT Supported
======================

- Reading back or partially updating a pad register. The whole register is overwritten, so each pin
  group must give every bit it needs.
- GPIO data and direction (handled by the GPIO driver).

Usage
=====

Enabling the Driver
-------------------

``CONFIG_PINCTRL_TI_K3`` is enabled by default once a ``ti,k3-pinctrl`` node has ``status =
"okay"``. Peripheral drivers that use pinctrl (I2C, GPIO, UART and so on) select ``CONFIG_PINCTRL``;
set it explicitly if none is enabled:

.. code-block:: kconfig

   CONFIG_PINCTRL=y

.. code-block:: devicetree

   wkup_pinctrl: pinctrl@4084000 {
           compatible = "ti,k3-pinctrl";
           reg = <0x04084000 0x24c>;
   };

Set ``status = "okay"`` in the board devicetree or an overlay:

.. code-block:: devicetree

   &wkup_pinctrl {
           status = "okay";
   };

Pin Group Syntax
----------------

Each pin is one ``K3_PINMUX()`` entry from ``<zephyr/dt-bindings/pinctrl/ti-k3-pinctrl.h>``:

.. code-block:: text

   K3_PINMUX(offset, value, mux_mode)

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Argument
     - Description
   * - ``offset``
     - Offset of the pad configuration register from the controller base.
   * - ``value``
     - ``PIN_OUTPUT``, ``PIN_OUTPUT_PULLUP``, ``PIN_OUTPUT_PULLDOWN``,
       ``PIN_INPUT``, ``PIN_INPUT_PULLUP`` or ``PIN_INPUT_PULLDOWN``.
   * - ``mux_mode``
     - ``MUX_MODE_0`` to ``MUX_MODE_14``; the available modes depend on the pin.

Pin groups are child nodes of the controller:

.. code-block:: devicetree

   &wkup_pinctrl {
           uart0_rx_default: uart0_rx_default {
                   pinmux = <K3_PINMUX(0x1b4, PIN_INPUT, MUX_MODE_0)>;
           };
   };

A consumer node selects the groups for its ``default`` state:

.. code-block:: devicetree

   &main_i2c0 {
           pinctrl-0 = <&main_i2c0_scl &main_i2c0_sda>;
           pinctrl-names = "default";
           status = "okay";
   };

Peripheral drivers apply the ``default`` state when they initialize, so applications normally make
no pinctrl calls.

Devicetree Bindings
===================

- :ws-doc:`ti,k3-pinctrl <zephyr/build/dts/api/bindings/pinctrl/ti,k3-pinctrl.html>`
