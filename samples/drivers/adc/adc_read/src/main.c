/*
 * Copyright (c) 2024 Texas Instruments Incorporated
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>

#define ADC_NODE    DT_ALIAS(adc0)
#define ADC_CHANNEL 0

static const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);

static const struct adc_channel_cfg ch_cfg = {
	.gain             = ADC_GAIN_1,
	.reference        = ADC_REF_INTERNAL,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id       = ADC_CHANNEL,
};

int main(void)
{
	int16_t buf;
	struct adc_sequence seq = {
		.channels    = BIT(ADC_CHANNEL),
		.buffer      = &buf,
		.buffer_size = sizeof(buf),
		.resolution  = 12,
	};

	if (!device_is_ready(adc_dev)) {
		return 0;
	}
	adc_channel_setup(adc_dev, &ch_cfg);

	while (1) {
		adc_read(adc_dev, &seq);
		printf("ADC ch%d: %d\n", ADC_CHANNEL, buf);
		k_msleep(500);
	}
	return 0;
}
