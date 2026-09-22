/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_
#define SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_

enum demo_mode {
	IDLE = 0,
	BLINK_LED_MODE,
	LIGHTSENSOR_MODE,
	THERMISTOR_MODE,
	DAC_MODE,
};

extern volatile enum demo_mode current_mode;

#endif /* SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_ */
