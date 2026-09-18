/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_H_
#define SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_H_

#include <stdbool.h>
#include <stdint.h>

void function_generator_run(void);

void function_generator_set_wave_type(uint16_t type);

void function_generator_set_frequency(uint16_t freq_sel);

void function_generator_set_amplitude(uint16_t pct);

void function_generator_set_adc_enable(bool enable);

#if defined(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_OPA_TIER_FULL)
void function_generator_set_gain(uint16_t gain);

void function_generator_set_inversion(uint16_t invert);
#endif

#endif /* SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR_H_ */
