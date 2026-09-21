/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "counters.h"
#include <gui_composer/gui_composer.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define Q8_ONE 256
#define Q8_MAX (100 * Q8_ONE)

static const struct gpio_dt_spec sw1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec sw2 = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static struct gpio_callback sw1_cb;
static struct gpio_callback sw2_cb;

static uint8_t u8_counter = CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U8_STEP;
static uint16_t u16_counter = CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U16_STEP;
static uint32_t u32_counter = CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U32_STEP;
static int16_t q8_counter = Q8_ONE / 2;
static int16_t q8_increment = CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_Q8_DEFAULT_INCREMENT;
static bool q8_counter_enabled;

static struct k_work send_work;

static void timer_expired(struct k_timer *timer)
{
	ARG_UNUSED(timer);

	u32_counter += CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U32_STEP;
	if (q8_counter_enabled) {
		q8_counter += q8_increment;
		if (q8_counter > Q8_MAX) {
			q8_counter = 0;
		}
	}

	k_work_submit(&send_work);
}

K_TIMER_DEFINE(counters_timer, timer_expired, NULL);

static void send_work_handler(struct k_work *work)
{
	ARG_UNUSED(work);

	guicomm_send_uint8("c1", 2, u8_counter);
	guicomm_send_uint16("c2", 2, u16_counter);
	guicomm_send_uint32("c3", 2, u32_counter);
	if (q8_counter_enabled) {
		guicomm_send_int16("c4", 2, q8_counter);
	}
}

static void sw1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	u16_counter += CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U16_STEP;
}

static void sw2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	u8_counter += CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_U8_STEP;
}

void counters_set_enable(bool enable)
{
	q8_counter_enabled = enable;
}

void counters_set_increment(uint16_t increment)
{
	q8_increment = (int16_t)increment;
}

void counters_init(void)
{
	int ret = gpio_pin_configure_dt(&sw1, GPIO_INPUT);

	if (ret != 0) {
		printf("sw1 gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_interrupt_configure_dt(&sw1, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printf("sw1 gpio_pin_interrupt_configure_dt failed: %d\n", ret);
	}
	gpio_init_callback(&sw1_cb, sw1_pressed, BIT(sw1.pin));
	ret = gpio_add_callback(sw1.port, &sw1_cb);
	if (ret != 0) {
		printf("sw1 gpio_add_callback failed: %d\n", ret);
	}

	ret = gpio_pin_configure_dt(&sw2, GPIO_INPUT);
	if (ret != 0) {
		printf("sw2 gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_interrupt_configure_dt(&sw2, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printf("sw2 gpio_pin_interrupt_configure_dt failed: %d\n", ret);
	}
	gpio_init_callback(&sw2_cb, sw2_pressed, BIT(sw2.pin));
	ret = gpio_add_callback(sw2.port, &sw2_cb);
	if (ret != 0) {
		printf("sw2 gpio_add_callback failed: %d\n", ret);
	}

	guicomm_send_uint8("c1", 2, u8_counter);
	guicomm_send_uint16("c2", 2, u16_counter);
	guicomm_send_uint32("c3", 2, u32_counter);
	guicomm_send_int16("c4", 2, q8_counter);
	guicomm_send_int16("u16Data", 7, q8_increment);

	k_work_init(&send_work, send_work_handler);
	k_timer_start(&counters_timer,
		      K_MSEC(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS),
		      K_MSEC(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_PERIOD_MS));
}
