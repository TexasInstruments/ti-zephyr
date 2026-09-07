/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief TI MSPM0 RTC Calendar Sample
 *
 * The sample exercises the following features:
 *
 * - Set/get calendar date and time.
 * - Alarm — fires a callback at minute :20 (one minute after the initial :19).
 * - Calibration (CONFIG_RTC_CALIBRATION) — compensates for crystal frequency
 *   error by applying a ppb offset. Example: a +10 ppm fast crystal is
 *   corrected with -10000 ppb. Range: ±240 ppm, granularity: 1 ppm (1000 ppb).
 * - Update callback (CONFIG_RTC_UPDATE) — invoked once per second by the
 *   driver; replaces the polling loop when enabled.
 *
 * TI MSPM0-specific features exercised on all supported boards:
 *
 * - Interval timer  — fires a callback once per minute, hour, midnight or noon (RTCTEV).
 * - Prescaler timers — two independent sub-second timers with selectable divisors:
 *     RT0PS (32 kHz input): DIV8=244us, DIV16=488us, DIV32=976us,
 *                           DIV64=1.95ms, DIV128=3.90ms, DIV256=7.81ms
 *     RT1PS (RT0PS/256 input): DIV2=15.6ms, DIV4=31.2ms, DIV8=62.5ms,
 *                              DIV16=125ms, DIV32=250ms, DIV64=500ms,
 *                              DIV128=1s,   DIV256=2s
 *
 * SDK analog: rtc_calendar
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/sys/util.h>

#if defined(CONFIG_RTC_TI_MSPM0_INTERVAL_TIMER) || defined(CONFIG_RTC_TI_MSPM0_PS_TIMER)
#include <zephyr/drivers/rtc/rtc_ti_mspm0.h>
#endif

const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

#if defined(CONFIG_RTC_TI_MSPM0_INTERVAL_TIMER)
static void interval_callback(const struct device *dev, void *user_data)
{
	printf("RTC interval event fired\n");
}
#endif

#if defined(CONFIG_RTC_TI_MSPM0_PS_TIMER)
static void rt1ps_callback(const struct device *dev, void *user_data)
{
	printf("RT1PS prescaler event fired\n");
}
#endif

#if defined(CONFIG_RTC_UPDATE)
static void update_callback(const struct device *dev, void *user_data)
{
	struct rtc_time tm;

	if (rtc_get_time(dev, &tm) == 0) {
		printf("RTC date and time: %04d-%02d-%02d %02d:%02d:%02d\n",
		       tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		       tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
}
#endif

#if defined(CONFIG_RTC_ALARM)
static void alarm_callback(const struct device *dev, uint16_t id, void *user_data)
{
	printf("Alarm %d fired!\n", id);
}
#endif

#if defined(CONFIG_RTC_CALIBRATION)
static int set_calibration(const struct device *rtc)
{
	int ret;
	int32_t cal;
	/* Correct a +10 ppm fast crystal: apply -10 ppm = -10000 ppb */
	const int32_t calibration_ppb = -10000;

	ret = rtc_set_calibration(rtc, calibration_ppb);
	if (ret < 0) {
		printf("Cannot set calibration: %d\n", ret);
		return ret;
	}

	ret = rtc_get_calibration(rtc, &cal);
	if (ret < 0) {
		printf("Cannot get calibration: %d\n", ret);
		return ret;
	}

	printf("RTC calibration set to %d ppb\n", cal);
	return 0;
}
#endif

static int set_date_time(const struct device *rtc)
{
	int ret;
	struct rtc_time tm = {
		.tm_year = 2026 - 1900,
		.tm_mon  = 9 - 1,
		.tm_mday = 7,
		.tm_hour = 4,
		.tm_min  = 19,
		.tm_sec  = 0,
	};

	ret = rtc_set_time(rtc, &tm);
	if (ret < 0) {
		printf("Cannot write date time: %d\n", ret);
	}
	return ret;
}

#if !defined(CONFIG_RTC_UPDATE)
static int get_date_time(const struct device *rtc)
{
	int ret;
	struct rtc_time tm;

	ret = rtc_get_time(rtc, &tm);
	if (ret < 0) {
		printf("Cannot read date time: %d\n", ret);
		return ret;
	}

	printf("RTC date and time: %04d-%02d-%02d %02d:%02d:%02d\n",
	       tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
	       tm.tm_hour, tm.tm_min, tm.tm_sec);

	return ret;
}
#endif

#if defined(CONFIG_RTC_ALARM)
static int set_alarm(const struct device *rtc)
{
	int ret;
	/* Fire at minute :20, one minute after the initial time of :19 */
	struct rtc_time alarm_time = {
		.tm_min = 20,
	};

	ret = rtc_alarm_set_callback(rtc, 0, alarm_callback, NULL);
	if (ret < 0) {
		printf("Cannot set alarm callback: %d\n", ret);
		return ret;
	}

	ret = rtc_alarm_set_time(rtc, 0, RTC_ALARM_TIME_MASK_MINUTE, &alarm_time);
	if (ret < 0) {
		printf("Cannot set alarm time: %d\n", ret);
		return ret;
	}

	printf("Alarm set for minute :20\n");
	return 0;
}
#endif

int main(void)
{
	if (!device_is_ready(rtc)) {
		printf("Device is not ready\n");
		return 0;
	}

#if defined(CONFIG_RTC_CALIBRATION)
	set_calibration(rtc);
#endif

	set_date_time(rtc);

#if defined(CONFIG_RTC_ALARM)
	set_alarm(rtc);
#endif

#if defined(CONFIG_RTC_TI_MSPM0_INTERVAL_TIMER)
	rtc_mspm0_set_interval_callback(rtc, RTC_MSPM0_INTERVAL_MINUTE,
					interval_callback, NULL);
#endif

#if defined(CONFIG_RTC_TI_MSPM0_PS_TIMER)
	rtc_mspm0_set_ps_callback(rtc, RTC_MSPM0_PS_TIMER_1,
				  RTC_MSPM0_RT1PS_DIV256, rt1ps_callback, NULL);
#endif

#if defined(CONFIG_RTC_UPDATE)
	rtc_update_set_callback(rtc, update_callback, NULL);
#endif

#if !defined(CONFIG_RTC_UPDATE)
	/* Continuously poll the RTC when the update callback is not enabled */
	while (get_date_time(rtc) == 0) {
		k_sleep(K_MSEC(1000));
	}
#else
	while (1) {
		k_sleep(K_FOREVER);
	}
#endif
	return 0;
}
