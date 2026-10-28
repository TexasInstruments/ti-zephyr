Features Supported
******************

- Zephyr DMA API (``<zephyr/drivers/dma.h>``): :c:func:`dma_config`, :c:func:`dma_start`,
  :c:func:`dma_stop`, :c:func:`dma_suspend`, :c:func:`dma_resume`, :c:func:`dma_reload`,
  :c:func:`dma_get_status` and channel allocation through :c:func:`dma_request_channel` (the channel
  filter accepts an optional pointer to a channel number to request that channel)
- Memory-to-memory, memory-to-peripheral and peripheral-to-memory transfers.
- Multi-block transfers: the interrupt handler walks the ``next_block`` chain of ``struct
  dma_block_config`` and reloads the channel with the next block each time one completes. With
  ``complete_callback_en`` the callback is called after every block, otherwise only after the last.
- Cyclic transfers (``cyclic = true``) on full channels. The channel is not disabled on completion,
  and the callback is called with ``DMA_STATUS_COMPLETE`` on every pass over the buffer.
- Half-complete notification (``half_complete_callback_en``) through the hardware early interrupt,
  reported as ``DMA_STATUS_HALF_COMPLETE``. Available on full channels 0 to 7.
- Channel linking through ``source_chaining_en`` / ``dest_chaining_en`` and ``linked_channel``:
  completion of the configured channel triggers the linked channel, which the driver enables.
- Source and destination data widths of 1, 2, 4 and 8 bytes, set independently (the hardware
  converts the width).
- Address adjustment ``DMA_ADDR_ADJ_INCREMENT``, ``DMA_ADDR_ADJ_NO_CHANGE`` and
  ``DMA_ADDR_ADJ_DECREMENT`` on source and destination. Hardware stride increments (2 to 9 elements)
  are selected through ``source_gather_interval`` / ``dest_scatter_interval`` when
  ``source_gather_en`` / ``dest_scatter_en`` is set, with a count of at most 1 and
  ``DMA_ADDR_ADJ_INCREMENT``.
- Extended modes on full channels, selected with the upper bits of ``dma_slot`` (see below):

  - fill: writes a pattern to a memory block
  - table: reads ``{address, data}`` pairs and writes each data word to its address (the driver sets
    a 64-bit source and a 32-bit destination width)
  - gather: copies data through an address table (32-bit width), only on instances with
    ``ti,gather-mode``

- Hardware or software triggers. The trigger number is the channel ``DMATSEL`` value. Trigger 0 is
  the software request, which :c:func:`dma_start` issues itself. Multi-block transfers on a
  software-triggered channel are re-requested automatically for each block.
- Error reporting: an address or data error interrupt invokes the callback of every busy channel
  with ``-EFAULT`` (address error) or ``-EIO`` (data error), unless that channel set
  ``error_callback_dis``.
- Instance-wide arbitration settings from devicetree: fixed or round-robin priority and the burst
  size before arbitration is re-evaluated.

Full and Basic Channels
=======================

The first ``ti,num-full-channels`` channels (from channel 0, the highest priority) are
full-featured. The remaining channels are basic and only support single and block transfers.
:c:func:`dma_config` returns ``-ENOTSUP`` when a basic channel is asked for:

- ``cyclic`` (repeated single or repeated block mode)
- an extended mode (fill, table or gather)
- ``half_complete_callback_en``

Triggers and Slots
==================

``dma_slot`` has two fields: bits 5:0 are the ``DMATSEL`` trigger and bits 7:6 are the extended
mode. ``TI_MSPM0_DMA_SLOT(trigger, mode)`` from ``<zephyr/dt-bindings/dma/ti-mspm0-dma.h>`` builds
the value, where ``mode`` is ``NORMAL``, ``FILL``, ``TABLE`` or ``GATHER``. A normal transfer needs
only the trigger number, which is the ``trigger`` cell of a ``dmas`` property.

Features NOT Supported
**********************

- Hardware features the driver does not expose:

  - DMA auto-enable
  - 128-bit (long long) transfers
  - the general event interrupt line (``GEN_EVENT``); only the CPU interrupt is used
  - per-channel priority; arbitration is fixed by channel number, or round-robin for all channels
    when ``ti,round-robin-priority`` is set
  - the early interrupt at positions other than half of the transfer

Usage
*****

Enabling the Driver
===================

``CONFIG_DMA_TI_MSPM0`` defaults to on when an enabled ``ti,mspm0-dma`` node exists. The application
only enables the DMA subsystem:

.. code-block:: kconfig

   CONFIG_DMA=y

A peripheral uses the controller through a ``dmas`` property with the channel and trigger as cells:

.. code-block:: devicetree

   &adc0 {
           dmas = <&dma 6 23>; /* channel 6, trigger 23 */
   };

A memory-to-memory transfer
===========================

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/dma.h>
   #include <zephyr/kernel.h>

   #define DMA_NODE DT_NODELABEL(dma)

   static K_SEM_DEFINE(done, 0, 1);
   static uint8_t src[64], dst[64];

   static void dma_cb(const struct device *dev, void *user_data,
                      uint32_t channel, int status)
   {
           if (status == DMA_STATUS_COMPLETE) {
                   k_sem_give(&done);
           }
   }

   int main(void)
   {
           const struct device *dma = DEVICE_DT_GET(DMA_NODE);
           uint32_t channel = 0;
           struct dma_block_config blk = {
                   .source_address = (uintptr_t)src,
                   .dest_address = (uintptr_t)dst,
                   .block_size = sizeof(src),
                   .source_addr_adj = DMA_ADDR_ADJ_INCREMENT,
                   .dest_addr_adj = DMA_ADDR_ADJ_INCREMENT,
           };
           struct dma_config cfg = {
                   .channel_direction = MEMORY_TO_MEMORY,
                   .source_data_size = 1,
                   .dest_data_size = 1,
                   .dma_slot = 0, /* software trigger */
                   .complete_callback_en = 1,
                   .dma_callback = dma_cb,
                   .block_count = 1,
                   .head_block = &blk,
           };

           if (!device_is_ready(dma) ||
               dma_request_channel(dma, &channel) < 0) {
                   return -1;
           }

           if (dma_config(dma, channel, &cfg) < 0 ||
               dma_start(dma, channel) < 0) {
                   return -1;
           }

           k_sem_take(&done, K_FOREVER);
           dma_release_channel(dma, channel);

           return 0;
   }

Notes on the configuration:

- ``block_size`` is in bytes and is divided by the data width, so it must be a multiple of the
  width.

Hardware Notes
==============

- Channels in a peripheral's ``dmas`` property are reserved by that driver. The ADC takes its
  channel with :c:func:`dma_request_channel` at init, so applications should request channels the
  same way instead of hard-coding one.

Peripherals Using the Driver
============================

Only the ADC12 driver (``ti,mspm0-adc12``) uses the DMA controller in the tree. It requires
``CONFIG_ADC_MSPM0_ADC12_DMA=y`` and a ``dmas`` property on the ADC node, and falls back to
interrupt mode if the channel cannot be set up. See the :ti-ref:`ADC page
<ti-zephyr-component-adc>`.

Devicetree Bindings
*******************

- :ws-doc:`ti,mspm0-dma <zephyr/build/dts/api/bindings/dma/ti,mspm0-dma.html>`
