/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SAMPLE_GUI_COMPOSER_DEMO_ADC_SAMPLE_H_
#define SAMPLE_GUI_COMPOSER_DEMO_ADC_SAMPLE_H_

#include <zephyr/drivers/adc.h>

static inline int adc_sample_dt(const struct adc_dt_spec *spec, struct adc_sequence *seq)
{
	int ret = adc_sequence_init_dt(spec, seq);

	if (ret == 0) {
		ret = adc_read_dt(spec, seq);
	}

	return ret;
}

#endif /* SAMPLE_GUI_COMPOSER_DEMO_ADC_SAMPLE_H_ */
