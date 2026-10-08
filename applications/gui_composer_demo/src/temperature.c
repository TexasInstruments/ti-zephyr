/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#include "temperature.h"

#include "adc_sample.h"
#include "demo_mode.h"
#include <gui_composer/gui_composer.h>

#include <stdio.h>

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/syscon.h>
#include <zephyr/kernel.h>

#define TEMP_SAMPLE_PERIOD_MS \
	CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE_SAMPLE_PERIOD_MS
#define TEMP_DELTA_THRESHOLD_C \
	CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE_DELTA_THRESHOLD_C
#if defined(CONFIG_SOC_SERIES_MSPM0G) || defined(CONFIG_SOC_SERIES_MSPM0L)
#define TEMP_ADC_BIT_RESOLUTION (1 << 12)
#define TEMP_ADC_VREF_VOLTAGE 1.4f
#define TEMP_TS_COEF_MV_C (-571.42f)
#define TEMP_TS_TRIM_C 25.0f
#else
#error "Define TEMP_ADC/TS calibration constants for this SoC"
#endif
#define TEMP_BUTTON_S1_DEBOUNCE_MS 200

static const struct adc_dt_spec adc_temp =
	ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), temperature);
static const struct gpio_dt_spec led_red =
	GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec led_green =
	GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_blue =
	GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

static int16_t adc_raw;
static struct adc_sequence sequence = {
	.buffer = &adc_raw,
	.buffer_size = sizeof(adc_raw),
};

static bool temperature_fahrenheit;

void temperature_set_units(bool fahrenheit)
{
	temperature_fahrenheit = fahrenheit;
}

#if DT_NODE_HAS_STATUS(DT_ALIAS(sw1), okay)
#define TEMP_HAS_BUTTON_S1 1

static const struct gpio_dt_spec button_s1 =
	GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static struct gpio_callback button_s1_cb;
static int64_t button_s1_last_press_ms;

static void button_s1_pressed(const struct device *dev,
			       struct gpio_callback *cb, uint32_t pins)
{
	int64_t now = k_uptime_get();

	if (now - button_s1_last_press_ms < TEMP_BUTTON_S1_DEBOUNCE_MS) {
		return;
	}
	button_s1_last_press_ms = now;

	temperature_fahrenheit = !temperature_fahrenheit;
	guicomm_send_bool("tUnits", 6, temperature_fahrenheit);
}
#else
#define TEMP_HAS_BUTTON_S1 0
#endif

#if defined(CONFIG_SOC_SERIES_MSPM0G) || defined(CONFIG_SOC_SERIES_MSPM0L)
#define TEMP_FACTORYREGION_TEMP_SENSE0_OFFSET 0x3CUL
#define TEMP_CPUSS_CTL_OFFSET 0x1300UL
#define TEMP_CPUSS_CTL_ICACHE_MASK 0x2UL
#define TEMP_CPUSS_CTL_ICACHE_DISABLE 0x0UL
#else
#error "Define TEMP_CPUSS/FACTORYREGION syscon offsets for this SoC"
#endif

static const struct device *const temp_cpuss_syscon =
	DEVICE_DT_GET(DT_NODELABEL(cpuss_syscon));
static const struct device *const temp_factoryregion_syscon =
	DEVICE_DT_GET(DT_NODELABEL(factoryregion_syscon));

static uint32_t temperature_read_calibration_constant(void)
{
	uint32_t prev_ctl = 0;
	uint32_t cal = 0;
	int ret;

	ret = syscon_read_reg(temp_cpuss_syscon, TEMP_CPUSS_CTL_OFFSET,
			       &prev_ctl);
	if (ret != 0) {
		printf("syscon_read_reg(cpuss) failed: %d\n", ret);
	}

	ret = syscon_update_bits(temp_cpuss_syscon, TEMP_CPUSS_CTL_OFFSET,
				  TEMP_CPUSS_CTL_ICACHE_MASK,
				  TEMP_CPUSS_CTL_ICACHE_DISABLE);
	if (ret != 0) {
		printf("syscon_update_bits(cpuss) failed: %d\n", ret);
	}

	ret = syscon_read_reg(temp_factoryregion_syscon,
			       TEMP_FACTORYREGION_TEMP_SENSE0_OFFSET, &cal);
	if (ret != 0) {
		printf("syscon_read_reg(factoryregion) failed: %d\n", ret);
	}

	ret = syscon_write_reg(temp_cpuss_syscon, TEMP_CPUSS_CTL_OFFSET,
				prev_ctl);
	if (ret != 0) {
		printf("syscon_write_reg(cpuss) failed: %d\n", ret);
	}

	return cal;
}

