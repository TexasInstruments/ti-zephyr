/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include <gui_composer/gui_composer.h>
#include <ti/gui_composer/transport.h>

#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>

#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
#include "counters.h"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

#if defined(CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK)
static void on_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	bool enable = guicomm_read_bool(tag);
	int ret = gpio_pin_set_dt(&led, enable);

	if (ret != 0) {
		printf("gpio_pin_set_dt failed: %d\n", ret);
	}
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	counters_set_enable(enable);
#endif
}

#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
static void on_u16_data(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	counters_set_increment(guicomm_read_uint16(tag));
}
#endif
#elif defined(CONFIG_GUI_COMPOSER_PROTOCOL_JSON)
static void on_enable(char *string)
{
	bool enable = guicomm_read_bool(string);
	int ret = gpio_pin_set_dt(&led, enable);

	if (ret != 0) {
		printf("gpio_pin_set_dt failed: %d\n", ret);
	}
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	counters_set_enable(enable);
#endif
}

#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
static void on_u16_data(char *string)
{
	counters_set_increment(guicomm_read_uint16(string));
}
#endif
#endif

static const gui_rx_cmd_t rx_cmds[] = {
	{"bEnable", on_enable},
#if defined(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS)
	{"u16Data", on_u16_data},
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
