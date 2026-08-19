/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief Temperature-controlled PWM with watchdog safety supervision
 *
 * Demonstrates a complete embedded control loop using five MSPM0 peripherals:
 *
 *   ADC (channel 11 = internal temperature sensor)
 *      -> reads die temperature every second
 *   Counter (set_top_value, 1 s period)
 *      -> triggers each ADC conversion at a precise interval
 *   PWM (TIMA0, center-aligned)
 *      -> LED brightness tracks temperature (warmer = brighter)
 *   WWDT (window watchdog)
 *      -> supervises the control loop; if the loop stalls the device resets
 *   HWINFO
 *      -> on every boot, reports whether the previous reset was a WDT violation
 *
 * This pattern -- sense -> compute -> actuate -> supervise -- is standard in
 * industrial and automotive firmware. No upstream Zephyr sample combines
 * all five peripherals in a single control loop.
 *
 * SDK analogs: adc12_internal_temp_sensor_rts + timx_timer_mode_pwm_edge_sleep
 *              + wwdt_window_mode_periodic_reset
 *
 * Hardware: zero external components -- internal temp sensor, on-board LED.
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/sys/printk.h>

/* Device aliases (defined in board overlay) */
#if !DT_HAS_ALIAS(temp_adc) || !DT_HAS_ALIAS(pwm_led) || !DT_HAS_ALIAS(loop_timer) ||              \
	!DT_HAS_ALIAS(watchdog0)
#error "Board overlay must define temp-adc, pwm-led, loop-timer, watchdog0 aliases."
#endif

/*
 * WDT reset scope: derived from the watchdog DT node's ti,watchdog-reset-action
 * property so the software flag matches what the hardware instance supports.
 * L-series: ti,watchdog-reset-action=1 -> WDT_FLAG_RESET_SOC
 * G-series: ti,watchdog-reset-action=0 -> WDT_FLAG_RESET_CPU_CORE
 */
#define WDT_RESET_FLAG                                                                             \
	(DT_PROP(DT_ALIAS(watchdog0), ti_watchdog_reset_action) ? WDT_FLAG_RESET_SOC               \
								: WDT_FLAG_RESET_CPU_CORE)

#define ADC_DEV DEVICE_DT_GET(DT_ALIAS(temp_adc))
#define PWM_DEV DEVICE_DT_GET(DT_ALIAS(pwm_led))
#define CTR_DEV DEVICE_DT_GET(DT_ALIAS(loop_timer))
#define WDT_DEV DEVICE_DT_GET(DT_ALIAS(watchdog0))

#define PWM_CH 0U
/* Channel 11 = MSPM0 internal temperature sensor (all variants) */
#define ADC_CH 11U

/* PWM period: 1 kHz -- fast enough for smooth LED dimming */
#define PWM_PERIOD_NS PWM_USEC(1000)

/*
 * WWDT window: must feed after 500 ms but before 2000 ms.
 * Control loop fires every 1 s -- safely within the open window.
 */
#define WDT_WIN_MIN_MS 500U
#define WDT_WIN_MAX_MS 2000U

/*
 * Temperature -> duty mapping:
 *   Below TEMP_LOW_C  -> MIN_DUTY_PCT  (dim)
 *   Above TEMP_HIGH_C -> MAX_DUTY_PCT  (bright)
 *   Linear between
 *
 * Thresholds are tuned for typical MSPM0 die temperature range.
 * Adjust for your thermal environment.
 */
#define TEMP_LOW_C   15
#define TEMP_HIGH_C  60
#define MIN_DUTY_PCT 5U
#define MAX_DUTY_PCT 95U

/* ADC: 12-bit, internal 1.4 V VREF (MSPM0 bandgap -- accurate for temp sensor) */
#define ADC_VREF_MV    1400U
#define ADC_RESOLUTION 12U

/*
 * MSPM0 internal temperature sensor calibration constants (TRM §16).
 * These are typical values; for best accuracy read factory trim from
 * SYSCTL_TEMP_SENSE_CAL at 0x41C40170 (G-series) / 0x41C00170 (L-series).
 */
#define TEMP_SENS_CAL_MV_PER_C 27  /* ~2.7 mV / deg C  */
#define TEMP_SENS_OFFSET_MV    750 /* 750 mV at 25 deg C */
#define TEMP_SENS_REF_C        25

/* Temperature conversion */
static int32_t adc_raw_to_celsius(int32_t raw)
{
	/* Convert raw -> millivolts */
	int32_t mv = (raw * ADC_VREF_MV) >> ADC_RESOLUTION;

	/* Apply sensor equation: T = (mv - offset) / sensitivity + ref */
	return ((mv - TEMP_SENS_OFFSET_MV) * 10 / TEMP_SENS_CAL_MV_PER_C) +
	       TEMP_SENS_REF_C * 10; /* result in 0.1 deg C units */
}

/* Duty cycle mapping */
static uint32_t temp_to_duty_pct(int32_t temp_01c)
{
	int32_t low = TEMP_LOW_C * 10;
	int32_t high = TEMP_HIGH_C * 10;

	if (temp_01c <= low) {
		return MIN_DUTY_PCT;
	}
	if (temp_01c >= high) {
		return MAX_DUTY_PCT;
	}

	/* Linear interpolation */
	return MIN_DUTY_PCT +
	       (uint32_t)((temp_01c - low) * (MAX_DUTY_PCT - MIN_DUTY_PCT) / (high - low));
}

/* Counter callback: fires every 1 s */
static void loop_timer_cb(const struct device *dev, void *user_data)
{
	ARG_UNUSED(dev);
	k_sem_give((struct k_sem *)user_data);
}

