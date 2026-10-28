Features Supported
******************

- Clock tree set up at boot from devicetree: SYSOSC frequency, HFCLK source, SYSPLL, HSCLK source,
  LFCLK source, MCLK source and dividers, MFPCLK and CANCLK
- Clock sources:

  - SYSOSC at 32 MHz or 4 MHz
  - LFOSC (32.768 kHz), the LFXT crystal, or an external digital input as LFCLK
  - HFXT crystal (4 to 48 MHz) or an external digital input as HFCLK
  - SYSPLL, referenced from SYSOSC or HFCLK, with the CLK0, CLK1 and CLK2X outputs

- Clock-control API (``<zephyr/drivers/clock_control.h>``) through ``struct mspm0_sys_clock``, whose
  ``clk`` field is one of the ``MSPM0_CLOCK_*`` identifiers:

  - :c:func:`clock_control_get_rate` for SYSOSC, LFCLK, HFCLK, HSCLK, MCLK, ULPCLK, MFPCLK, CANCLK,
    the three SYSPLL outputs, and ``MCLK_DIV_2`` and ``MCLK_DIV_4`` where they exist. The rate is
    computed from live register contents, not devicetree.
  - :c:func:`clock_control_get_status` for SYSOSC, MFPCLK, SYSPLL, HFCLK, HSCLK, CANCLK and LFCLK
  - :c:func:`clock_control_on` and :c:func:`clock_control_off` for SYSOSC, MFPCLK and SYSPLL
  - :c:func:`clock_control_set_rate` to switch SYSOSC between 32 MHz and 4 MHz
  - :c:func:`clock_control_configure` to change the source of LFCLK, HFCLK, HSCLK, MCLK, MFPCLK,
    CANCLK and the SYSPLL reference at run time, using ``enum mspm0_clock_source``. A clock that is
    on is switched off for the change and back on afterwards.

- Factory SYSPLL trim values matching the PLL input range (4 to 48 MHz), loaded from the factory
  region. On MSPM0G and MSPM0L the CPUSS instruction cache is disabled while the trim is read.
- Flash wait states (and RAM wait states on MSPM33C) programmed when the SYSPLL is used on MSPM33C
  and AM13E
- Wait for oscillator and PLL ready flags, with a timeout and ``-ETIMEDOUT`` on failure
- HFXT startup monitor enabled when HFXT is selected
- Build-time checks on the clock topology (invalid sources, dividers, crystal range)

Features NOT Supported
**********************

- Hardware features the driver does not expose:

  - CLK_OUT / EXCLK external clock output
  - the frequency clock counter (FCC)
  - the SYSOSC frequency correction loop (FCL) and the user-trimmed 16 MHz and 24 MHz SYSOSC
    settings.
  - the LFCLK monitor and clock-failure interrupts; failures are only visible in
    :c:func:`clock_control_get_status`
  - run-time reconfiguration of the SYSPLL dividers (``p-div``, ``q-div`` and the output dividers);
    they are applied once at boot

- Power management; the driver has no suspend/resume hooks.

Usage
*****

.. list-table::
   :header-rows: 1
   :widths: 18 82

   * - Node
     - Role
   * - ``sysosc``
     - Internal oscillator, 32 MHz by default. ``clock-frequency`` may be 32 MHz or
       4 MHz; any other value fails the build. Reset source of MCLK.
   * - ``lfosc``, ``lfxt``
     - 32.768 kHz internal oscillator and low-frequency crystal. Set
       ``status = "okay"`` to use ``lfxt``.
   * - ``lfclk``
     - Low-frequency clock. Source must be ``lfxt``, ``lfosc`` (default) or a ``lfclk_in``
       node.
   * - ``hfxt``
     - High-frequency crystal. Set ``status = "okay"`` to use it. Frequency must be between 4
       and 48 MHz.
   * - ``hfclk``
     - High-frequency clock. Set ``status = "okay"`` to use it. Source must be ``hfxt`` or an
       ``hfclk_in`` node.
   * - ``syspll``
     - System PLL, ``ti,mspm0-pll``. Reference must be ``sysosc`` or ``hfclk``.
   * - ``hsclk``
     - High-speed clock that feeds MCLK. Source is ``hfclk``, ``syspll`` or ``sysosc``.
       Set ``status = "okay"`` to use it.
   * - ``mclk``
     - Main clock. Source is ``sysosc`` (default), ``hsclk``, or ``lfclk`` on G and L only.
       ``clk-div`` (MDIV) is only applied when MCLK comes from a 4 MHz SYSOSC.
   * - ``ulpclk``
     - MSPM0G/L only. MCLK divided by ``clk-div`` (UDIV).
   * - ``mclk_div_2``, ``mclk_div_4``
     - MSPM33C and AM13E only. Replace ULPCLK. ``clk-div`` is 1 or 2 for ``mclk_div_2`` and
       1, 2 or 4 for ``mclk_div_4``; the second must not be smaller than the first.
   * - ``mfpclk``
     - Fixed 4 MHz clock for peripherals. Source is ``sysosc`` or ``hfclk``. From HFCLK, the
       HFCLK rate must be a multiple of 4 MHz up to 64 MHz so a divider from 1 to 16 gives
       exactly 4 MHz. Set ``status = "okay"`` to use it. Absent on AM13E.
   * - ``canclk``
     - Clock for the CAN peripherals. Source is ``hfclk`` or ``syspll`` (its
       CLK1 output). Set ``status = "okay"`` to use it.

