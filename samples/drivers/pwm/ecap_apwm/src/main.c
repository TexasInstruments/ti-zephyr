/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

/**
 * Drives the TI eCAP peripheral's Auxiliary PWM (APWM) mode to generate a
 * PWM waveform of a given frequency and duty cycle for a fixed run time,
 * then stops.
 */

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ecap_apwm);

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

	LOG_INF("ECAP APWM mode sample started");

	if (!pwm_is_ready_dt(&ecap0)) {
		LOG_ERR("PWM device %s is not ready", ecap0.dev->name);
		return 0;
	}

	if ((APWM_OUT_FREQ_HZ == 0U) || (APWM_OUT_DUTY_CYCLE > 100U)) {
		LOG_ERR("Invalid frequency/duty cycle configuration");
		return 0;
	}

	period_ns = PWM_HZ(APWM_OUT_FREQ_HZ);
	pulse_ns = (period_ns / 100U) * APWM_OUT_DUTY_CYCLE;

	LOG_INF("Generating %u Hz, %u%% duty cycle on %s channel %u for %u s",
	       APWM_OUT_FREQ_HZ, APWM_OUT_DUTY_CYCLE, ecap0.dev->name, ecap0.channel,
	       APWM_RUN_TIME_SEC);

	ret = pwm_set_dt(&ecap0, period_ns, pulse_ns);
	if (ret < 0) {
		LOG_ERR("Failed to configure APWM output");
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
		LOG_INF("Error %d: failed to stop APWM output", ret);
		return 0;
	}

	LOG_INF("ECAP APWM mode sample done");

	return 0;
}
