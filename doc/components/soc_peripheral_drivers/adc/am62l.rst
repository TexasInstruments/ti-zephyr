AM335x ADC
**********

Features Supported
==================

- ADC API (``<zephyr/drivers/adc.h>``): :c:func:`adc_channel_setup`, :c:func:`adc_read` and, with
  ``CONFIG_ADC_ASYNC``, :c:func:`adc_read_async`
- Several channels per :c:func:`adc_read` call, converted back to back. Conversions can repeat with
  ``extra_samplings`` and wait ``interval_us`` between rounds.
- Software-started conversions
- Single-ended and differential inputs (``differential`` in :c:struct:`adc_channel_cfg`)
- Analog input selection with ``input_positive`` and ``input_negative`` when
  ``CONFIG_ADC_CONFIGURABLE_INPUTS`` is enabled. Without it, channel *n* reads analog input *n*.
- Hardware averaging of 1, 2, 4, 8 or 16 conversions per sample, set per channel with
  ``zephyr,oversampling`` (0 to 4)
- Per-channel timing:

  - ``ti,open-delay``: wait, in ADC sample-clock periods, before the input is sampled (up to 262143)
  - ``acquisition_time`` (``zephyr,acquisition-time``): settling time, 0 to 255, plus two
    sample-clock periods

- Internal result buffer selection (``ti,fifo``, 0 or 1); both give the same results
- Recovery from result buffer overflow or underflow: the driver stops the ADC, clears the error and
  restarts it
- Internal reference voltage from ``ti,vrefp`` (in mV) for ``ADC_REF_INTERNAL``
- Default pinctrl state applied at initialization

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - external hardware triggers (``ADC_EXT_TRIGGER0`` and ``ADC_EXT_TRIGGER1``); software starts
    every conversion
  - continuous conversion; use the Zephyr sequence options for repeated sampling
  - DMA; the processor copies results in the interrupt handler

- Device power management.

Usage
=====

Enabling the Driver
-------------------

The application must set ``CONFIG_ADC``:

.. code-block:: kconfig

   CONFIG_ADC=y

``CONFIG_ADC_TI_AM335X`` is enabled by default when an enabled ``ti,am335x-adc`` node exists. These
options are optional:

.. code-block:: kconfig

   CONFIG_ADC_ASYNC=y
   CONFIG_ADC_CONFIGURABLE_INPUTS=y

Devicetree Configuration
------------------------

The controller node, as the SoC devicetree defines it:

.. code-block:: devicetree

   main_adc0: adc@28001000 {
           compatible = "ti,am335x-adc";
           reg = <0x28001000 DT_SIZE_K(1)>;
           interrupts = <GIC_SPI 253 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-parent = <&gic>;
           power-domains = <&main_adc0_pd>;
           clocks = <&scmi_clk 4>;
           #io-channel-cells = <1>;
           #address-cells = <1>;
           #size-cells = <0>;
   };

To use the ADC, set ``status = "okay"``, apply the pin configuration, and add one child node per
channel (``reg`` is the channel number):

.. code-block:: devicetree

   &main_adc0 {
           ti,fifo = <0>;
           pinctrl-0 = <&adc0_ext_trigger0>, <&adc0_ext_trigger1>;
           pinctrl-names = "default";
           status = "okay";

           channel@0 {
                   reg = <0>;
                   ti,open-delay = <0>;
                   zephyr,gain = "ADC_GAIN_1";
                   zephyr,reference = "ADC_REF_VDD_1";
                   zephyr,acquisition-time = <0>;
                   zephyr,resolution = <12>;
                   zephyr,oversampling = <4>;
                   zephyr,vref-mv = <1800>;
           };
   };

Reading the Converter
---------------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/adc.h>

   static const struct adc_dt_spec adc_chan =
           ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

   int main(void)
   {
           uint16_t sample;
           struct adc_sequence sequence = {
                   .buffer = &sample,
                   .buffer_size = sizeof(sample),
           };
           int32_t mv;
           int ret;

           if (!adc_is_ready_dt(&adc_chan)) {
                   return -ENODEV;
           }

           ret = adc_channel_setup_dt(&adc_chan);
           if (ret < 0) {
                   return ret;
           }

           (void)adc_sequence_init_dt(&adc_chan, &sequence);

           ret = adc_read_dt(&adc_chan, &sequence);
           if (ret < 0) {
                   return ret;
           }

           mv = sample;
           (void)adc_raw_to_millivolts_dt(&adc_chan, &mv);

           printk("raw %u, %d mV\n", sample, mv);
           return 0;
   }

- Raw values go from 0 to 1023.
- Channels in a sequence are converted, and their samples stored, in the order of first
  :c:func:`adc_channel_setup`, not channel number. Set channels up in ascending order for the usual
  layout.
- Each channel needs 2 bytes in the buffer per sampling.
- Calling :c:func:`adc_channel_setup` again for a channel replaces its settings and keeps its
  position in the order.

Samples
-------

- :ws-doc:`ADC devicetree driver sample <zephyr/samples/drivers/adc/adc_dt/README.html>`
  (``samples/drivers/adc/adc_dt``)
- :ws-doc:`ADC driver sequence sample <zephyr/samples/drivers/adc/adc_sequence/README.html>`

Devicetree Bindings
===================

- :ws-doc:`ti,am335x-adc <zephyr/build/dts/api/bindings/adc/ti,am335x-adc.html>`
