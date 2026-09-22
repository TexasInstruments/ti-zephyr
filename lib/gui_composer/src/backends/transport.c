/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <ti/gui_composer/transport.h>

#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#if !DT_HAS_CHOSEN(zephyr_gui_composer_uart)
#error "Add to a board/app devicetree overlay: " \
	"/ { chosen { zephyr,gui-composer-uart = <&your_uart_node>; }; };"
#endif

static const struct device *const gui_uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_gui_composer_uart));

static guicomm_rx_char_callback_t rx_byte_callback;

static void gui_composer_uart_isr(const struct device *dev, void *user_data)
{
	ARG_UNUSED(user_data);
	uint8_t byte;

	while (uart_irq_update(dev), uart_irq_is_pending(dev)) {
		if (uart_irq_rx_ready(dev)) {
			while (uart_fifo_read(dev, &byte, 1) == 1) {
				if (rx_byte_callback != NULL) {
					(void)rx_byte_callback((char)byte);
				}
			}
		}
	}
}

void gui_composer_transport_init(void)
{
	if (!device_is_ready(gui_uart_dev)) {
		printk("GUI Composer UART not ready\n");
	}
}

void gui_composer_transport_start(guicomm_rx_char_callback_t rx_char_cb)
{
	rx_byte_callback = rx_char_cb;

	uart_irq_rx_disable(gui_uart_dev);
	uart_irq_tx_disable(gui_uart_dev);
	uart_irq_callback_set(gui_uart_dev, gui_composer_uart_isr);
	uart_irq_rx_enable(gui_uart_dev);
}

void gui_composer_transport_send_char(char character)
{
	uart_poll_out(gui_uart_dev, (uint8_t)character);
}
