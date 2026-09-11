/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief RTC-timestamped temperature logger with segment LCD output
 *
 * Combines four peripherals into a live temperature monitor:
 *
 *   ADC (board-defined channel, internal VREF)
 *      -> reads on-chip die temperature every second
 *   RTC (always-on VBAT domain on MSPM0)
 *      -> provides elapsed HH:MM:SS timestamp since boot
 *   auxdisplay (board-defined segment LCD)
 *      -> shows temperature live on the display
 *   UART (via printf)
 *      -> prints "[HH:MM:SS] XX.X C" every second
 *
 * Board requirements (defined in board overlay):
 *   - temp-adc alias  -> ADC device
 *   - rtc alias       -> RTC device
 *   - auxdisplay0 alias -> segment LCD device
 *
 * No external components needed.
 *
 * SDK analogs: adc12_internal_temp_sensor_rts + rtc_alarm
 *
 * Notes:
 *   - The RTC is in the always-on VBAT domain and survives SW resets.
 *     rtc_set_time() takes effect at the next LFOSC tick boundary, so
 *     a 50 ms delay is required before read-back.
 *   - ADC readings are 3-sample averaged per iteration to reduce noise.
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/auxdisplay.h>
#include <zephyr/drivers/hwinfo.h>

/* Device aliases (defined in board overlay) */
#if !DT_HAS_ALIAS(temp_adc) || !DT_HAS_ALIAS(rtc) || !DT_HAS_ALIAS(auxdisplay0)
#error "Board overlay must define temp-adc, rtc, and auxdisplay0 aliases."
#endif

#define ADC_DEV DEVICE_DT_GET(DT_ALIAS(temp_adc))
#define RTC_DEV DEVICE_DT_GET(DT_ALIAS(rtc))
#define LCD_DEV DEVICE_DT_GET(DT_ALIAS(auxdisplay0))

/*
 * ADC channel for die temperature sensor. Channel 11 is the internal
 * temperature sensor on all MSPM0 variants (TRM sec. 16.1). Override per
 * board by redefining in the board overlay or application Kconfig if a
 * different board uses a different channel.
 */
#ifndef ADC_TEMP_CHANNEL
#define ADC_TEMP_CHANNEL 11U
#endif
#define ADC_CH             ADC_TEMP_CHANNEL
#define ADC_VREF_MV        1400U
#define ADC_RESOLUTION     12U
#define ADC_AVG_SAMPLES    3U /* samples averaged per reading */
#define ADC_WARMUP_SAMPLES 5U /* discarded at startup for VREF settle */

/*
 * MSPM0 internal temperature sensor typical calibration (TRM sec. 16).
 * For best accuracy read factory trim from SYSCTL_TEMP_SENSE_CAL.
 */
#define TEMP_SENS_MV_PER_C  27  /* ~2.7 mV / deg C */
#define TEMP_SENS_OFFSET_MV 750 /* 750 mV at 25 deg C */
#define TEMP_SENS_REF_C     25

/* Minimum temperature change (0.1 deg C units) to log a WARMING/COOLING event */
#define TEMP_EVENT_THRESHOLD 30 /* 3.0 C */

/* Summary interval: print min/max/cur every N iterations */
#define SUMMARY_INTERVAL 10U

/* Duration to show all-segments-ON at boot (display hardware verification) */
#define LCD_STARTUP_SHOW_MS 600U

/* RTC boot time: application counts elapsed time from 00:00:00 */
static const struct rtc_time boot_time = {
	.tm_sec = 0,
	.tm_min = 0,
	.tm_hour = 0,
	.tm_mday = 1,
	.tm_mon = 0,
	.tm_year = 126, /* years since 1900 -> 2026 */
};

/* Temperature conversion */
static int32_t adc_raw_to_celsius(int32_t raw)
{
	int32_t mv = (raw * (int32_t)ADC_VREF_MV) >> ADC_RESOLUTION;

	return ((mv - TEMP_SENS_OFFSET_MV) * 10 / TEMP_SENS_MV_PER_C) +
	       TEMP_SENS_REF_C * 10; /* 0.1 deg C units */
}

/* LCD temperature formatting */
/*
 * Format temperature right-aligned into a digits-wide string.
 * Input: temp_01c in 0.1 deg C units (490 = 49.0 C).
 * buf must be digits+1 bytes.
 */
