/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#include <stdio.h>

#include <gui_composer/gui_composer.h>
#include <gui_composer/transport.h>

#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>

#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
#include "counters.h"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static void on_enable(gui_rx_value_t value)
{
	bool enable = guicomm_read_bool(value);
	int ret = gpio_pin_set_dt(&led, enable);

	if (ret != 0) {
		printf("gpio_pin_set_dt failed: %d\n", ret);
	}
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	counters_set_enable(enable);
#endif
}

#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
static void on_u16_data(gui_rx_value_t value)
{
	counters_set_increment(guicomm_read_uint16(value));
}
#endif

static const gui_rx_cmd_t rx_cmds[] = {
	{"bEnable", GUI_RX_BOOL, on_enable},
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	{"u16Data", GUI_RX_NUMBER, on_u16_data},
#endif
};

int main(void)
{
	printf("\n[simple_gui_demo] boot\n");

	if (!gpio_is_ready_dt(&led)) {
		printf("led0 gpio not ready\n");
	}

	int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	if (ret != 0) {
		printf("gpio_pin_configure_dt failed: %d\n", ret);
	}

	gui_composer_transport_init();
	gui_init();
	gui_init_rx_cmd(rx_cmds, ARRAY_SIZE(rx_cmds));

	guicomm_send_bool("bEnable", 7, false);
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	counters_init();
#endif

	return 0;
}
