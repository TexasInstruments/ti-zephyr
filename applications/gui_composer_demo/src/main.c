/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
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
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE)
#include "temperature.h"
#endif
#include <gui_composer/gui_composer.h>
#include <gui_composer/transport.h>

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

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK)
static void on_blink_enable(gui_rx_value_t value)
{
	current_mode = guicomm_read_bool(value) ? BLINK_LED_MODE : IDLE;
}

static void on_blink_tick(gui_rx_value_t value)
{
	float delay_s = guicomm_read_uint16(value) / 256.0f;

	blink_led_set_period((uint32_t)(delay_s * 1000.0f));
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
static void on_therm_enable(gui_rx_value_t value)
{
	current_mode = guicomm_read_bool(value) ? THERMISTOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
static void on_light_enable(gui_rx_value_t value)
{
	current_mode = guicomm_read_bool(value) ? LIGHTSENSOR_MODE : IDLE;
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE)
static void on_temperature_enable(gui_rx_value_t value)
{
	current_mode = guicomm_read_bool(value) ? TEMPERATURE_MODE : IDLE;
}

static void on_temperature_units(gui_rx_value_t value)
{
	temperature_set_units(guicomm_read_bool(value));
}
#endif

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
static void on_dac_enable(gui_rx_value_t value)
{
	current_mode = guicomm_read_bool(value) ? FNGEN_MODE : IDLE;
}

static void on_set_type(gui_rx_value_t value)
{
	function_generator_set_wave_type(guicomm_read_uint16(value));
}

static void on_set_freq(gui_rx_value_t value)
{
	function_generator_set_frequency(guicomm_read_uint16(value));
}

static void on_set_amp(gui_rx_value_t value)
{
	function_generator_set_amplitude(guicomm_read_uint16(value));
}

static void on_adc_enable(gui_rx_value_t value)
{
	function_generator_set_adc_enable(guicomm_read_bool(value));
}

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
static void on_set_gain(gui_rx_value_t value)
{
	function_generator_set_gain(guicomm_read_uint16(value));
}

static void on_inversion(gui_rx_value_t value)
{
	function_generator_set_inversion(guicomm_read_uint16(value));
}
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
	FNGEN_MODE,
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE)
	TEMPERATURE_MODE,
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
	{"blinkEnable", GUI_RX_BOOL, on_blink_enable},
	{"blinkTick", GUI_RX_NUMBER, on_blink_tick},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR)
	{"thermEnable", GUI_RX_BOOL, on_therm_enable},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR)
	{"lightEnable", GUI_RX_BOOL, on_light_enable},
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR)
	{"dacEnable", GUI_RX_BOOL, on_dac_enable},
	{"setType", GUI_RX_NUMBER, on_set_type},
	{"setFreq", GUI_RX_NUMBER, on_set_freq},
	{"setAmp", GUI_RX_NUMBER, on_set_amp},
	{"adcEnable", GUI_RX_BOOL, on_adc_enable},
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
	{"setGain", GUI_RX_NUMBER, on_set_gain},
	{"inversion", GUI_RX_NUMBER, on_inversion},
#endif
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE)
	{"temperatureEnable", GUI_RX_BOOL, on_temperature_enable},
	{"tUnits", GUI_RX_BOOL, on_temperature_units},
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
		case FNGEN_MODE:
			function_generator_run();
			break;
#endif
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE)
		case TEMPERATURE_MODE:
			temperature_run();
			break;
#endif
		case IDLE:
			idle_run();
			break;
		default:
			printf("unsupported mode %d\n", current_mode);
			current_mode = IDLE;
			break;
		}
	}

	return 0;
}
