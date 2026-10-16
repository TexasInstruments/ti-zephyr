/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

/**
 * @file
 * @brief Multi-state temperature monitor with two-button navigation
 *
 * Three display states cycled via S1 (PA18, backward) and S2 (PB8, forward):
 *
 *   STATE_TEMP  — live die temperature  "  49.0 °C"
 *   STATE_TIME  — elapsed HH:MM:SS      "12:34:56"
 *   STATE_DATE  — boot date DD.MM.YY    "01.01.26"
 *
 * Auto-returns to TEMP after AUTO_RETURN_MS of inactivity.
 *
 * Threads:
 *   adc_thread — samples temperature every 1 s; logs to UART with
 *                WARMING/COOLING events and periodic min/max summary.
 *   main       — display loop at 250 ms; owns the LCD.
 *
 * Board requirements (board overlay):
 *   temp-adc    → ADC0 ch11 (internal temperature sensor, 1.4 V INTREF)
 *   rtc         → RTC (LFSS, always-on VBAT domain)
 *   auxdisplay0 → 6-digit 14-segment LCD
 *   sw0         → S2, PA18 (backward)
 *   sw1         → S3, PB8  (forward)
 *
 * Notes:
 *   - RTC is seeded from the build Unix timestamp (CMake string(TIMESTAMP), UTC) at boot.
 *     Flash immediately after build for best accuracy.
 *   - The '.' in temperature strings is a decorator (no digit slot consumed).
 *     Formatting into lcd_digits chars uses lcd_digits-1 physical slots,
 *     so appending " C" places them in the last two slots (right of ° indicator).
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/auxdisplay.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/atomic.h>

/* ── Device aliases ──────────────────────────────────────────────────────── */

#if !DT_HAS_ALIAS(temp_adc) || !DT_HAS_ALIAS(rtc) || !DT_HAS_ALIAS(auxdisplay0)
#error "Board overlay must define temp-adc, rtc, and auxdisplay0 aliases."
#endif
#if !DT_HAS_ALIAS(sw0) || !DT_HAS_ALIAS(sw1)
#error "Board overlay must define sw0 (S1 backward) and sw1 (S2 forward) aliases."
#endif

#define ADC_DEV DEVICE_DT_GET(DT_ALIAS(temp_adc))
#define RTC_DEV DEVICE_DT_GET(DT_ALIAS(rtc))
#define LCD_DEV DEVICE_DT_GET(DT_ALIAS(auxdisplay0))

/* ── ADC / temperature calibration ──────────────────────────────────────── */

#ifndef ADC_TEMP_CHANNEL
#define ADC_TEMP_CHANNEL 11U
#endif
#define ADC_CH             ADC_TEMP_CHANNEL
#define ADC_VREF_MV        1400U
#define ADC_RESOLUTION     12U
#define ADC_AVG_SAMPLES    3U
#define ADC_WARMUP_SAMPLES 5U

/* MSPM0 internal temperature sensor (TRM sec. 16) */
#define TEMP_SENS_MV_PER_C  27
#define TEMP_SENS_OFFSET_MV 750
#define TEMP_SENS_REF_C     25

/* ── Display state machine ───────────────────────────────────────────────── */

#define STATE_TEMP  0
#define STATE_TIME  1
#define STATE_DATE  2
#define STATE_COUNT 3

static const char *const state_names[STATE_COUNT] = {"TEMP", "TIME", "DATE"};

/* ── Timing ──────────────────────────────────────────────────────────────── */

#define AUTO_RETURN_MS   10000U /* idle → snap back to TEMP */
#define DEBOUNCE_MS         50U
#define DISPLAY_PERIOD_MS  250U
#define ADC_PERIOD_MS     1000U
#define LCD_STARTUP_SHOW_MS 600U

/* ── Logging ─────────────────────────────────────────────────────────────── */

#define TEMP_EVENT_THRESHOLD 30  /* 3.0 °C in 0.1 °C units */
#define SUMMARY_INTERVAL     10U

/*
 * RTC crystal calibration offset in PPB (parts per billion).
 * Positive = speed up, negative = slow down.
 * Measure drift over a known period and adjust to compensate LFOSC error.
 * Example: if RTC gains 1 s/day → set to -(1*1e9/86400) ≈ -11574 PPB.
 */
