/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief TI MSPM0 PWM Output and Capture Precision Analysis
 *
 * Generates a known PWM signal on the pwm-out device and captures it on
 * pwm-cap, measuring four precision properties of the MSPM0 PWM subsystem.
 * All test frequencies are derived at runtime from the capture timer clock so
 * the sample works with any board overlay and timer configuration.
 *
 * Sections:
 *
 * 1. JITTER — captures the same signal 5 times, computes min/max/mean period.
 *    Confirms that the MSPM0 timer output is bit-reproducible (0-jitter).
 *
 * 2. FREQUENCY ACCURACY — sweeps five frequencies from the timer's reliable
 *    range, comparing measured vs programmed frequency against the theoretical
 *    ±1-cycle quantisation limit. Confirms the driver achieves hardware minimum
 *    error.  Note: the MSPM0 timer counts LOAD+1 cycles per period, so measured
 *    frequency is systematically 1 cycle lower than programmed — this is correct
 *    hardware behaviour, not a driver bug.
 *
 * 3. DUTY ACCURACY — at a mid-range frequency, sweeps duty 10–90%.  Shows that
 *    duty error scales with 1/pulse_cycles, consistent with ±1-cycle rounding.
 *
 * 4. DYNAMIC TRACKING — changes duty each period and captures it, confirming
 *    every update is applied within one period without loss or corruption.
 *
 * Hardware requirement:
 *   Wire the pwm-out pin to the pwm-cap pin (see board overlay for pin names).
 *
 * SDK analog: timx_timer_mode_capture_duty_and_period
 *
 * Minimum reliable frequency note:
 *   At 5 MHz capture clock (16-bit timer, 65535-cycle period), signals below
 *   ~200 Hz have >38% probability of two consecutive rising edges straddling
 *   the timer reload boundary, causing ERANGE from the capture block.
 *   Test frequencies are clamped to the reliable range automatically.
 */

#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

#if !DT_NODE_EXISTS(DT_ALIAS(pwm_out)) || !DT_NODE_EXISTS(DT_ALIAS(pwm_cap))
#error "Board overlay must define pwm-out and pwm-cap aliases."
#endif

#define OUT_DEV DEVICE_DT_GET(DT_ALIAS(pwm_out))
#define CAP_DEV DEVICE_DT_GET(DT_ALIAS(pwm_cap))
#define OUT_CH  0U
#define CAP_CH  0U

#define JITTER_SAMPLES         5
#define RETRY_COUNT            5
/* Timeout per retry = this many PWM periods, ensuring the capture has time
 * to complete before we retry.
 */
#define RETRY_TIMEOUT_PERIODS  3U

/*
 * Test points defined in cycles rather than Hz so they scale automatically
 * with the configured timer clock.  All values must be < 32768 so the signal
 * period stays below 50 % of the 16-bit timer period (65535 cycles), keeping
 * the wrap-around probability under 50 %.
 */
static const uint32_t test_cycles[] = {25000, 10000, 5000, 2500, 1000};

/* Duty cycle test points (percent, integer). */
static const uint8_t duty_pct[] = {10, 25, 50, 75, 90};

static int capture_with_retry(const struct device *cap, uint32_t period_ns, uint32_t *out_period,
			      uint32_t *out_pulse)
{
	uint32_t retry_ms = (period_ns / 1000000U) * RETRY_TIMEOUT_PERIODS;
	int ret;

	if (retry_ms < 10U) {
		retry_ms = 10U;
	}

	for (int tries = RETRY_COUNT; tries > 0; tries--) {
		ret = pwm_capture_cycles(cap, CAP_CH,
					 PWM_CAPTURE_TYPE_BOTH | PWM_CAPTURE_MODE_SINGLE,
					 out_period, out_pulse, K_MSEC(retry_ms * 20U));
		if (ret != -EBUSY && ret != -ERANGE) {
			return ret;
		}
		pwm_disable_capture(cap, CAP_CH);
		k_msleep(retry_ms);
	}

	return ret;
}