Enabling the Driver
===================

``CONFIG_CLOCK_CONTROL_MSPM0`` is ``y`` by default when the ``ckm`` node is enabled with the
``ti,mspm0-ckm`` compatible, and it selects ``SYSCON``. Nothing is needed in ``prj.conf``. The
MSPM0G and MSPM0L SoC Kconfig selects ``CONFIG_CLOCK_CONTROL``. The SoC code sets
``CONFIG_SYSCON_INIT_PRIORITY`` to 20 so the SYSCTL region is ready before the driver, which
initializes at ``PRE_KERNEL_1``.

Peripherals reference the controller through a one-cell clock specifier from
``<zephyr/dt-bindings/clock/mspm0_clock.h>``:

.. code-block:: devicetree

   uart0: serial@40108000 {
           clocks = <&ckm MSPM0_CLOCK_ULPCLK>;
   };

The SoC devicetree files do this for every peripheral; applications normally only change the clock
tree nodes described below.

Devicetree Configuration
========================

The ``ckm`` node (``ti,mspm0-ckm``) holds the register regions as phandles to ``syscon`` nodes:

.. code-block:: devicetree

   ckm: clock-module {
           compatible = "ti,mspm0-ckm";
           #clock-cells = <1>;
           sysctl = <&sysctl>;
           cpuss = <&cpuss>;
           factoryregion = <&factoryregion>;
   };

.. only:: mspm0

   **80 MHz MCLK example (MSPM0G).** The SoC devicetree defines ``syspll`` as SYSOSC (32 MHz) / 2
   * 5 = 80 MHz VCO, with CLK2X divided by 2. To use it:

   .. code-block:: devicetree

      syspll: syspll {
              compatible = "ti,mspm0-pll";
              clocks = <&sysosc>;
              p-div = <2>;
              q-div = <5>;
              clk1-div = <2>;
              clk2x-div = <2>;
              #clock-cells = <0>;
      };

      /* user configuration */
      &syspll {
              status = "okay";
      };

      &ulpclk {
              clk-div = <2>;
      };

      &mclk {
              clock-frequency = <DT_FREQ_M(80)>;
              clocks = <&hsclk>;
      };

      &hsclk {
              clocks = <&syspll>;
              status = "okay";
      };

      &mfpclk {
              status = "okay";
      };

      &canclk {
              clocks = <&syspll>;
              status = "okay";
      };

   MCLK runs at 80 MHz (CLK2X is twice the VCO divided by ``clk2x-div``) and ULPCLK at 40 MHz.
   CANCLK is SYSPLL CLK1, the VCO divided by ``clk1-div``, so 40 MHz.

**Crystals.** Describe the crystals and their startup times:

.. code-block:: devicetree

   &hfxt {
           clock-frequency = <DT_FREQ_M(48)>;
           ti,xtal-startup-delay-us = <500>;
           status = "okay";
   };

   &lfxt {
           clock-frequency = <32768>;
           ti,xtal-startup-delay-us = <1000000>;
           status = "okay";
   };

Setting a crystal node to ``okay`` does not make the tree use it; a clock must be sourced from it.
To run LFCLK from the crystal, and HFCLK from the crystal so it can feed HSCLK or the PLL:

.. code-block:: devicetree

   &lfclk {
           clocks = <&lfxt>;
   };

   &hfclk {
           clocks = <&hfxt>;
           status = "okay";
   };

   &hsclk {
           clocks = <&hfclk>;
           status = "okay";
   };

   &mclk {
           clocks = <&hsclk>;
   };

Using the Clock Control API
===========================