#define RTC_CAL_PPB 0

/* ── LCD custom indicator ────────────────────────────────────────────────── */

#define LCD_INDICATOR_DEG_C 7U  /* COL4: ° symbol after digit 5 */

/* ── Shared state ────────────────────────────────────────────────────────── */

static atomic_t g_state       = ATOMIC_INIT(STATE_TEMP);
static atomic_t g_last_btn_ms = ATOMIC_INIT(0);   /* k_uptime_get_32() at last press */
static atomic_t g_temp_01c    = ATOMIC_INIT(250); /* 25.0 °C until first ADC read */
static atomic_t g_min_temp    = ATOMIC_INIT(9999);
static atomic_t g_max_temp    = ATOMIC_INIT(-9999);

/* ── ADC sequence (init in main, read in adc_thread) ────────────────────── */

static int16_t             g_raw_buf;
static struct adc_sequence g_adc_seq;

/* ── Buttons ─────────────────────────────────────────────────────────────── */

static const struct gpio_dt_spec btn_s1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn_s2 = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static struct gpio_callback btn_s1_cb_data;
static struct gpio_callback btn_s2_cb_data;

static void btn_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(cb);

	uint32_t now = k_uptime_get_32();

	/* Debounce: atomically claim the slot — if another press just landed, skip */
	atomic_val_t last = atomic_get(&g_last_btn_ms);

	if ((uint32_t)(now - (uint32_t)last) < DEBOUNCE_MS ||
	    !atomic_cas(&g_last_btn_ms, last, (atomic_val_t)now)) {
		return;
	}

	/* S1 = backward (STATE_COUNT-1 ≡ -1 mod STATE_COUNT), S2 = forward (+1) */
	int dir = (dev == btn_s1.port && (pins & BIT(btn_s1.pin))) ? (STATE_COUNT - 1) : 1;

	/* Atomic RMW: loop until CAS succeeds */
	atomic_val_t old, new_val;

	do {
		old = atomic_get(&g_state);
		new_val = ((int)old + dir) % STATE_COUNT;
	} while (!atomic_cas(&g_state, old, new_val));
}

/* ── Temperature helpers ─────────────────────────────────────────────────── */

static int32_t adc_raw_to_celsius(int32_t raw)
{
	int32_t mv = (raw * (int32_t)ADC_VREF_MV) >> ADC_RESOLUTION;

	return ((mv - TEMP_SENS_OFFSET_MV) * 10 / TEMP_SENS_MV_PER_C) +
	       TEMP_SENS_REF_C * 10;
}

static int adc_read_avg(uint32_t n, int32_t *out_raw)
{
	int64_t sum = 0;

	for (uint32_t i = 0; i < n; i++) {
		int ret = adc_read(ADC_DEV, &g_adc_seq);

		if (ret < 0) {
			return ret;
		}
		sum += g_raw_buf;
	}
	*out_raw = (int32_t)(sum / (int64_t)n);
	return 0;
}

/*
 * Format temperature right-aligned into `digits` columns.
 * Input: temp_01c in 0.1 °C units (490 = 49.0 °C).
 * buf must be at least digits+1 bytes.
 */
static void format_lcd_temp(char *buf, int32_t temp_01c, uint16_t digits)
{
	int32_t abs_val = temp_01c < 0 ? -temp_01c : temp_01c;
	char tmp[16];
	int len = snprintf(tmp, sizeof(tmp),
			   temp_01c < 0 ? "-%d.%d" : "%d.%d",
			   (int)(abs_val / 10), (int)(abs_val % 10));

	memset(buf, ' ', digits);
	buf[digits] = '\0';
	if (len <= digits) {
		memcpy(buf + digits - len, tmp, len);
	} else {
		memcpy(buf, tmp + len - digits, digits);
	}
}

/* ── RTC time sync from host ─────────────────────────────────────────────── */

/* Seed RTC from the Unix timestamp (UTC) passed by CMake at configure time. */
static struct rtc_time rtc_from_build_time(void)
{
	time_t t = BUILD_UNIX_TIME;
	struct tm tm;

