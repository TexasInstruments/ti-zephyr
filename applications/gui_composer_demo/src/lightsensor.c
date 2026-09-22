/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lightsensor.h"
#include "adc_sample.h"
#include "demo_mode.h"
#include <gui_composer/gui_composer.h>

#include <stdio.h>

#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

PINCTRL_DT_DEFINE(DT_NODELABEL(pwma1));

#define LIGHTSENSOR_SAMPLE_PERIOD_MS CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR_SAMPLE_PERIOD_MS
#define LIGHTSENSOR_PWM_PERIOD 512
#define LIGHTSENSOR_ADC_TO_DUTY_SHIFT 3

static const struct adc_dt_spec adc_light = ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), lightsensor);
static const struct device *pwm_rg = DEVICE_DT_GET(DT_NODELABEL(pwma1));
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

static int16_t adc_raw;
static struct adc_sequence sequence = {
	.buffer = &adc_raw,
	.buffer_size = sizeof(adc_raw),
};

void lightsensor_run(void)
{
	int ret = pinctrl_apply_state(PINCTRL_DT_DEV_CONFIG_GET(DT_NODELABEL(pwma1)),
								  PINCTRL_STATE_DEFAULT);

	if (ret != 0) {
		printf("pinctrl_apply_state failed: %d\n", ret);
	}

	ret = adc_channel_setup_dt(&adc_light);
	if (ret != 0) {
		printf("adc_channel_setup_dt failed: %d\n", ret);
	}

	guicomm_send_bool("lightEnable", 11, true);

	while (current_mode == LIGHTSENSOR_MODE) {
		ret = adc_sample_dt(&adc_light, &sequence);
		if (ret != 0) {
			printf("adc read failed: %d\n", ret);
			k_sleep(K_MSEC(LIGHTSENSOR_SAMPLE_PERIOD_MS));
			continue;
		}

		uint16_t duty_cycle = (uint16_t)adc_raw >> LIGHTSENSOR_ADC_TO_DUTY_SHIFT;

		duty_cycle = duty_cycle*duty_cycle;

		if (duty_cycle >= LIGHTSENSOR_PWM_PERIOD) {
			duty_cycle = LIGHTSENSOR_PWM_PERIOD - 1;
		}

		ret = pwm_set_cycles(pwm_rg, 0, LIGHTSENSOR_PWM_PERIOD, duty_cycle, 0);
		if (ret != 0) {
			printf("pwm_set_cycles (channel 0) failed: %d\n", ret);
		}
		ret = pwm_set_cycles(pwm_rg, 1, LIGHTSENSOR_PWM_PERIOD, duty_cycle, 0);
		if (ret != 0) {
			printf("pwm_set_cycles (channel 1) failed: %d\n", ret);
		}
		guicomm_send_uint16("lsADC", 5, 4095 - (uint16_t)adc_raw);
	}

	ret = pwm_set_cycles(pwm_rg, 0, LIGHTSENSOR_PWM_PERIOD, 0, 0);
	if (ret != 0) {
		printf("pwm_set_cycles (channel 0) teardown failed: %d\n", ret);
	}
	ret = pwm_set_cycles(pwm_rg, 1, LIGHTSENSOR_PWM_PERIOD, 0, 0);
	if (ret != 0) {
		printf("pwm_set_cycles (channel 1) teardown failed: %d\n", ret);
	}

	gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);

	guicomm_send_bool("lightEnable", 11, false);
}