static void format_lcd_temp(char *buf, int32_t temp_01c, uint16_t digits)
{
	int32_t abs_val = temp_01c < 0 ? -temp_01c : temp_01c;
	char tmp[16];
	int len;

	len = snprintf(tmp, sizeof(tmp), temp_01c < 0 ? "-%d.%d" : "%d.%d", (int)(abs_val / 10),
		       (int)(abs_val % 10));

	memset(buf, ' ', digits);
	buf[digits] = '\0';
	if (len <= digits) {
		memcpy(buf + digits - len, tmp, len);
	} else {
		memcpy(buf, tmp + len - digits, digits);
	}
}

/* ADC averaged read */
static int adc_read_avg(struct adc_sequence *seq, int16_t *buf, uint32_t n, int32_t *out_raw)
{
	int64_t sum = 0;

	for (uint32_t i = 0; i < n; i++) {
		int ret = adc_read(ADC_DEV, seq);

		if (ret < 0) {
			return ret;
		}
		sum += *buf;
	}
	*out_raw = (int32_t)(sum / (int64_t)n);
	return 0;
}

int main(void)
{
	int ret;
	uint32_t cause = 0;

	hwinfo_get_reset_cause(&cause);
	hwinfo_clear_reset_cause();

	printf("TI MSPM0 RTC + Temperature + LCD Display\n");
	printf("  ADC ch%u -> auxdisplay + UART\n", ADC_CH);
	printf("  RTC elapsed time: HH:MM:SS since boot\n");
	if (cause & RESET_WATCHDOG) {
		printf("  *** Reset by watchdog ***\n\n");
	} else {
		printf("  Clean boot.\n\n");
	}

	/* 1. Device readiness */
	if (!device_is_ready(ADC_DEV) || !device_is_ready(RTC_DEV) || !device_is_ready(LCD_DEV)) {
		printf("Error: one or more devices not ready\n");
		return -ENODEV;
	}

	/* 2. Query display geometry at runtime */
	struct auxdisplay_capabilities lcd_caps;

	ret = auxdisplay_capabilities_get(LCD_DEV, &lcd_caps);
	if (ret < 0) {
		printf("Error: auxdisplay_capabilities_get: %d\n", ret);
		return ret;
	}
	uint16_t lcd_digits = lcd_caps.columns * lcd_caps.rows;

	/* 3. LCD startup: all segments on, then clear */
	char *all_seg = k_malloc(lcd_digits * 2 + 1);

	if (!all_seg) {
		printf("Error: out of memory\n");
		return -ENOMEM;
	}
	for (uint16_t i = 0; i < lcd_digits; i++) {
		all_seg[i * 2] = '8';
		all_seg[i * 2 + 1] = '.';
	}
	all_seg[lcd_digits * 2] = '\0';
	ret = auxdisplay_write(LCD_DEV, (const uint8_t *)all_seg, lcd_digits * 2);
	k_free(all_seg);
	if (ret < 0) {
		printf("Error: auxdisplay_write (startup): %d\n", ret);
		return ret;
	}
	k_sleep(K_MSEC(LCD_STARTUP_SHOW_MS));
	auxdisplay_clear(LCD_DEV);

	/* 4. Configure ADC */
	struct adc_channel_cfg adc_cfg = {
		.gain = ADC_GAIN_1,
		.reference = ADC_REF_INTERNAL,
		.acquisition_time = ADC_ACQ_TIME_DEFAULT,
		.channel_id = ADC_CH,
	};
	ret = adc_channel_setup(ADC_DEV, &adc_cfg);
	if (ret < 0) {
		printf("Error: adc_channel_setup: %d\n", ret);
		return ret;
	}

	int16_t raw_buf;
	struct adc_sequence seq = {
		.channels = BIT(ADC_CH),
		.buffer = &raw_buf,
		.buffer_size = sizeof(raw_buf),
		.resolution = ADC_RESOLUTION,
	};

	/*
	 * Discard warmup conversions -- VREF needs several cycles to settle
	 * after regulator_enable(); return values ignored intentionally.
	 */
	for (uint32_t i = 0; i < ADC_WARMUP_SAMPLES; i++) {
		(void)adc_read(ADC_DEV, &seq);
	}

	/* 5. Read boot temperature */
	int32_t boot_raw = 0;

	ret = adc_read_avg(&seq, &raw_buf, ADC_AVG_SAMPLES, &boot_raw);
	if (ret < 0) {
		printf("Error: boot adc_read: %d\n", ret);
		return ret;
	}
	int32_t boot_temp = adc_raw_to_celsius(boot_raw);

	/* 6. Set RTC to 00:00:00 */
	ret = rtc_set_time(RTC_DEV, &boot_time);
	if (ret < 0) {
		printf("Error: rtc_set_time: %d\n", ret);
		return ret;
	}
	/*
	 * Wait one LFOSC tick boundary before relying on the new time.
	 * RTC writes take effect at the next 32768 Hz tick (~31 us);
	 * a small margin ensures reliable read-back.
	 */
	k_sleep(K_MSEC(50));

	printf("Boot temp: %d.%01d C  RTC started at 00:00:00\n\n", boot_temp / 10,
	       (uint32_t)(boot_temp < 0 ? -boot_temp : boot_temp) % 10);

	printf("%-12s  %-8s  %s\n", "Time", "Temp(C)", "Event");
	printf("--------------------------------------------\n");

	/* 7. Main loop: 1 s tick */
	char *lcd_buf = k_malloc(lcd_digits + 1);

	if (!lcd_buf) {
		printf("Error: out of memory\n");
		return -ENOMEM;
	}

	int32_t min_temp = INT32_MAX;
	int32_t max_temp = INT32_MIN;
	int32_t prev_temp = INT32_MIN;
	uint32_t iter = 0;
	int32_t avg_raw = boot_raw;

	while (1) {
		k_sleep(K_SECONDS(1));
		iter++;

		/* Averaged ADC read */
		ret = adc_read_avg(&seq, &raw_buf, ADC_AVG_SAMPLES, &avg_raw);
		if (ret < 0) {
			printf("ADC read error: %d\n", ret);
			continue;
		}

		/* RTC timestamp */
		struct rtc_time now;

		ret = rtc_get_time(RTC_DEV, &now);
		if (ret < 0) {
			printf("RTC read error: %d\n", ret);
			continue;
		}

		int32_t temp_01c = adc_raw_to_celsius(avg_raw);

		/* Update running min/max */
		if (temp_01c < min_temp) {
			min_temp = temp_01c;
		}
		if (temp_01c > max_temp) {
			max_temp = temp_01c;
		}

		/* Detect significant temperature transitions */
		const char *event = "";

		if (prev_temp != INT32_MIN) {
			int32_t delta = temp_01c - prev_temp;

			if (delta >= TEMP_EVENT_THRESHOLD) {
				event = "^ WARMING";
			} else if (delta <= -TEMP_EVENT_THRESHOLD) {
				event = "v COOLING";
			}
		}
		prev_temp = temp_01c;

		/* Update display */
		format_lcd_temp(lcd_buf, temp_01c, lcd_digits);
		ret = auxdisplay_clear(LCD_DEV);
		if (ret == 0) {
			ret = auxdisplay_write(LCD_DEV, (const uint8_t *)lcd_buf, lcd_digits);
		}
		if (ret < 0) {
			printf("LCD error: %d\n", ret);
		}

		/* UART output */
		printf("[%02d:%02d:%02d]  %3d.%01d C    %s\n", now.tm_hour, now.tm_min, now.tm_sec,
		       temp_01c / 10, (uint32_t)(temp_01c < 0 ? -temp_01c : temp_01c) % 10, event);

		/* Periodic summary */
		if (iter % SUMMARY_INTERVAL == 0) {
			printf("  -- %u s  min=%d.%01d C  max=%d.%01d C"
			       "  cur=%d.%01d C --\n",
			       iter, min_temp / 10,
			       (uint32_t)(min_temp < 0 ? -min_temp : min_temp) % 10, max_temp / 10,
			       (uint32_t)(max_temp < 0 ? -max_temp : max_temp) % 10, temp_01c / 10,
			       (uint32_t)(temp_01c < 0 ? -temp_01c : temp_01c) % 10);
		}
	}

	k_free(lcd_buf);
	return 0;
}
