Davinci GPIO
************

Features Supported
==================

- Zephyr GPIO API (``<zephyr/drivers/gpio.h>``) on each ``ti,davinci-gpio`` bank (up to 32 pins)
- Input or output pins via :c:func:`gpio_pin_configure`
- Initial output level with ``GPIO_OUTPUT_INIT_HIGH`` or ``GPIO_OUTPUT_INIT_LOW``; the pin is driven
  low if neither is given
- Port-level access: read input register, masked write, set, clear and toggle bits
- All pins start as inputs; the ``default`` pinctrl state is applied if the node defines one
- Pins addressed through a bank node (for example ``&main_gpio0_3``) or the ``main_gpio0`` nexus
  node, which maps global pin numbers 0 to 125 onto the four banks

Features NOT Supported
======================

- Hardware features the driver does not expose:
  - GPIO interrupts (per-pin rising/falling edge detection and per-bank interrupt lines)

Usage
=====

Enabling the Driver
-------------------

``CONFIG_GPIO_DAVINCI`` is enabled by default once an enabled ``ti,davinci-gpio`` node is present,
and selects ``PINCTRL``. The application (or a driver that selects it) must enable the GPIO
subsystem; ``CONFIG_LED_GPIO`` does so when a ``gpio-leds`` node is enabled.

.. code-block:: kconfig

   CONFIG_GPIO=y

The SoC devicetree defines each bank as follows (shown for ``main_gpio0_3``):

.. code-block:: devicetree

   main_gpio0_3: gpio@600088 {
           compatible = "ti,davinci-gpio";
           reg = <0x00600088 0x28>;
           gpio-controller;
           #gpio-cells = <2>;
           ngpios = <30>;
   };

Enable the bank in the board devicetree or an overlay, with the pin setup for the signals used:

.. code-block:: devicetree

   &main_gpio0_3 {
           pinctrl-0 = <&main_gpio0_3_led>;
           pinctrl-names = "default";
           status = "okay";
   };

Controlling a Pin
-----------------

.. code-block:: c

   #include <zephyr/drivers/gpio.h>

   static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

   int main(void)
   {
           if (!gpio_is_ready_dt(&led)) {
                   return -1;
           }

           gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

           while (1) {
                   gpio_pin_toggle_dt(&led);
                   k_msleep(500);
           }
   }

Samples
-------

- :ws-doc:`Blinky sample <zephyr/samples/basic/blinky/README.html>`

Devicetree Bindings
===================

- :ws-doc:`ti,davinci-gpio <zephyr/build/dts/api/bindings/gpio/ti,davinci-gpio.html>`
