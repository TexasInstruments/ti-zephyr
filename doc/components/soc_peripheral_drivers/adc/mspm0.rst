ADC12
*****

.. important::

   The ``ti,mspm0-adc12`` driver covers the 12-bit ADC.

Features Supported
==================

- Single-ended conversions through the ADC API (``<zephyr/drivers/adc.h>``):
  :c:func:`adc_channel_setup`, :c:func:`adc_read`, and :c:func:`adc_read_async` when
  ``CONFIG_ADC_ASYNC`` is set
- Software-triggered sequences of up to ``max-result-reg`` channels (12 in the SoC devicetrees),
  converted back to back in one read
- Resolutions of 8, 10 and 12 bits. A 14-bit request is accepted only with maximum oversampling and
  is converted at 12 bits.
- Hardware averaging through ``oversampling`` (2x to 128x)
- Two programmable sample times per instance, shared by all channels (``acquisition_time`` in ticks,
  microseconds or nanoseconds)
- Per-channel reference: ``ADC_REF_VDD_1`` (VDDA), ``ADC_REF_INTERNAL`` (on-chip VREF regulator) and
  ``ADC_REF_EXTERNAL0`` (external reference)
- Repeated sampling with ``extra_samplings`` and sampling intervals through ``sequence->options``
- Optional DMA transfer of results (``CONFIG_ADC_MSPM0_ADC12_DMA``), with fall back to interrupt
  mode if the DMA channel cannot be set up
- Sample clock selection (``ULPCLK``, ``SYSOSC`` or ``HFCLK``) with a divider of 1, 2, 4, 8, 16, 24,
  32 or 48
- Automatic power-down between conversions (``auto-powerdown``)
- Device power management and runtime power management

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - hardware (event) triggers; only the software trigger is used
  - single-channel, repeat-single-channel and repeat-sequence conversion modes; the driver runs one
    sequence per sampling round and repeats sampling in software

- Mixing internal and external references on one instance. After one is selected, a channel asking
  for the other fails with ``-EINVAL``.

Usage
=====

Enabling the Driver
-------------------

The SoC devicetree defines the controller node (shown for one instance, without its ``status``
property):

.. code-block:: devicetree

   adc0: adc@40000000 {
           compatible = "ti,mspm0-adc12";
           reg = <0x40000000 0x2000>;
           clocks = <&ckm MSPM0_CLOCK_ULPCLK>;
           max-result-reg = <12>;
           interrupts = <4 0>;
           zephyr,disabling-power-states = <&stop2 &stdby0 &stdby1>;
           ti,num-channels = <16>;
           #io-channel-cells = <1>;
   };

Set the ADC node to ``status = "okay"`` in an application overlay, route the analog pins in
``pinctrl-0`` and add one child node per channel. Enable the ADC subsystem in ``prj.conf``.
``CONFIG_ADC_MSPM0_ADC12`` is selected automatically when an enabled ``ti,mspm0-adc12`` node exists.

.. code-block:: devicetree

   #include <dt-bindings/adc/adc.h>

   / {
           zephyr,user {
                   io-channels = <&adc0 0>, <&adc0 1>;
           };
   };

   &adc0 {
           vref = <&vref0>;
           pinctrl-0 = <&analog_pa27 /* ADC0.0 */
                        &analog_pa26 /* ADC0.1 */>;
           pinctrl-names = "default";
           status = "okay";
           #address-cells = <1>;
           #size-cells = <0>;

           channel@0 {
                   reg = <0>;
                   zephyr,gain = "ADC_GAIN_1";
                   zephyr,reference = "ADC_REF_INTERNAL";
                   zephyr,acquisition-time = <ADC_ACQ_TIME(ADC_ACQ_TIME_NANOSECONDS, 63)>;
                   zephyr,resolution = <12>;
           };

           channel@1 {
                   reg = <1>;
                   zephyr,gain = "ADC_GAIN_1";
                   zephyr,reference = "ADC_REF_INTERNAL";
                   zephyr,acquisition-time = <ADC_ACQ_TIME(ADC_ACQ_TIME_NANOSECONDS, 63)>;
                   zephyr,resolution = <12>;
           };
   };

.. code-block:: kconfig

   CONFIG_ADC=y
   CONFIG_REGULATOR=y

``CONFIG_REGULATOR=y`` is only needed for ``ADC_REF_INTERNAL`` and ``ADC_REF_EXTERNAL0``. Both
require ``CONFIG_REGULATOR_MSPM0_VREF``, which is enabled by default when the ``vref0`` node is
enabled. ``ADC_REF_VDD_1`` does not need it. For DMA, also set:

.. code-block:: kconfig

   CONFIG_DMA=y
   CONFIG_ADC_MSPM0_ADC12_DMA=y

DMA is used only for instances that have a ``dmas`` property.

Reading a Sequence
------------------

Configure channels once, from devicetree with ``ADC_DT_SPEC_GET()`` or at run time with
:c:func:`adc_channel_setup`. A read converts all channels in ``channels`` in ascending order and
stores one ``uint16_t`` per channel.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/adc.h>

   static const struct adc_dt_spec adc_channels[] = {
           ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
           ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
   };

   int main(void)
   {
           uint16_t buf[ARRAY_SIZE(adc_channels)];
           struct adc_sequence sequence = {
                   .buffer = buf,
                   .buffer_size = sizeof(buf),
                   .resolution = 12,
                   .oversampling = 3,  /* 2^3 = 8 samples averaged */
           };

           for (size_t i = 0; i < ARRAY_SIZE(adc_channels); i++) {
                   if (!adc_is_ready_dt(&adc_channels[i]) ||
                       adc_channel_setup_dt(&adc_channels[i]) < 0) {
                           return -1;
                   }
                   sequence.channels |= BIT(adc_channels[i].channel_id);
           }

           while (1) {
                   if (adc_read(adc_channels[0].dev, &sequence) == 0) {
                           int32_t mv = buf[0];

                           adc_raw_to_millivolts_dt(&adc_channels[0], &mv);
                           printk("ch0 = %d mV\n", mv);
                   }
                   k_msleep(500);
           }
   }


Samples
-------

- :ws-doc:`ADC devicetree driver sample <zephyr/samples/drivers/adc/adc_dt/README.html>`
- :ws-doc:`ADC driver sequence sample <zephyr/samples/drivers/adc/adc_sequence/README.html>`
- :ti-ref:`MSPM0 ADC sampling with power management <mspm0_adc_uart_pm>`

Devicetree Bindings
===================

- :ws-doc:`ti,mspm0-adc12 <zephyr/build/dts/api/bindings/adc/ti,mspm0-adc12.html>`