static float temperature_calc_celsius_full(uint16_t raw_adc)
{
	uint32_t cal = temperature_read_calibration_constant();
	float vtrim = (TEMP_ADC_VREF_VOLTAGE / TEMP_ADC_BIT_RESOLUTION) *
		      (cal - 0.5f);
	float vsample = (TEMP_ADC_VREF_VOLTAGE / TEMP_ADC_BIT_RESOLUTION) *
			(raw_adc - 0.5f);

	return (1.0f / TEMP_TS_COEF_MV_C * 1000.0f) * (vsample - vtrim) +
	       TEMP_TS_TRIM_C;
}

static void set_rgb(bool red, bool green, bool blue)
{
	gpio_pin_set_dt(&led_red, red);
	gpio_pin_set_dt(&led_green, green);
	gpio_pin_set_dt(&led_blue, blue);
}

void temperature_run(void)
{
	int32_t initial_reading = 0;
	bool first_reading = true;

	gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE);

	int ret = adc_channel_setup_dt(&adc_temp);

	if (ret != 0) {
		printf("adc_channel_setup_dt failed: %d\n", ret);
	}

#if TEMP_HAS_BUTTON_S1
	ret = gpio_pin_configure_dt(&button_s1, GPIO_INPUT);
	if (ret != 0) {
		printf("button_s1 gpio_pin_configure_dt failed: %d\n", ret);
	}
	ret = gpio_pin_interrupt_configure_dt(&button_s1,
					       GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printf("button_s1 gpio_pin_interrupt_configure_dt failed: %d\n",
		       ret);
	}
	gpio_init_callback(&button_s1_cb, button_s1_pressed,
			   BIT(button_s1.pin));
	ret = gpio_add_callback(button_s1.port, &button_s1_cb);
	if (ret != 0) {
		printf("button_s1 gpio_add_callback failed: %d\n", ret);
	}
#endif

	(void)adc_sample_dt(&adc_temp, &sequence);

	guicomm_send_bool("temperatureEnable", 17, true);
	guicomm_send_int16("tValue", 6, 0);

	while (current_mode == TEMPERATURE_MODE) {
		ret = adc_sample_dt(&adc_temp, &sequence);
		if (ret != 0) {
			printf("adc read failed: %d\n", ret);
			k_sleep(K_MSEC(TEMP_SAMPLE_PERIOD_MS));
			continue;
		}

		int32_t celsius_reading = (int32_t)
			temperature_calc_celsius_full((uint16_t)adc_raw);

		if (first_reading) {
			initial_reading = celsius_reading;
			first_reading = false;
		}

		int32_t sent_reading = celsius_reading;

		if (temperature_fahrenheit) {
			sent_reading = (int32_t)
				(celsius_reading * 9.0f / 5.0f + 32.0f);
		}

		guicomm_send_int16("tValue", 6, (int16_t)sent_reading);

		if (celsius_reading >
		    initial_reading + TEMP_DELTA_THRESHOLD_C) {
			set_rgb(true, false, false);
		} else if (celsius_reading <
			   initial_reading - TEMP_DELTA_THRESHOLD_C) {
			set_rgb(false, false, true);
		} else {
			set_rgb(false, true, false);
		}

		k_sleep(K_MSEC(TEMP_SAMPLE_PERIOD_MS));
	}

	set_rgb(false, false, false);
	guicomm_send_int16("tValue", 6, 0);
	guicomm_send_bool("temperatureEnable", 17, false);

#if TEMP_HAS_BUTTON_S1
	gpio_pin_interrupt_configure_dt(&button_s1, GPIO_INT_DISABLE);
	gpio_remove_callback(button_s1.port, &button_s1_cb);
#endif
}