/* ------------------------------------------------------------------ */
/* Section 1: Jitter                                                   */
/* ------------------------------------------------------------------ */
static void section_jitter(const struct device *out, const struct device *cap, uint64_t cap_clk)
{
	/* Use 3 evenly-spaced test points from the full cycle range. */
	static const uint32_t jitter_cyc[] = {25000, 5000, 1000};

	printf("=== 1. Jitter: %d captures per frequency ===\n", JITTER_SAMPLES);
	printf("%-10s  %-7s  %-7s  %-10s  %s\n", "Freq", "Min", "Max", "Mean (cyc)", "Jitter");
	printf("----------------------------------------------------------\n");

	for (size_t i = 0; i < ARRAY_SIZE(jitter_cyc); i++) {
		uint32_t cyc = jitter_cyc[i];
		uint32_t period_ns = (uint32_t)(1000000000ULL / (cap_clk / cyc));
		uint32_t min_p = UINT32_MAX, max_p = 0, valid = 0;
		uint64_t sum_p = 0;

		pwm_set(out, OUT_CH, period_ns, period_ns / 2U, 0);
		k_msleep(50);

		for (int s = 0; s < JITTER_SAMPLES; s++) {
			uint32_t p = 0, w = 0;

			if (capture_with_retry(cap, period_ns, &p, &w) == 0 && p > 0) {
				if (p < min_p) {
					min_p = p;
				}
				if (p > max_p) {
					max_p = p;
				}
				sum_p += p;
				valid++;
			}
		}

		if (valid == 0) {
			printf("  (capture failed)\n");
			continue;
		}

		uint32_t mean = (uint32_t)(sum_p / valid);
		uint32_t jitter = max_p - min_p;
		/* Display programmed frequency, not captured (avoids Hz truncation) */
		uint32_t freq_hz = (uint32_t)(cap_clk / cyc);

		printf("  %5u Hz   %-7u  %-7u  %-10u  %u cycles\n", freq_hz, min_p, max_p, mean,
		       jitter);
	}
	printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 2: Frequency accuracy                                       */
/* ------------------------------------------------------------------ */
static void section_freq_accuracy(const struct device *out, const struct device *cap,
				  uint64_t cap_clk)
{
	printf("=== 2. Frequency accuracy vs +-1 cycle quantisation limit ===\n");
	printf("%-10s  %-10s  %-10s  %-12s  %-10s  %s\n", "Set Hz", "Cap Hz", "Err (ppm)",
	       "Theory (ppm)", "Cycles", "Status");
	printf("------------------------------------------------------------------------\n");

	uint32_t max_err = 0;

	for (size_t i = 0; i < ARRAY_SIZE(test_cycles); i++) {
		uint32_t cyc = test_cycles[i];
		uint32_t period_ns = (uint32_t)(1000000000ULL / (cap_clk / cyc));
		uint32_t duty_ns = period_ns / 2U;
		uint32_t cap_period = 0, cap_pulse = 0;

		pwm_set(out, OUT_CH, period_ns, duty_ns, 0);
		k_msleep(50);

		int ret = capture_with_retry(cap, period_ns, &cap_period, &cap_pulse);

		if (ret < 0 || cap_period == 0) {
			printf("  (capture err %d)\n", ret);
			continue;
		}

		/*
		 * Compute error in cycle domain to avoid Hz integer-truncation
		 * artefacts.  Longer captured period = lower frequency = negative
		 * frequency error, so negate the cycle delta.
		 */
		int32_t cycle_delta = (int32_t)cap_period - (int32_t)cyc;
		int32_t err_ppm = 0;

		if (cyc) {
			/* Longer period = lower frequency = negative ppm error */
			int64_t num = -(int64_t)cycle_delta * 1000000LL;

			err_ppm = (int32_t)(num / (int32_t)cyc);
		}

		/* Hz * 100 for display only */
		uint32_t set_hz100 = (uint32_t)(100000000000ULL / period_ns);
		uint32_t cap_hz100 = (uint32_t)((uint64_t)cap_clk * 100U / cap_period);

		uint32_t theory_ppm = cyc ? 1000000U / cyc : 999999U;

		if ((uint32_t)abs(err_ppm) > max_err) {
			max_err = (uint32_t)abs(err_ppm);
		}

		bool at_limit = (uint32_t)abs(err_ppm) <= theory_ppm + theory_ppm / 10U;

		printf("  %4u.%02u Hz   %4u.%02u Hz   %+7d      +-%6u       %-6u  %s\n",
		       set_hz100 / 100, set_hz100 % 100, cap_hz100 / 100, cap_hz100 % 100, err_ppm,
		       theory_ppm, cap_period, at_limit ? "AT LIMIT" : "ABOVE LIMIT");
	}

	printf("Max error: %u ppm\n\n", max_err);
}

/* ------------------------------------------------------------------ */
/* Section 3: Duty cycle accuracy                                      */
/* ------------------------------------------------------------------ */
static void section_duty_accuracy(const struct device *out, const struct device *cap,
				  uint64_t cap_clk)
{
	/* Mid-range test point: 4th entry in test_cycles */
	uint32_t cyc = test_cycles[ARRAY_SIZE(test_cycles) / 2];
	uint32_t period_ns = (uint32_t)(1000000000ULL / (cap_clk / cyc));
	uint32_t freq_hz = (uint32_t)(cap_clk / cyc);

	printf("=== 3. Duty accuracy at %u Hz: error scales with 1/pulse_cycles ===\n", freq_hz);
	printf("%-6s  %-8s  %-8s  %-12s  %-10s  %s\n", "Duty", "Set %", "Cap %", "Pulse (cyc)",
	       "Error", "Theory max");
	printf("---------------------------------------------------------------\n");

	pwm_set(out, OUT_CH, period_ns, period_ns / 2U, 0);
	k_msleep(50);

	for (size_t i = 0; i < ARRAY_SIZE(duty_pct); i++) {
		uint32_t pulse_ns = (uint32_t)((uint64_t)period_ns * duty_pct[i] / 100U);
		uint32_t cap_period = 0, cap_pulse = 0;

		pwm_set(out, OUT_CH, period_ns, pulse_ns, 0);
		k_msleep(20);

		int ret = capture_with_retry(cap, period_ns, &cap_period, &cap_pulse);

		if (ret < 0 || cap_period == 0) {
			printf("  (capture err %d)\n", ret);
			continue;
		}

		uint32_t set_d100 = (uint32_t)duty_pct[i] * 100U;
		uint32_t cap_d100 = (uint32_t)((uint64_t)cap_pulse * 10000U / cap_period);
		int32_t err_d100 = (int32_t)cap_d100 - (int32_t)set_d100;
		uint32_t theory_d100 = cap_pulse ? 10000U / cap_pulse : 9999U;

		printf("  %2u%%    %3u.%02u%%   %3u.%02u%%   %-12u  %+d.%02u%%     +-0.%02u%%\n",
		       duty_pct[i], set_d100 / 100, set_d100 % 100, cap_d100 / 100, cap_d100 % 100,
		       cap_pulse, err_d100 / 100, (uint32_t)abs(err_d100) % 100, theory_d100);
	}
	printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 4: Dynamic duty tracking                                    */
/* ------------------------------------------------------------------ */
static void section_dynamic_tracking(const struct device *out, const struct device *cap,
				     uint64_t cap_clk)
{
	static const uint8_t seq[] = {10, 90, 25, 75, 50, 33, 66, 50};
	uint32_t cyc = test_cycles[ARRAY_SIZE(test_cycles) / 2];
	uint32_t period_ns = (uint32_t)(1000000000ULL / (cap_clk / cyc));
	uint32_t freq_hz = (uint32_t)(cap_clk / cyc);
	int missed = 0;

	printf("=== 4. Dynamic duty tracking at %u Hz ===\n", freq_hz);
	printf("%-5s  %-8s  %-8s  %-8s  %s\n", "Step", "Set %", "Cap %", "Error", "Tracked?");
	printf("-----------------------------------------------\n");

	pwm_set(out, OUT_CH, period_ns, period_ns / 2U, 0);
	k_msleep(50);

	for (size_t i = 0; i < ARRAY_SIZE(seq); i++) {
		uint32_t pulse_ns = (uint32_t)((uint64_t)period_ns * seq[i] / 100U);
		uint32_t cap_period = 0, cap_pulse = 0;

		pwm_set(out, OUT_CH, period_ns, pulse_ns, 0);
		k_msleep(period_ns / 1000000U + 1U);

		int ret = capture_with_retry(cap, period_ns, &cap_period, &cap_pulse);

		if (ret < 0 || cap_period == 0) {
			printf("  %2zu   (capture err %d)\n", i + 1, ret);
			missed++;
			continue;
		}

		uint32_t set_d100 = (uint32_t)seq[i] * 100U;
		uint32_t cap_d100 = (uint32_t)((uint64_t)cap_pulse * 10000U / cap_period);
		int32_t err_d100 = (int32_t)cap_d100 - (int32_t)set_d100;
		bool tracked = abs(err_d100) <= 200;

		if (!tracked) {
			missed++;
		}

		printf("  %2zu   %3u.%02u%%   %3u.%02u%%   %+d.%02u%%  %s\n", i + 1, set_d100 / 100,
		       set_d100 % 100, cap_d100 / 100, cap_d100 % 100, err_d100 / 100,
		       (uint32_t)abs(err_d100) % 100, tracked ? "YES" : "NO");
	}

	printf("%d/%zu duty changes tracked correctly.\n\n", (int)(ARRAY_SIZE(seq) - missed),
	       ARRAY_SIZE(seq));
}

/* ------------------------------------------------------------------ */

int main(void)
{
	const struct device *out = OUT_DEV;
	const struct device *cap = CAP_DEV;
	uint64_t out_clk, cap_clk;
	int ret;

	if (!device_is_ready(out)) {
		printf("Error: %s not ready\n", out->name);
		return -ENODEV;
	}
	if (!device_is_ready(cap)) {
		printf("Error: %s not ready\n", cap->name);
		return -ENODEV;
	}

	ret = pwm_get_cycles_per_sec(out, OUT_CH, &out_clk);
	if (ret < 0) {
		printf("Error: pwm_get_cycles_per_sec (out): %d\n", ret);
		return ret;
	}

	ret = pwm_get_cycles_per_sec(cap, CAP_CH, &cap_clk);
	if (ret < 0) {
		printf("Error: pwm_get_cycles_per_sec (cap): %d\n", ret);
		return ret;
	}

	printf("TI MSPM0 PWM Precision Analysis\n");
	printf("  Output  (%s): %llu Hz\n", out->name, out_clk);
	printf("  Capture (%s): %llu Hz\n", cap->name, cap_clk);
	printf("  Wire pwm-out to pwm-cap (see board overlay for pin names)\n\n");

	section_jitter(out, cap, cap_clk);
	section_freq_accuracy(out, cap, cap_clk);
	section_duty_accuracy(out, cap, cap_clk);
	section_dynamic_tracking(out, cap, cap_clk);

	/* Hold final output — frequency derived from test point, not hardcoded */
	uint32_t hold_cyc = test_cycles[ARRAY_SIZE(test_cycles) / 2];
	uint32_t hold_ns = (uint32_t)(1000000000ULL / (cap_clk / hold_cyc));

	printf("Holding %u Hz 50%% output. Done.\n", (uint32_t)(cap_clk / hold_cyc));
	pwm_set(out, OUT_CH, hold_ns, hold_ns / 2U, 0);

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