	gmtime_r(&t, &tm);

	return (struct rtc_time){
		.tm_sec  = tm.tm_sec,
		.tm_min  = tm.tm_min,
		.tm_hour = tm.tm_hour,
		.tm_mday = tm.tm_mday,
		.tm_mon  = tm.tm_mon,
		.tm_year = tm.tm_year, /* both count years from 1900 */
		.tm_wday = tm.tm_wday,
	};
}

/* ── ADC thread ──────────────────────────────────────────────────────────── */

#define ADC_STACK_SIZE 768
#define ADC_PRIORITY   6

static K_THREAD_STACK_DEFINE(adc_stack, ADC_STACK_SIZE);
static struct k_thread adc_thread;

static void adc_thread_fn(void *a, void *b, void *c)
{
	ARG_UNUSED(a);
	ARG_UNUSED(b);
	ARG_UNUSED(c);

	uint32_t iter = 0;
	int32_t  prev_temp = INT32_MIN;

	while (1) {
		k_sleep(K_MSEC(ADC_PERIOD_MS));
		iter++;

		int32_t raw;

		if (adc_read_avg(ADC_AVG_SAMPLES, &raw) < 0) {
			printf("ADC read error\n");
			continue;
		}

		int32_t temp = adc_raw_to_celsius(raw);

		atomic_set(&g_temp_01c, (atomic_val_t)temp);

		if (temp < (int32_t)atomic_get(&g_min_temp)) {
			atomic_set(&g_min_temp, (atomic_val_t)temp);
		}
		if (temp > (int32_t)atomic_get(&g_max_temp)) {
			atomic_set(&g_max_temp, (atomic_val_t)temp);
		}

		struct rtc_time now;

		if (rtc_get_time(RTC_DEV, &now) < 0) {
			continue;
		}

		const char *event = "";

		if (prev_temp != INT32_MIN) {
			int32_t delta = temp - prev_temp;

			if (delta >= TEMP_EVENT_THRESHOLD) {
				event = "^ WARMING";
			} else if (delta <= -TEMP_EVENT_THRESHOLD) {
				event = "v COOLING";
			}
		}
		prev_temp = temp;

		printf("[%02d:%02d:%02d]  %d.%01d \xc2\xb0" "C  %s\n",
		       now.tm_hour, now.tm_min, now.tm_sec,
		       temp / 10,
		       (uint32_t)(temp < 0 ? -temp : temp) % 10,
		       event);

		if (iter % SUMMARY_INTERVAL == 0) {
			int32_t mn = (int32_t)atomic_get(&g_min_temp);
			int32_t mx = (int32_t)atomic_get(&g_max_temp);

			printf("  -- %u s  min=%d.%01d  max=%d.%01d"
			       "  cur=%d.%01d \xc2\xb0" "C --\n",
			       iter,
			       mn / 10, (uint32_t)(mn < 0 ? -mn : mn) % 10,
			       mx / 10, (uint32_t)(mx < 0 ? -mx : mx) % 10,
			       temp / 10, (uint32_t)(temp < 0 ? -temp : temp) % 10);
		}
	}
}

/* ── main ────────────────────────────────────────────────────────────────── */

