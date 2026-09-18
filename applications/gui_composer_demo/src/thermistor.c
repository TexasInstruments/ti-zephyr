/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "thermistor.h"
#include "adc_sample.h"
#include "demo_mode.h"
#include <gui_composer/gui_composer.h>

#include <stdio.h>
#include <math.h>

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define THERM_VBIAS            3.30f
#define THERM_ADC_BITS         4096.0f
#define THERM_SAMPLE_PERIOD_MS CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_SAMPLE_PERIOD_MS
#define THERM_DELTA_THRESHOLD  CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR_DELTA_THRESHOLD_C

static const struct adc_dt_spec adc_therm = ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), thermistor);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

static int16_t adc_raw;
static struct adc_sequence sequence = {
	.buffer = &adc_raw,
	.buffer_size = sizeof(adc_raw),
};

static float thermistor_calc_temperature(int raw_adc)
{
	static const float a0 = -4.232811E+02f;
	static const float a1 = 4.728797E+02f;
	static const float a2 = -1.988841E+02f;
	static const float a3 = 4.869521E+01f;
	static const float a4 = -1.158754E+00f;

	float vtemp = (THERM_VBIAS / THERM_ADC_BITS) * (float)raw_adc;
	float temp_c = (a4 * powf(vtemp, 4)) + (a3 * powf(vtemp, 3)) + (a2 * powf(vtemp, 2)) +
		       (a1 * vtemp) + a0;

	return temp_c*32;
}

static void set_rgb(bool red, bool green, bool blue)
{
	gpio_pin_set_dt(&led_red, red);
	gpio_pin_set_dt(&led_green, green);
	gpio_pin_set_dt(&led_blue, blue);
}

void thermistor_run(void)
{
	int32_t initial_reading = 0;
	bool first_reading = true;

	gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE);

	int ret = adc_channel_setup_dt(&adc_therm);

	if (ret != 0) {
		printf("adc_channel_setup_dt failed: %d\n", ret);
	}

	(void)adc_sample_dt(&adc_therm, &sequence);

	guicomm_send_bool("thermEnable", 11, true);

	while (current_mode == THERMISTOR_MODE) {
		ret = adc_sample_dt(&adc_therm, &sequence);
		if (ret != 0) {
			printf("adc read failed: %d\n", ret);
			k_sleep(K_MSEC(THERM_SAMPLE_PERIOD_MS));
			continue;
		}

		uint16_t celsius_reading = (uint16_t)thermistor_calc_temperature(adc_raw);

		if (first_reading) {
			initial_reading = celsius_reading;
			first_reading = false;
		}

		guicomm_send_uint32("thADC", 5, celsius_reading);

		if (celsius_reading - THERM_DELTA_THRESHOLD*32 > initial_reading) {
			set_rgb(true, false, false);
		} else if (celsius_reading < initial_reading - THERM_DELTA_THRESHOLD) {
			set_rgb(false, false, true);
		} else {
			set_rgb(false, true, false);
		}

		k_sleep(K_MSEC(THERM_SAMPLE_PERIOD_MS));
	}

	set_rgb(false, false, false);
	guicomm_send_bool("thermEnable", 11, false);
}