A driver with a ``clocks`` entry in devicetree builds its subsystem descriptor with
``MSPM0_CLOCK_SUBSYS_FN()``. Application code can build the descriptor directly:

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/clock_control.h>
   #include <zephyr/drivers/clock_control/mspm0_clock_control.h>
   #include <zephyr/kernel.h>

   int main(void)
   {
           const struct device *ckm = DEVICE_DT_GET(DT_NODELABEL(ckm));
           struct mspm0_sys_clock mclk = {.clk = MSPM0_CLOCK_MCLK};
           struct mspm0_sys_clock pll = {.clk = MSPM0_CLOCK_SYSPLL};
           uint32_t rate;

           if (!device_is_ready(ckm)) {
                   return -ENODEV;
           }

           if (clock_control_get_rate(ckm, (clock_control_subsys_t)&mclk, &rate) == 0) {
                   printk("MCLK = %u Hz\n", rate);
           }

           if (clock_control_get_status(ckm, (clock_control_subsys_t)&pll) ==
               CLOCK_CONTROL_STATUS_ON) {
                   printk("SYSPLL locked\n");
           }

           return 0;
   }

``SYSPLL`` can be switched off and on; turning it on sets SYSOSC to 32 MHz first and waits for the
PLL-good flag:

.. code-block:: c

   struct mspm0_sys_clock pll = {.clk = MSPM0_CLOCK_SYSPLL};

   clock_control_off(ckm, (clock_control_subsys_t)&pll);
   /* ... */
   clock_control_on(ckm, (clock_control_subsys_t)&pll);

Switching SYSOSC to 4 MHz works only while MCLK is on SYSOSC:

.. code-block:: c

   struct mspm0_sys_clock sysosc = {.clk = MSPM0_CLOCK_SYSOSC};
   uint32_t freq = MHZ(4);

   int ret = clock_control_set_rate(ckm, (clock_control_subsys_t)&sysosc,
                                    (clock_control_subsys_rate_t)&freq);

Change a clock source at run time with :c:func:`clock_control_configure` and an ``enum
mspm0_clock_source``:

.. code-block:: c

   enum mspm0_clock_source src = MSPM0_CLOCK_SRC_LFXT;
   struct mspm0_sys_clock lfclk = {.clk = MSPM0_CLOCK_LFCLK};

   clock_control_configure(ckm, (clock_control_subsys_t)&lfclk, &src);

The accepted sources per clock are:

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Clock
     - ``enum mspm0_clock_source`` values accepted
   * - ``MSPM0_CLOCK_LFCLK``
     - ``LFXT``, ``LFCLK_IN``
   * - ``MSPM0_CLOCK_HFCLK``
     - ``HFXT``, ``HFCLK_IN``
   * - ``MSPM0_CLOCK_SYSPLL``
     - ``SYSOSC``, ``HFCLK`` (reference)
   * - ``MSPM0_CLOCK_HSCLK``
     - ``HFCLK``, ``SYSPLL`` (``SYSOSC`` when the SoC has no PLL)
   * - ``MSPM0_CLOCK_MCLK``
     - ``SYSOSC``, ``HSCLK``, and ``LFCLK`` on G and L only
   * - ``MSPM0_CLOCK_MFPCLK``
     - ``SYSOSC``, ``HFCLK``
   * - ``MSPM0_CLOCK_CANCLK``
     - ``HFCLK``, ``SYSPLL``

An unlisted source returns ``-ENOTSUP``. Only sources whose nodes are enabled in devicetree are
compiled in. When HSCLK changes while MCLK runs from it, the driver moves MCLK to SYSOSC for the
switch and back afterwards.

- :c:func:`clock_control_get_rate` reads the live hardware state. If MCLK has been switched to LFCLK
  (for example by power-management code), MCLK reports the LFCLK rate.
- Clocks sourced from the PLL need the PLL output enabled. A PLL output whose divider property is
  missing returns ``-ENODEV``.

Devicetree Bindings
*******************

- :ws-doc:`ti,mspm0-ckm <zephyr/build/dts/api/bindings/clock/ti,mspm0-ckm.html>`
- :ws-doc:`ti,mspm0-clk <zephyr/build/dts/api/bindings/clock/ti,mspm0-clk.html>`
- :ws-doc:`ti,mspm0-osc <zephyr/build/dts/api/bindings/clock/ti,mspm0-osc.html>`
- :ws-doc:`ti,mspm0-pll <zephyr/build/dts/api/bindings/clock/ti,mspm0-pll.html>`