int main(void)
{
	int ret;
	uint32_t cause = 0;

	hwinfo_get_reset_cause(&cause);
	hwinfo_clear_reset_cause();

	printf("TI MSPM0 RTC + Temperature + LCD\n");
	printf("  S2 (PB8) = forward   S1 (PA18) = backward\n");
	printf("  TEMP -> TIME -> DATE  (auto-return in %u s)\n\n",
	       AUTO_RETURN_MS / 1000U);
	if (cause & RESET_WATCHDOG) {
		printf("  *** Reset by watchdog ***\n\n");
	}

	/* 1. Device readiness */
	if (!device_is_ready(ADC_DEV) || !device_is_ready(RTC_DEV) ||
	    !device_is_ready(LCD_DEV)) {
		printf("Error: peripheral not ready\n");
		return -ENODEV;
	}
	if (!gpio_is_ready_dt(&btn_s1) || !gpio_is_ready_dt(&btn_s2)) {
		printf("Error: button GPIO not ready\n");
		return -ENODEV;
	}

	/* 2. Configure buttons with interrupt */
	gpio_pin_configure_dt(&btn_s1, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&btn_s1, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&btn_s1_cb_data, btn_pressed, BIT(btn_s1.pin));
	gpio_add_callback(btn_s1.port, &btn_s1_cb_data);

	gpio_pin_configure_dt(&btn_s2, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&btn_s2, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&btn_s2_cb_data, btn_pressed, BIT(btn_s2.pin));
	gpio_add_callback(btn_s2.port, &btn_s2_cb_data);

	/* 3. Query LCD geometry */
	struct auxdisplay_capabilities lcd_caps;

	ret = auxdisplay_capabilities_get(LCD_DEV, &lcd_caps);
	if (ret < 0) {
		printf("Error: auxdisplay_capabilities_get: %d\n", ret);
		return ret;
	}
	uint16_t lcd_digits = lcd_caps.columns * lcd_caps.rows;

	/* 4. LCD startup: all segments on for hardware verification */
	char *all_seg = k_malloc(lcd_digits * 2 + 1);

	if (!all_seg) {
		printf("Error: out of memory\n");
		return -ENOMEM;
	}
	for (uint16_t i = 0; i < lcd_digits; i++) {
		all_seg[i * 2]     = '8';
		all_seg[i * 2 + 1] = '.';
	}
	all_seg[lcd_digits * 2] = '\0';
	ret = auxdisplay_write(LCD_DEV, (const uint8_t *)all_seg, lcd_digits * 2);
	k_free(all_seg);
	if (ret < 0) {
		printf("Error: startup write: %d\n", ret);
		return ret;
	}
	k_sleep(K_MSEC(LCD_STARTUP_SHOW_MS));
	auxdisplay_clear(LCD_DEV);

	/* 5. Configure ADC */
	struct adc_channel_cfg adc_cfg = {
		.gain             = ADC_GAIN_1,
		.reference        = ADC_REF_INTERNAL,
		.acquisition_time = ADC_ACQ_TIME_DEFAULT,
		.channel_id       = ADC_CH,
	};
	ret = adc_channel_setup(ADC_DEV, &adc_cfg);
	if (ret < 0) {
		printf("Error: adc_channel_setup: %d\n", ret);
		return ret;
	}
	g_adc_seq = (struct adc_sequence){
		.channels    = BIT(ADC_CH),
		.buffer      = &g_raw_buf,
		.buffer_size = sizeof(g_raw_buf),
		.resolution  = ADC_RESOLUTION,
	};

	/* Discard warmup conversions — VREF settling */
	for (uint32_t i = 0; i < ADC_WARMUP_SAMPLES; i++) {
		(void)adc_read(ADC_DEV, &g_adc_seq);
	}

	/* 6. Boot temperature — seed shared atomics before ADC thread starts */
	int32_t boot_raw;

	ret = adc_read_avg(ADC_AVG_SAMPLES, &boot_raw);
	if (ret < 0) {
		printf("Error: boot adc_read: %d\n", ret);
		return ret;
	}
	int32_t boot_temp = adc_raw_to_celsius(boot_raw);

	atomic_set(&g_temp_01c, (atomic_val_t)boot_temp);
	atomic_set(&g_min_temp, (atomic_val_t)boot_temp);
	atomic_set(&g_max_temp, (atomic_val_t)boot_temp);

	/* 7. Seed RTC from build timestamp */
	struct rtc_time boot_time = rtc_from_build_time();

	ret = rtc_set_time(RTC_DEV, &boot_time);
	if (ret < 0) {
		printf("Error: rtc_set_time: %d\n", ret);
		return ret;
	}

	if (RTC_CAL_PPB != 0) {
		rtc_set_calibration(RTC_DEV, RTC_CAL_PPB);
	}

	printf("Boot temp: %d.%01d \xc2\xb0" "C  %02d/%02d/%04d %02d:%02d:%02d\n\n",
	       boot_temp / 10,
	       (uint32_t)(boot_temp < 0 ? -boot_temp : boot_temp) % 10,
	       boot_time.tm_mday, boot_time.tm_mon + 1, boot_time.tm_year + 1900,
	       boot_time.tm_hour, boot_time.tm_min, boot_time.tm_sec);
	printf("%-12s  %-10s  %s\n", "Time", "Temp(\xc2\xb0" "C)", "Event");
	printf("--------------------------------------------\n");

	/* 8. Start ADC thread — after init so g_adc_seq and devices are ready */
	k_thread_create(&adc_thread, adc_stack, K_THREAD_STACK_SIZEOF(adc_stack),
			adc_thread_fn, NULL, NULL, NULL,
			ADC_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(&adc_thread, "adc");

	/* 9. Display loop */
	/*
	 * Each digit slot may carry one decorator ('.' / ':'), so worst case is
	 * 2 chars per digit, plus a trailing 'C' and NUL.  Sized from the
	 * runtime LCD geometry.
	 */
	size_t lcd_buf_len = lcd_digits * 2 + 2;
	char *lcd_buf = k_calloc(2, lcd_buf_len);

	if (lcd_buf == NULL) {
		printf("Error: out of memory\n");
		return -ENOMEM;
	}
	char *prev_lcd_buf = lcd_buf + lcd_buf_len; /* zeroed: forces first render */
	int  prev_disp_state = -1;

	while (1) {
		k_sleep(K_MSEC(DISPLAY_PERIOD_MS));

		/* Auto-return to TEMP after inactivity */
		uint32_t last_btn = (uint32_t)atomic_get(&g_last_btn_ms);

		if (last_btn != 0 &&
		    k_uptime_get_32() - last_btn > AUTO_RETURN_MS) {
			atomic_set(&g_state, STATE_TEMP);
			atomic_set(&g_last_btn_ms, 0);
		}

		int state = (int)atomic_get(&g_state);

		/* Log state transitions to UART */
		if (state != prev_disp_state) {
			printf(">> %s\n", state_names[state]);
		}

		/* RTC read — used by TIME and DATE states */
		struct rtc_time now;

		rtc_get_time(RTC_DEV, &now);

		/* Format LCD for current state */
		int write_len;

		switch (state) {
		case STATE_TEMP: {
			/*
			 * '.' is a decorator — no digit slot consumed.
			 * format_lcd_temp into (lcd_digits) chars fills
			 * (lcd_digits-1) physical slots.  Appending ' ' and 'C'
			 * places them in the last two slots, to the right of °.
			 */
			int32_t temp = (int32_t)atomic_get(&g_temp_01c);

			format_lcd_temp(lcd_buf, temp, lcd_digits - 1);
			lcd_buf[lcd_digits - 1] = ' ';
			lcd_buf[lcd_digits]     = 'C';
			write_len = lcd_digits + 1;
			break;
		}
		case STATE_TIME:
			/* ':' = COL2 / COL3 decorators; 6 digit slots */
			snprintf(lcd_buf, lcd_buf_len, "%02d:%02d:%02d",
				 now.tm_hour, now.tm_min, now.tm_sec);
			write_len = 8;
			break;
		case STATE_DATE:
			/* '.' = P-dot decorators; 6 digit slots */
			snprintf(lcd_buf, lcd_buf_len, "%02u.%02u.%02u",
				 (unsigned int)(now.tm_mday & 0x1f),
				 (unsigned int)((now.tm_mon + 1) & 0x0f),
				 (unsigned int)((now.tm_year + 1900) % 100));
			write_len = 8;
			break;
		default:
			continue;
		}

		/* Only write to LCD when displayed content actually changes */
		if (state == prev_disp_state &&
		    memcmp(lcd_buf, prev_lcd_buf, lcd_buf_len) == 0) {
			continue;
		}

		auxdisplay_clear(LCD_DEV);
		auxdisplay_write(LCD_DEV, (const uint8_t *)lcd_buf, write_len);

		/* ° indicator: on in TEMP state only */
		auxdisplay_custom_indicator_set(LCD_DEV, LCD_INDICATOR_DEG_C,
						state == STATE_TEMP);

		memcpy(prev_lcd_buf, lcd_buf, lcd_buf_len);
		prev_disp_state = state;
	}

	return 0;
}
