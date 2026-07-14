/*
 * Copyright (c) 2024 Texas Instruments Incorporated
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>

#define UART_NODE DT_CHOSEN(zephyr_console)

static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

static void uart_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	uart_irq_update(dev);
	while (uart_irq_rx_ready(dev)) {
		uart_fifo_read(dev, &c, 1);
		uart_fifo_fill(dev, &c, 1);
	}
}

int main(void)
{
	if (!device_is_ready(uart_dev)) {
		return 0;
	}
	uart_irq_callback_set(uart_dev, uart_cb);
	uart_irq_rx_enable(uart_dev);
	printf("UART echo ready\n");
	return 0;
}
