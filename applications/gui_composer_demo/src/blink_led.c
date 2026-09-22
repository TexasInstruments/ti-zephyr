/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "blink_led.h"
#include "demo_mode.h"
#include <gui_composer/gui_composer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

static bool led_on;

static volatile uint32_t blink_period_ms = CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK_DEFAULT_PERIOD_MS;

void blink_led_set_period(uint32_t period_ms)
{
	if (period_ms > 0) {
		blink_period_ms = period_ms;
	}
}

void blink_led_run(void)
{
	gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);
	led_on = false;
	guicomm_send_bool("blinkEnable", 11, true);
	guicomm_send_uint16("blinkTick", 9, (uint16_t)(blink_period_ms * 256 / 1000));

	while (current_mode == BLINK_LED_MODE) {
		k_sleep(K_MSEC(blink_period_ms));
		if (current_mode != BLINK_LED_MODE) {
			break;
		}

		gpio_pin_toggle_dt(&led_green);
		led_on = !led_on;
		guicomm_send_bool("ledOn", 5, led_on);
	}

	gpio_pin_set_dt(&led_green, 0);
	guicomm_send_bool("blinkEnable", 11, false);
}
