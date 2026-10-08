/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#ifndef SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_
#define SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_

enum demo_mode {
	IDLE = 0,
	BLINK_LED_MODE,
	LIGHTSENSOR_MODE,
	THERMISTOR_MODE,
	FNGEN_MODE,
	TEMPERATURE_MODE,
};

extern volatile enum demo_mode current_mode;

#endif /* SAMPLE_GUI_COMPOSER_DEMO_DEMO_MODE_H_ */
