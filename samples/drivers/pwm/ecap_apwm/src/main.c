/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief eCAP APWM mode sample.
 *
 * Drives the TI eCAP peripheral's Auxiliary PWM (APWM) mode to generate a
 * PWM waveform of a given frequency and duty cycle for a fixed run time,
 * then stops.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

/* Output frequency of the generated APWM wave, in Hz */
#define APWM_OUT_FREQ_HZ     1000U
/* Duty cycle of the generated APWM wave, in % */
#define APWM_OUT_DUTY_CYCLE  50U
/* How long to drive the APWM output before stopping, in seconds */
#define APWM_RUN_TIME_SEC    10U

static const struct pwm_dt_spec ecap0 = PWM_DT_SPEC_GET(DT_PATH(zephyr_user));

int main(void)
{
	uint32_t period_ns;
	uint32_t pulse_ns;
	int ret;

	printf("ECAP APWM mode sample started\n");

	if (!pwm_is_ready_dt(&ecap0)) {
		printf("Error: PWM device %s is not ready\n", ecap0.dev->name);
		return 0;
	}

	if ((APWM_OUT_FREQ_HZ == 0U) || (APWM_OUT_DUTY_CYCLE > 100U)) {
		printf("Invalid frequency/duty cycle configuration\n");
		return 0;
	}

	period_ns = PWM_HZ(APWM_OUT_FREQ_HZ);
	pulse_ns = (period_ns / 100U) * APWM_OUT_DUTY_CYCLE;

	printf("Generating %u Hz, %u%% duty cycle on %s channel %u for %u s\n",
	       APWM_OUT_FREQ_HZ, APWM_OUT_DUTY_CYCLE, ecap0.dev->name, ecap0.channel,
	       APWM_RUN_TIME_SEC);

	ret = pwm_set_dt(&ecap0, period_ns, pulse_ns);
	if (ret < 0) {
		printf("Error %d: failed to configure APWM output\n", ret);
		return 0;
	}

	k_sleep(K_SECONDS(APWM_RUN_TIME_SEC));

	/*
	 * The eCAP driver has no dedicated "stop counter" call in the PWM
	 * API, so drive the output to a constant idle level by zeroing the
	 * pulse width instead.
	 */
	ret = pwm_set_dt(&ecap0, period_ns, 0);
	if (ret < 0) {
		printf("Error %d: failed to stop APWM output\n", ret);
		return 0;
	}

	printf("ECAP APWM mode sample done\n");

	return 0;
}