int main(void)
{
	int ret;
	int wdt_ch = -1;
	uint32_t cause = 0;
	uint32_t feed_count = 0;

	/* 1. Check reset cause */
	hwinfo_get_reset_cause(&cause);
	hwinfo_clear_reset_cause();

	printk("TI MSPM0 Temperature-Controlled PWM with WDT Supervision\n");
	printk("  ADC ch%u (internal temp) -> PWM LED brightness\n", ADC_CH);
	printk("  WWDT window [%u..%u ms] supervises control loop\n", WDT_WIN_MIN_MS,
	       WDT_WIN_MAX_MS);

	if (cause & RESET_WATCHDOG) {
		printk("\n*** Safety reset: WWDT detected stalled control loop ***\n\n");
	} else {
		printk("  Clean boot.\n\n");
	}

	/* 2. Validate devices */
	if (!device_is_ready(ADC_DEV) || !device_is_ready(PWM_DEV) || !device_is_ready(CTR_DEV) ||
	    !device_is_ready(WDT_DEV)) {
		printk("Error: one or more devices not ready\n");
		return -ENODEV;
	}

	/* 3. Configure ADC */
	struct adc_channel_cfg adc_cfg = {
		.gain = ADC_GAIN_1,
		.reference = ADC_REF_INTERNAL,
		.acquisition_time = ADC_ACQ_TIME_DEFAULT,
		.channel_id = ADC_CH,
	};
	ret = adc_channel_setup(ADC_DEV, &adc_cfg);
	if (ret < 0) {
		printk("Error: adc_channel_setup: %d\n", ret);
		return ret;
	}

	/* 4. Configure PWM */
	ret = pwm_set(PWM_DEV, PWM_CH, PWM_PERIOD_NS,
		      (uint32_t)(PWM_PERIOD_NS * MIN_DUTY_PCT / 100U), 0);
	if (ret < 0) {
		printk("Error: pwm_set: %d\n", ret);
		return ret;
	}

	/* 5. Configure counter: 1 s period */
	struct k_sem loop_sem;

	k_sem_init(&loop_sem, 0, 1);

	uint32_t ctr_freq = counter_get_frequency(CTR_DEV);
	uint32_t ctr_period = ctr_freq; /* 1 s worth of ticks */

	counter_start(CTR_DEV);

	struct counter_top_cfg top_cfg = {
		.callback = loop_timer_cb,
		.user_data = &loop_sem,
		.ticks = ctr_period,
		.flags = 0,
	};
	ret = counter_set_top_value(CTR_DEV, &top_cfg);
	if (ret < 0) {
		printk("Error: counter_set_top_value: %d\n", ret);
		return ret;
	}

	printk("Counter: %u Hz -> period = %u ticks (1 s)\n", ctr_freq, ctr_period);

	/* 6. Configure WWDT */
	struct wdt_timeout_cfg wdt_cfg = {
		.flags = WDT_RESET_FLAG,
		.window = {.min = WDT_WIN_MIN_MS, .max = WDT_WIN_MAX_MS},
		.callback = NULL,
	};
	wdt_ch = wdt_install_timeout(WDT_DEV, &wdt_cfg);
	if (wdt_ch < 0) {
		printk("Error: wdt_install_timeout: %d\n", wdt_ch);
		return wdt_ch;
	}
	ret = wdt_setup(WDT_DEV, WDT_OPT_PAUSE_HALTED_BY_DBG);
	if (ret < 0) {
		printk("Error: wdt_setup: %d\n", ret);
		return ret;
	}

	printk("WWDT armed: window [%u..%u ms]\n\n", WDT_WIN_MIN_MS, WDT_WIN_MAX_MS);
	printk("%-5s  %-8s  %-6s  %-8s\n", "Feed", "Temp(C)", "Duty%", "PWM ns");
	printk("------------------------------------------\n");

	/* 7. Main control loop */
	int16_t raw_buf;
	struct adc_sequence seq = {
		.channels = BIT(ADC_CH),
		.buffer = &raw_buf,
		.buffer_size = sizeof(raw_buf),
		.resolution = ADC_RESOLUTION,
	};

	/*
	 * Discard the first conversion -- VREF needs one cycle to settle after
	 * regulator_enable(); the initial reading is otherwise unreliable.
	 */
	adc_read(ADC_DEV, &seq);

	while (1) {
		/* Wait for counter top callback (1 s nominal, 1.5 s timeout) */
		ret = k_sem_take(&loop_sem, K_MSEC(1500));
		if (ret < 0) {
			/* Semaphore timeout -- counter stalled, don't feed WDT */
			printk("Counter stall detected -- not feeding WDT\n");
			continue;
		}

		/* Read ADC */
		ret = adc_read(ADC_DEV, &seq);
		if (ret < 0) {
			printk("ADC read error: %d\n", ret);
			continue;
		}

		/* Convert to temperature */
		int32_t temp_01c = adc_raw_to_celsius((int32_t)raw_buf);
		uint32_t duty_pct = temp_to_duty_pct(temp_01c);
		uint32_t pulse_ns = (uint32_t)(PWM_PERIOD_NS * duty_pct / 100U);

		/* Update PWM */
		ret = pwm_set(PWM_DEV, PWM_CH, PWM_PERIOD_NS, pulse_ns, 0);
		if (ret < 0) {
			printk("PWM set error: %d\n", ret);
		}

		/* Feed WWDT -- proof the loop is alive */
		wdt_feed(WDT_DEV, wdt_ch);
		feed_count++;

		printk("%-5u  %3d.%01d C    %3u%%   %7u\n", feed_count, temp_01c / 10,
		       (uint32_t)(temp_01c < 0 ? -temp_01c : temp_01c) % 10, duty_pct, pulse_ns);
	}

	return 0;
}
