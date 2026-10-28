MSPM0 VREF
**********

The VREF module is a shared voltage reference for the on-chip analog peripherals. It is exposed as a
regulator (``ti,mspm0-vref``), so dependent peripherals such as the ADC12 can turn the reference on
and off through the regulator API.

Features Supported
==================

- Internal reference of 1.4 V or 2.5 V, selected with ``regulator-uv`` or at run time with
  :c:func:`regulator_set_voltage`
- Reference buffer enable/disable with :c:func:`regulator_enable` and :c:func:`regulator_disable`
  (reference counted by the regulator core)
- Sample and hold mode with configurable cycle counts, clock source and divider

.. only:: mspm33

   - Voltage-to-current (V2I) mode (MSPM33 only), alone or combined with sample and hold

- :c:func:`regulator_get_voltage` and :c:func:`regulator_get_mode`
- ADC12 reference: internal (``ADC_REF_INTERNAL``) or external on the VREF+/VREF- pins
  (``ADC_REF_EXTERNAL0``)
- ADC12 power management: the ADC driver disables the reference on suspend and enables it on resume

Features NOT Supported
=======================

- Changing voltage or mode while enabled. Both calls fail with ``-EBUSY`` unless the requested value
  is already active.
- Run-time changes of sample and hold cycle counts, clock source or divider (devicetree only).

Usage
=====

The driver implements the Zephyr regulator API (``<zephyr/drivers/regulator.h>``). Its binding
includes ``regulator.yaml``, so common properties such as ``regulator-allowed-modes``,
``regulator-initial-mode``, ``regulator-boot-on``, ``regulator-always-on`` and ``off-on-delay-us``
are available.

.. code-block:: devicetree

   vref0: vref@40030000 {
           compatible = "ti,mspm0-vref";
           reg = <0x40030000 0x2000>;
           clocks = <&ckm MSPM0_CLOCK_BUSCLK>;
           ti,clk-div = <0x1>;
           off-on-delay-us = <200>;
           regulator-uv = <1400000>;
           ti,sample-cycles = <10>;
           ti,hold-cycles = <40>;
           regulator-allowed-modes = <MSPM0_VREF_MODE_NORMAL>,
                                     <MSPM0_VREF_MODE_SHMODE>;
   };

Enable the node and set the VREF+ pin (PA23) to analog function:

.. code-block:: devicetree

   &vref0 {
           status = "okay";
           pinctrl-0 = <&analog_pa23>;
           pinctrl-names = "default";
   };

Enabling the Driver
-------------------

Enable the regulator subsystem in ``prj.conf``. ``CONFIG_REGULATOR_MSPM0_VREF`` defaults to ``y``
and selects ``CONFIG_PINCTRL`` once an enabled ``ti,mspm0-vref`` node is present.

.. code-block:: kconfig

   CONFIG_REGULATOR=y

Internal Reference Voltage
--------------------------

The buffer produces either 1.4 V or 2.5 V, and the reference needs an external decoupling capacitor
on VREF+ (see the device datasheet for the value). The voltage after boot is ``regulator-uv``, or
``regulator-init-microvolt`` if set (applied through :c:func:`regulator_set_voltage`). While no
consumer has the regulator enabled, :c:func:`regulator_set_voltage` can switch between the two:

.. code-block:: c

   #include <zephyr/drivers/regulator.h>

   const struct device *vref = DEVICE_DT_GET(DT_NODELABEL(vref0));

   regulator_set_voltage(vref, 2500000, 2500000);

Sample and Hold
---------------

In sample and hold mode the reference capacitor charges during the sample phase and discharges
during the hold phase, which saves power and keeps the reference usable in STANDBY. Phase lengths
are counted in cycles of the ``clocks`` source after the ``ti,clk-div`` divider:

- ``ti,sample-cycles`` sets the sample phase
- ``ti,hold-cycles`` sets the hold phase

The driver programs ``SHCYCLE = sample + hold`` and ``HCYCLE = hold`` (``SHCYCLE`` is the total and
must be greater than ``HCYCLE``). Both counts are 16 bits wide. See the device datasheet for
recommended values.

.. only:: mspm33

   On MSPM33, sample and hold is not available in STOP2, STANDBY0 or STANDBY1 if MFCLK is the
   source, and not in STANDBY1 if BUSCLK or LFCLK is the source.

.. only:: mspm33

   V2I Mode
   --------

   V2I mode enables the voltage-to-current buffer and is present only on MSPM33. It is controlled
   through ``regulator-allowed-modes`` and the mode API:

   .. list-table::
      :header-rows: 1
      :widths: 40 60

      * - Macro
        - Meaning
      * - ``MSPM0_VREF_MODE_NORMAL``
        - Normal operation
      * - ``MSPM0_VREF_MODE_SHMODE``
        - Sample and hold
      * - ``MSPM0_VREF_MODE_V2I``
        - Voltage-to-current buffer
      * - ``MSPM0_VREF_MODE_SHMODE_V2I``
        - Sample and hold with the voltage-to-current buffer

   .. code-block:: c

      #include <zephyr/dt-bindings/regulator/mspm0_vref.h>

      regulator_set_mode(vref, MSPM0_VREF_MODE_SHMODE_V2I);

External Reference
------------------

An external reference is applied on the VREF+ and VREF- pins. Disable the internal buffer before
connecting the external source; only one source can be used at a time. The consumer makes the
choice, for the ADC through :ref:`ADC_REF_EXTERNAL0 <ti-zephyr-regulator-adc>`.

.. _ti-zephyr-regulator-adc:

Use by the ADC12 Driver
-----------------------

The ADC12 node takes a ``vref`` phandle to the regulator. The reference is then chosen per channel
through ``zephyr,reference``:

.. code-block:: devicetree

   &adc0 {
           vref = <&vref0>;
           status = "okay";
           /* ... */

           channel@0 {
                   reg = <0>;
                   zephyr,gain = "ADC_GAIN_1";
                   zephyr,reference = "ADC_REF_INTERNAL";
                   zephyr,acquisition-time = <ADC_ACQ_TIME_DEFAULT>;
                   zephyr,resolution = <12>;
           };
   };

The ADC takes the reference voltage from devicetree, so if an ADC channel uses the internal
reference, set the voltage with ``regulator-uv`` rather than at run time.

With ``CONFIG_PM_DEVICE`` the ADC driver disables the regulator on suspend and enables it on resume
if an internal-reference channel is configured.

Enabling and Disabling
----------------------

Applications using VREF only through the ADC need no regulator calls; the ADC driver enables it. For
standalone use, see below.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/regulator.h>
   #include <zephyr/dt-bindings/regulator/mspm0_vref.h>

   static const struct device *const vref = DEVICE_DT_GET(DT_NODELABEL(vref0));

   int main(void)
   {
           int32_t uv;

           if (!device_is_ready(vref)) {
                   return -1;
           }

           /* The regulator is disabled, so voltage and mode can be changed. */
           regulator_set_voltage(vref, 2500000, 2500000);
           regulator_set_mode(vref, MSPM0_VREF_MODE_SHMODE);

           if (regulator_enable(vref) < 0) {
                   return -1;
           }

           regulator_get_voltage(vref, &uv);

           /* ... use the reference ... */

           regulator_disable(vref);
           return 0;
   }

Samples
-------

- :ws-doc:`Regulator Shell <zephyr/samples/regulator/regulator_shell>`

Devicetree Bindings
===================

- :ws-doc:`ti,mspm0-vref <zephyr/build/dts/api/bindings/regulator/ti,mspm0-vref.html>`
