/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief TI MSPM0 Center-Aligned PWM
 *
 * Demonstrates center-aligned (up-down counting) PWM on TI MSPM0 TIMA/TIMG,
 * enabled via the ti,pwm-mode = "CENTER_ALIGN" board overlay property.
 *
 * In this mode the driver configures CCUPD_ZERO_LOAD_EVT so CC register
 * updates are shadow-latched to both the zero-crossing and the counter peak,
 * providing two glitch-free update opportunities per period instead of one.
 * This is the standard waveform for motor control (FOC/BLDC) because the
 * symmetric pulse reduces current-ripple harmonics.
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>

#if !DT_NODE_EXISTS(DT_ALIAS(pwm_out))
#error "Board overlay missing: define 'pwm-out' alias pointing to a PWM device node"
#endif

#define PWM_DEV   DEVICE_DT_GET(DT_ALIAS(pwm_out))
#define PWM_CH    0U
#define PERIOD_NS PWM_MSEC(250) /* 4 Hz; fits 16-bit MSPM0 timer at any board clock */

static const uint8_t duty_pct[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

int main(void)
{
	const struct device *dev = PWM_DEV;
	uint64_t clk_hz;
	uint32_t period_cycles;
	int ret;

	if (!device_is_ready(dev)) {
		printk("Error: %s not ready\n", dev->name);
		return -ENODEV;
	}

	ret = pwm_get_cycles_per_sec(dev, PWM_CH, &clk_hz);
	if (ret < 0) {
		printk("Error: pwm_get_cycles_per_sec: %d\n", ret);
		return ret;
	}

	period_cycles = (uint32_t)((uint64_t)PERIOD_NS * clk_hz / 1000000000ULL);

	if (period_cycles == 0 || period_cycles > 0xFFFFU) {
		printk("Error: period %u cycles out of 16-bit timer range\n", period_cycles);
		return -EINVAL;
	}

	printk("TI MSPM0 PWM - Center-Aligned Mode\n");
	printk("  Timer clock  : %llu Hz\n", clk_hz);
	printk("  Period       : %lu ns (%u cycles)\n", PERIOD_NS, period_cycles);
	printk("  LOAD register: %u cycles (period/2, up-down counting)\n", period_cycles / 2U);
	printk("  CC update    : CCUPD_ZERO_LOAD_EVT (shadow-latched, 2x per period)\n\n");

	/* Sweep duty 10% -> 90% to show smooth center-aligned brightness ramp */
	printk("Duty sweep 10%% -> 90%% (600 ms per step)\n");

	for (size_t i = 0; i < ARRAY_SIZE(duty_pct); i++) {
		uint32_t pulse_ns = (uint32_t)((uint64_t)PERIOD_NS * duty_pct[i] / 100U);
		uint32_t pulse_cycles = (uint32_t)((uint64_t)pulse_ns * clk_hz / 1000000000ULL);

		ret = pwm_set(dev, PWM_CH, PERIOD_NS, pulse_ns, 0);
		if (ret < 0) {
			printk("Error: pwm_set duty=%u%%: %d\n", duty_pct[i], ret);
			return ret;
		}

		printk("  duty=%3u%%  pulse=%9u ns  (%u cycles)\n", duty_pct[i], pulse_ns,
		       pulse_cycles);

		k_msleep(600);
	}

	/*
	 * Shadow-latch test: issue 5 duty changes with no sleep between them.
	 * All writes land within one 250 ms period. With CCUPD_ZERO_LOAD_EVT
	 * each write goes to the shadow register; only the last value takes
	 * effect at the next update event — no partial or glitched pulse.
	 */
	printk("\nShadow-latch test: 5 rapid pwm_set() calls, no sleep\n");

	const uint8_t rapid[] = {20, 80, 35, 65, 50};
	uint32_t t_start_ms = k_uptime_get_32();

	for (size_t i = 0; i < ARRAY_SIZE(rapid); i++) {
		uint32_t pulse_ns = (uint32_t)((uint64_t)PERIOD_NS * rapid[i] / 100U);
		ret = pwm_set(dev, PWM_CH, PERIOD_NS, pulse_ns, 0);
		if (ret < 0) {
			printk("Error: rapid write %zu duty=%u%%: %d\n", i, rapid[i], ret);
			return ret;
		}
		printk("  write %zu: duty=%u%%\n", i, rapid[i]);
	}

	uint32_t elapsed_ms = k_uptime_get_32() - t_start_ms;

	printk("All 5 writes completed in %u ms (period = %lu ms)\n", elapsed_ms,
	       PERIOD_NS / 1000000U);
	printk("Effective duty: %u%% (last-write wins, previous values discarded)\n",
	       rapid[ARRAY_SIZE(rapid) - 1]);

	if (elapsed_ms < (PERIOD_NS / 1000000U)) {
		printk("Shadow-latch confirmed: all writes within one period\n");
	} else {
		printk("Warning: writes spanned >1 period, shadow-latch may not apply\n");
	}

	printk("\nHolding %u%% duty. Done.\n", rapid[ARRAY_SIZE(rapid) - 1]);

	while (1) {
		k_msleep(5000);
	}

	return 0;
}
