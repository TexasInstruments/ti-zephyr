.. ti-socs: mspm0

.. _ti-zephyr-component-dac:

DAC
###

.. rst-class:: ti-subtitle

`DAC Zephyr Documentation <https://docs.zephyrproject.org/4.4.0/hardware/peripherals/dac.html>`_

.. important::

   The DAC driver will only work for MSPM0G series devices (``lp_mspm0g3507``
   and ``lp_mspm0g3519``).

.. contents:: 
   :depth: 1
   :local:
   :class: hidden-title

Features Supported
******************

- DAC direct CPU based output to the DAC_OUT
- DAC fifo based output

Features NOT Supported
***********************

- DAC DMA based output

Usage
*****

The driver implements Zephyr's standard DAC API (``<zephyr/drivers/dac.h>``).
The application calls :c:func:`dac_channel_setup` once and then calls
:c:func:`dac_write_value` for every sample it wants to output. Which of the two
output modes is used is selected in devicetree, not at run time:

- no ``fifo-enable`` property: :ref:`direct CPU based output
  <ti-zephyr-dac-direct>`
- ``fifo-enable`` property set: :ref:`FIFO based output <ti-zephyr-dac-fifo>`

Enabling the Driver
===================

Enable the DAC node in the board devicetree (or an application overlay) and the
DAC subsystem in ``prj.conf``. ``CONFIG_DAC_MSPM0`` is selected automatically
once an enabled ``ti,mspm0-dac`` node is present.

.. code-block:: devicetree

   &dac0 {
           status = "okay";
   };

.. code-block:: kconfig

   CONFIG_DAC=y

Devicetree Properties
=====================

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Property
     - Description
   * - ``vref``
     - Optional phandle to an external reference. When present, ``VEREFP`` and
       ``VEREFN`` are used as the DAC reference; otherwise ``VDDA`` and
       ``VSSA`` are used.
   * - ``fifo-enable``
     - Enables the 4-slot hardware FIFO. The sample time generator then clocks
       samples out of the FIFO at ``sample-rate-hz``, independent of when the
       CPU writes them.
   * - ``sample-rate-hz``
     - Output sample rate used in FIFO mode. Must be set whenever
       ``fifo-enable`` is set (otherwise :c:func:`dac_channel_setup` fails with
       ``-EINVAL``). Allowed values are 500, 1000, 2000, 4000, 8000, 16000,
       100000, 200000, 500000 and 1000000.

Channel Setup
=============

The DAC has a single output channel. :c:func:`dac_channel_setup` takes a
``struct dac_channel_cfg`` with the following constraints:

- ``channel_id`` must be ``0``.
- ``resolution`` must be ``8`` or ``12`` bits. Values passed to
  :c:func:`dac_write_value` must fit the chosen resolution (at most ``255`` for
  8 bit and ``4095`` for 12 bit), otherwise ``-EINVAL`` is returned.
- ``buffered`` enables the output amplifier and runs its self-calibration.
- ``internal`` set to ``false`` routes the output to the DAC_OUT pin.

.. _ti-zephyr-dac-direct:

Direct CPU Based Output
=======================

When ``fifo-enable`` is not set, every call to :c:func:`dac_write_value` writes
the value straight into the DAC data register and the DAC_OUT pin updates
right away. The CPU decides when each sample is output, so the application sets
the timing itself (for example with ``k_sleep()`` or a timer) and the samples
are written one after another, in call order. No sample rate is configured and
no interrupt is used.

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/dac.h>

   static const struct device *const dac_dev = DEVICE_DT_GET_ONE(ti_mspm0_dac);

   static const struct dac_channel_cfg dac_cfg = {
           .channel_id = 0,
           .resolution = 12,
           .buffered = true,
           .internal = false,
   };

   int main(void)
   {
           if (!device_is_ready(dac_dev) ||
               dac_channel_setup(dac_dev, &dac_cfg) != 0) {
                   return -1;
           }

           for (uint32_t value = 0; ; value = (value + 1) % 4096) {
                   dac_write_value(dac_dev, 0, value);
                   k_msleep(1);
           }
   }

.. _ti-zephyr-dac-fifo:

FIFO Based Output
=================

When ``fifo-enable`` is set, :c:func:`dac_write_value` pushes the sample into the
4-slot hardware FIFO instead of driving the output directly. The sample time
generator takes one sample out of the FIFO every ``1 / sample-rate-hz`` seconds
and presents it on DAC_OUT, so the output timing no longer depends on when the
CPU writes.

.. code-block:: devicetree

   &dac0 {
           status = "okay";
           fifo-enable;
           sample-rate-hz = <8000>;
   };

.. code-block:: text

   dac_write_value() --> [ FIFO: 4 slots ] --> sample time generator --> DAC_OUT
                                ^                  (sample-rate-hz)
                                |
                       FIFO empty interrupt
                       releases a blocked writer

The writer and the hardware cooperate as follows:

#. Each call to :c:func:`dac_write_value` adds one sample to the FIFO and returns
   immediately, until the FIFO is full.
#. Once the FIFO is full, the next call to :c:func:`dac_write_value` does not
   write. It enables the FIFO empty interrupt and blocks the calling thread until
   the FIFO has been emptied by the output side.
#. When the FIFO becomes empty, the interrupt fires, the driver disables the
   interrupt again and wakes the blocked writer.
#. The writer carries on filling the FIFO while the sample time generator keeps
   draining it at ``sample-rate-hz``, so the FIFO is being emptied from the
   output side while it is being refilled by the CPU.
#. If the writer fills the FIFO faster than it drains, it reaches the full state
   again and stops. The next call blocks as in step 2 and waits for the FIFO empty
   interrupt before writing resumes.

The application code is the same as for direct output; the loop is paced by the
FIFO instead of by ``k_msleep()``:

.. code-block:: c

   for (uint32_t value = 0; ; value = (value + 1) % 4096) {
           dac_write_value(dac_dev, 0, value);
   }

.. note::

   - :c:func:`dac_write_value` can block indefinitely in FIFO mode, so it must be
     called from thread context and not from an interrupt handler.
   - The blocked writer is released only when the FIFO is completely empty. At
     high sample rates the CPU may not be able to refill the FIFO before the next
     sample is due, so gap-free output is not guaranteed.

Samples and Tests
******************
Samples
=======

- `DAC Sample (Outputs Sawtooth Wave) <https://docs.zephyrproject.org/4.4.0/samples/drivers/dac/README.html#dac>`_

Tests
=====

- ``dac_api test`` - Calls and tests DAC APIs
- ``dac_loopback test`` - Reads back DAC driver output with ADC and verify it is within the expected range
