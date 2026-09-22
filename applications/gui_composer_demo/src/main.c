/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "demo_mode.h"
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
#include "blink_led.h"
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
#include "function_generator.h"
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
#include "lightsensor.h"
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
#include "thermistor.h"
#endif
#include <gui_composer/gui_composer.h>
#include <ti/gui_composer/transport.h>

#include <stdio.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

volatile enum demo_mode current_mode = IDLE;

static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);
static const struct gpio_dt_spec led_heartbeat = GPIO_DT_SPEC_GET(DT_NODELABEL(led_heartbeat), gpios);
static const struct gpio_dt_spec button_s2 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static struct gpio_callback button_s2_cb;

static void idle_run(void)
{
	while (current_mode == IDLE) {
		gpio_pin_toggle_dt(&led_heartbeat);
		k_sleep(K_MSEC(500));
	}
}

#if defined(CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK)
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
static void on_blink_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	current_mode = guicomm_read_bool(tag) ? BLINK_LED_MODE : IDLE;
}

static void on_blink_tick(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	float delay_s = guicomm_read_uint16(tag) / 256.0f;

	blink_led_set_period((uint32_t)(delay_s * 1000.0f));
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
static void on_therm_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	current_mode = guicomm_read_bool(tag) ? THERMISTOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
static void on_light_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	current_mode = guicomm_read_bool(tag) ? LIGHTSENSOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
static void on_dac_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	current_mode = guicomm_read_bool(tag) ? DAC_MODE : IDLE;
}

static void on_set_type(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_wave_type(guicomm_read_uint16(tag));
}

static void on_set_freq(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_frequency(guicomm_read_uint16(tag));
}

static void on_set_amp(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_amplitude(guicomm_read_uint16(tag));
}

static void on_adc_enable(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_adc_enable(guicomm_read_bool(tag));
}

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
static void on_set_gain(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_gain(guicomm_read_uint16(tag));
}

static void on_inversion(mpack_tag_t *tag, mpack_reader_t *reader)
{
	ARG_UNUSED(reader);
	function_generator_set_inversion(guicomm_read_uint16(tag));
}
#endif
#endif
#elif defined(CONFIG_GUI_COMPOSER_PROTOCOL_JSON)
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
static void on_blink_enable(char *string)
{
	current_mode = guicomm_read_bool(string) ? BLINK_LED_MODE : IDLE;
}

static void on_blink_tick(char *string)
{
	float delay_s = guicomm_read_uint16(string) / 256.0f;

	blink_led_set_period((uint32_t)(delay_s * 1000.0f));
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
static void on_therm_enable(char *string)
{
	current_mode = guicomm_read_bool(string) ? THERMISTOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
static void on_light_enable(char *string)
{
	current_mode = guicomm_read_bool(string) ? LIGHTSENSOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
static void on_dac_enable(char *string)
{
	current_mode = guicomm_read_bool(string) ? DAC_MODE : IDLE;
}

static void on_set_type(char *string)
{
	function_generator_set_wave_type(guicomm_read_uint16(string));
}

static void on_set_freq(char *string)
{
	function_generator_set_frequency(guicomm_read_uint16(string));
}

static void on_set_amp(char *string)
{
	function_generator_set_amplitude(guicomm_read_uint16(string));
}

static void on_adc_enable(char *string)
{
	function_generator_set_adc_enable(guicomm_read_bool(string));
}

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
static void on_set_gain(char *string)
{
	function_generator_set_gain(guicomm_read_uint16(string));
}

static void on_inversion(char *string)
{
	function_generator_set_inversion(guicomm_read_uint16(string));
}
#endif
#endif
#endif

#define BUTTON_S2_DEBOUNCE_MS 50

static const enum demo_mode cycle_modes[] = {
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
	BLINK_LED_MODE,
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
	LIGHTSENSOR_MODE,
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
	THERMISTOR_MODE,
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
	DAC_MODE,
#endif
};

static void button_s2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	static int64_t last_press_ms;
	int64_t now = k_uptime_get();

	if (now - last_press_ms < BUTTON_S2_DEBOUNCE_MS) {
		return;
	}
	last_press_ms = now;

	if (ARRAY_SIZE(cycle_modes) == 0) {
		current_mode = IDLE;
		return;
	}

	if (current_mode == IDLE) {
		current_mode = cycle_modes[0];
		return;
	}

	for (size_t i = 0; i < ARRAY_SIZE(cycle_modes); i++) {
		if (cycle_modes[i] == current_mode) {
			current_mode = (i + 1 < ARRAY_SIZE(cycle_modes)) ? cycle_modes[i + 1] : IDLE;
			return;
		}
	}

	current_mode = cycle_modes[0];
}

static const gui_rx_cmd_t rx_cmds[] = {
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
	{"blinkEnable", on_blink_enable},
	{"blinkTick", on_blink_tick},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
	{"thermEnable", on_therm_enable},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
	{"lightEnable", on_light_enable},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
	{"dacEnable", on_dac_enable},
	{"setType", on_set_type},
	{"setFreq", on_set_freq},
	{"setAmp", on_set_amp},
	{"adcEnable", on_adc_enable},
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
	{"setGain", on_set_gain},
	{"inversion", on_inversion},
#endif
#endif
};

int main(void)
{
	printf("\n[gui_composer_demo] boot\n");

	int ret = gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);

	if (ret != 0) {
		printf("led_green gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		printf("led_red gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		printf("led_blue gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_configure_dt(&led_heartbeat, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		printf("led_heartbeat gpio_pin_configure_dt failed: %d\n", ret);
	}

	ret = gpio_pin_configure_dt(&button_s2, GPIO_INPUT);
	if (ret != 0) {
		printf("button_s2 gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_interrupt_configure_dt(&button_s2, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printf("button_s2 gpio_pin_interrupt_configure_dt failed: %d\n", ret);
	}
	gpio_init_callback(&button_s2_cb, button_s2_pressed, BIT(button_s2.pin));
	ret = gpio_add_callback(button_s2.port, &button_s2_cb);
	if (ret != 0) {
		printf("gpio_add_callback failed: %d\n", ret);
	}

	gui_composer_transport_init();
	gui_init();
	gui_init_rx_cmd(rx_cmds, ARRAY_SIZE(rx_cmds));

	while (1) {
		switch (current_mode) {
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
		case BLINK_LED_MODE:
			blink_led_run();
			break;
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
		case LIGHTSENSOR_MODE:
			lightsensor_run();
			break;
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
		case THERMISTOR_MODE:
			thermistor_run();
			break;
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
		case DAC_MODE:
			function_generator_run();
			break;
#endif
		case IDLE:
		default:
			idle_run();
			break;
		}
	}

	return 0;
}
