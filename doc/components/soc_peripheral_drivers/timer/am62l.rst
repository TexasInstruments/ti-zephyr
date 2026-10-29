DMTimer
*******

Features Supported
==================

- The TI dual-mode timer (``ti,am654-timer``) as the Zephyr system timer, which provides the kernel
  tick (``<zephyr/drivers/timer/system_timer.h>``)
- Tickless operation (``CONFIG_TICKLESS_KERNEL``); the driver selects ``TICKLESS_CAPABLE``. Without
  it the timer raises one interrupt per tick
- Free-running 32-bit counter in auto-reload mode, with the compare match interrupt programmed for
  the next timeout
- :c:func:`sys_clock_cycle_get_32` returns the raw counter value
- Optional functional clock source selection with the ``clksel`` property, written through a
  syscon device. The ``<zephyr/dt-bindings/timer/ti-dmtimer.h>`` header names the sources

Features NOT Supported
======================

- the counter, PWM and watchdog APIs; the driver only implements the system timer
- Use of the selected instance by the application. The instance named by ``zephyr,system-timer``
  belongs to the kernel

Usage
=====

Enabling the Driver
-------------------

``CONFIG_TI_DM_TIMER`` is enabled by default when ``zephyr,system-timer`` points to an enabled
``ti,am654-timer`` node. No other option is needed.

.. code-block:: devicetree

   / {
           chosen {
                   zephyr,system-timer = &main_timer0;
           };
   };

   &main_timer0 {
           status = "okay";
   };

.. code-block:: devicetree

   main_timer0: timer@2400000 {
           compatible = "ti,am654-timer";
           reg = <0x2400000 DT_SIZE_K(1)>;
           interrupts = <GIC_SPI 170 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-parent = <&gic>;
           clocks = <&scmi_clk 58>;
           clock-names = "fck";
           power-domains = <&main_timer0_pd>;
           status = "disabled";
   };

The kernel converts counter cycles to ticks with ``CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC``.

.. code-block:: kconfig

   CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC=<timer clock in Hz>

On SoCs where the clock source can be selected through a control module register, add ``clksel``
with the syscon, the register offset and a value from ``ti-dmtimer.h``:

.. code-block:: devicetree

   #include <zephyr/dt-bindings/timer/ti-dmtimer.h>

   &main_timer0 {
           clksel = <&syscon_node 0x1234 HFOSC0_CLKOUT>;
   };

Using the Tick
--------------

Applications use the kernel APIs that sit on the system timer. Nothing in the driver is called
directly:

.. code-block:: c

   #include <zephyr/kernel.h>
   #include <zephyr/sys/printk.h>

   int main(void)
   {
           int64_t start = k_uptime_get();

           k_msleep(100);

           printk("slept %lld ms\n", k_uptime_get() - start);

           return 0;
   }

Devicetree Bindings
===================

- :ws-doc:`ti,am654-timer <zephyr/build/dts/api/bindings/timer/ti,am654-timer.html>`
