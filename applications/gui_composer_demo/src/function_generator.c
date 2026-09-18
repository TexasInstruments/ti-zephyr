/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "function_generator.h"
#include "adc_sample.h"
#include "demo_mode.h"
#include "waveforms.h"
#include <gui_composer/gui_composer.h>

#include <stdio.h>

#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/dac.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
#include <zephyr/drivers/opamp.h>
#endif

#define FNGEN_LUT_LEN          512
#define FNGEN_DAC_CHANNEL      0
#define FNGEN_DAC_RESOLUTION   12
#define FNGEN_REPORT_PERIOD_MS CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_REPORT_PERIOD_MS
#define FNGEN_DEFAULT_AMPLITUDE_PCT CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_DEFAULT_AMPLITUDE_PCT

static const struct device *dac_dev = DEVICE_DT_GET(DT_NODELABEL(dac0));

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
static const struct adc_dt_spec adc_dac_readback =
	ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), fngen_readback_full);
#else
static const struct adc_dt_spec adc_dac_readback =
	ADC_DT_SPEC_GET_BY_NAME(DT_PATH(zephyr_user), fngen_readback_minimal);
#endif

static const uint16_t *const wave_luts[] = {
	gOutputSignalSine512,
	gOutputSignalSquare512,
	gOutputSignalSawtooth512,
	gOutputSignalTriangle512,
};

static const uint32_t period_us[] = {2000, 1000, 500};

static uint8_t wave_type;
static uint16_t amplitude_pct = FNGEN_DEFAULT_AMPLITUDE_PCT;
static uint16_t freq_sel;
static uint16_t sample_index;
static bool dispADC;

static int16_t adc_raw;
static struct adc_sequence sequence = {
	.buffer = &adc_raw,
	.buffer_size = sizeof(adc_raw),
};

void function_generator_set_wave_type(uint16_t type)
{
	if (type < ARRAY_SIZE(wave_luts)) {
		wave_type = (uint8_t)type;
	}
}

void function_generator_set_frequency(uint16_t freq_sel_in)
{
	freq_sel = (freq_sel_in < ARRAY_SIZE(period_us)) ? freq_sel_in : 0;
}

void function_generator_set_amplitude(uint16_t pct)
{
	amplitude_pct = (pct <= 100) ? pct : 100;
}

void function_generator_set_adc_enable(bool enable)
{
	dispADC = enable;
}

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
static const struct device *opa1_dev = DEVICE_DT_GET(DT_NODELABEL(opa1));

static enum opamp_gain gain_enum_for(uint16_t gain_step)
{
	static const enum opamp_gain gain_table[] = {
		OPAMP_GAIN_1,  OPAMP_GAIN_2,  OPAMP_GAIN_4,
		OPAMP_GAIN_8,  OPAMP_GAIN_16, OPAMP_GAIN_32,
	};

	return gain_table[gain_step < ARRAY_SIZE(gain_table) ? gain_step : 0];
}

void function_generator_set_gain(uint16_t gain)
{
	int ret = opamp_set_gain(opa1_dev, gain_enum_for(gain));

	if (ret != 0) {
		printf("opamp_set_gain failed: %d\n", ret);
	}
}

void function_generator_set_inversion(uint16_t invert)
{
	ARG_UNUSED(invert);
	printf("inversion is fixed at build time (devicetree functional-mode) under the FULL tier -- rebuild with the other overlay variant to change it\n");
}
#endif

void function_generator_run(void)
{
	static const struct dac_channel_cfg dac_cfg = {
		.channel_id = FNGEN_DAC_CHANNEL,
		.resolution = FNGEN_DAC_RESOLUTION,
		.buffered = true,
		.internal = true,
	};

	int ret = dac_channel_setup(dac_dev, &dac_cfg);

	if (ret != 0) {
		printf("dac_channel_setup failed: %d\n", ret);
	}
	ret = adc_channel_setup_dt(&adc_dac_readback);
	if (ret != 0) {
		printf("adc_channel_setup_dt failed: %d\n", ret);
	}

	wave_type = 0;
	amplitude_pct = FNGEN_DEFAULT_AMPLITUDE_PCT;
	freq_sel = 0;
	sample_index = 0;
	dispADC = false;

	guicomm_send_bool("dacEnable", 9, true);
	guicomm_send_uint16("setAmp", 6, amplitude_pct);
	guicomm_send_uint16("setType", 7, wave_type);
	guicomm_send_uint16("setFreq", 7, freq_sel);
#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
	guicomm_send_uint16("setGain", 7, 0);
	guicomm_send_uint16("inversion", 9, 0);
#endif

	int64_t next_report_ms = k_uptime_get() + FNGEN_REPORT_PERIOD_MS;

	while (current_mode == DAC_MODE) {
		uint16_t sample = (wave_luts[wave_type][sample_index] * amplitude_pct) / 100;

		ret = dac_write_value(dac_dev, FNGEN_DAC_CHANNEL, sample);
		if (ret != 0) {
			printf("dac_write_value failed: %d\n", ret);
		}
		sample_index = (sample_index + 1) % FNGEN_LUT_LEN;

		if (dispADC && k_uptime_get() >= next_report_ms) {
			ret = adc_sample_dt(&adc_dac_readback, &sequence);
			if (ret == 0) {
				guicomm_send_uint16("dacADC", 6, (uint16_t)adc_raw);
			} else {
				printf("adc read failed: %d\n", ret);
			}
			next_report_ms += FNGEN_REPORT_PERIOD_MS;
		}

		k_sleep(K_USEC(period_us[freq_sel]));
	}

	dac_write_value(dac_dev, FNGEN_DAC_CHANNEL, 0);
	guicomm_send_bool("dacEnable", 9, false);
}
