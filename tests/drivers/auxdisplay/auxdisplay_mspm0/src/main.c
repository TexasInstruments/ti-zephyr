/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 *
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/auxdisplay.h>
#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#define DISPLAY_COLS 6
#define DISPLAY_ROWS 1
#define PANEL_NODE   DT_NODELABEL(slcd_panel)

/* ms delays for visual observation; kept short to limit total test time. */
#define STEP_MS  600
#define HOLD_MS 1500

static const struct device *lcd_dev;

static void *suite_setup(void)
{
	lcd_dev = DEVICE_DT_GET(DT_NODELABEL(auxdisplay_0));
	zassert_true(device_is_ready(lcd_dev), "LCD device not ready");
	zassert_ok(auxdisplay_display_on(lcd_dev));
	return NULL;
}

/* Clear + home + write a C-string padded to DISPLAY_COLS. */
static void show(const char *s)
{
	char buf[DISPLAY_COLS];
	int  len = MIN((int)strlen(s), DISPLAY_COLS);

	memcpy(buf, s, len);
	memset(buf + len, ' ', DISPLAY_COLS - len);
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)buf, DISPLAY_COLS));
}

ZTEST_SUITE(auxdisplay_mspm0, NULL, suite_setup, NULL, NULL, NULL);

ZTEST(auxdisplay_mspm0, test_01_capabilities)
{
	struct auxdisplay_capabilities cap;

	zassert_ok(auxdisplay_capabilities_get(lcd_dev, &cap));
	zassert_equal(cap.columns, DISPLAY_COLS,
		      "expected %d columns, got %d", DISPLAY_COLS, cap.columns);
	zassert_equal(cap.rows, DISPLAY_ROWS,
		      "expected %d rows, got %d", DISPLAY_ROWS, cap.rows);
}

ZTEST(auxdisplay_mspm0, test_02_display_on_off)
{
	show("888888");
	k_sleep(K_MSEC(STEP_MS));

	zassert_ok(auxdisplay_display_off(lcd_dev));
	/* glass should be blank — observe */
	k_sleep(K_MSEC(HOLD_MS));

	zassert_ok(auxdisplay_display_on(lcd_dev));
	/* 888888 should reappear */
	k_sleep(K_MSEC(STEP_MS));
}

ZTEST(auxdisplay_mspm0, test_03_cursor)
{
	int16_t cx, cy;

	/* Absolute (0,0) */
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 0);
	zassert_equal(cy, 0);

	/* Absolute (3,0) */
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 3, 0));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 3);
	zassert_equal(cy, 0);

	/* Relative +1 → should land at (4,0) */
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_RELATIVE, 1, 0));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 4, "relative +1 from (3,0) should give x=4, got %d", cx);
	zassert_equal(cy, 0);

	/* Relative -2 → should land at (2,0) */
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_RELATIVE, -2, 0));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 2, "relative -2 from (4,0) should give x=2, got %d", cx);

	/* RELATIVE_DIRECTION → -ENOTSUP */
	zassert_equal(auxdisplay_cursor_position_set(
			      lcd_dev, AUXDISPLAY_POSITION_RELATIVE_DIRECTION, 1, 0),
		      -ENOTSUP);

	/* Out-of-bounds → -EINVAL */
	zassert_equal(auxdisplay_cursor_position_set(
			      lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 99, 0),
		      -EINVAL);
	zassert_equal(auxdisplay_cursor_position_set(
			      lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 99),
		      -EINVAL);
	zassert_equal(auxdisplay_cursor_position_set(
			      lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, -1, 0),
		      -EINVAL);

	/* Write two chars → cursor must advance by 2 */
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"AB", 2));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 2, "cursor should be at 2 after writing 2 chars, got %d", cx);
	zassert_equal(cy, 0);
}

ZTEST(auxdisplay_mspm0, test_04_clear)
{
	int16_t cx, cy;

	show("ABCDEF");
	k_sleep(K_MSEC(STEP_MS));

	zassert_ok(auxdisplay_clear(lcd_dev));

	/* Cursor must be reset to origin */
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 0, "cursor x should be 0 after clear, got %d", cx);
	zassert_equal(cy, 0, "cursor y should be 0 after clear, got %d", cy);

	/* Glass should be blank — observe */
	k_sleep(K_MSEC(STEP_MS));

	/* Double-clear must not corrupt COM bits — digit write must still work */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_clear(lcd_dev));
	show("012345");
	k_sleep(K_MSEC(STEP_MS));
}

ZTEST(auxdisplay_mspm0, test_05_digits)
{
	show("012345");
	k_sleep(K_MSEC(STEP_MS));
	show("6789  ");
	k_sleep(K_MSEC(STEP_MS));

	/* Per-position digit scan */
	zassert_ok(auxdisplay_clear(lcd_dev));
	for (int pos = 0; pos < DISPLAY_COLS; pos++) {
		for (int d = 0; d <= 9; d++) {
			uint8_t ch = (uint8_t)('0' + d);

			zassert_ok(auxdisplay_cursor_position_set(
				lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, pos, 0));
			zassert_ok(auxdisplay_write(lcd_dev, &ch, 1));
			k_sleep(K_MSEC(100));
		}
	}

	/* Write past end of display must return 0 and leave cursor in bounds */
	{
		int16_t cx, cy;

		zassert_ok(auxdisplay_cursor_position_set(
			lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE,
			DISPLAY_COLS - 1, 0));
		zassert_ok(auxdisplay_write(
			lcd_dev, (const uint8_t *)"012", 3));
		zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
		zassert_true(cx <= DISPLAY_COLS,
			     "cursor x out of bounds after overrun: %d", cx);
		zassert_equal(cy, 0);
	}
}

ZTEST(auxdisplay_mspm0, test_06_letters)
{
	for (int i = 0; i < 26; i += DISPLAY_COLS) {
		char buf[DISPLAY_COLS + 1];
		int  n = MIN(DISPLAY_COLS, 26 - i);

		for (int j = 0; j < n; j++) {
			buf[j] = (char)('A' + i + j);
		}
		memset(buf + n, ' ', DISPLAY_COLS - n);
		buf[DISPLAY_COLS] = '\0';
		show(buf);
		k_sleep(K_MSEC(STEP_MS));
	}

	/* Lowercase must be folded to uppercase and advance cursor normally */
	{
		int16_t cx, cy;

		zassert_ok(auxdisplay_cursor_position_set(
			lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
		zassert_ok(auxdisplay_write(
			lcd_dev, (const uint8_t *)"abcdef", DISPLAY_COLS));
		zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
		zassert_equal(cx, DISPLAY_COLS,
			      "cursor should advance to end after lowercase write, got %d", cx);
	}
}

ZTEST(auxdisplay_mspm0, test_07_decorators)
{
	int16_t cx, cy;

	/*
	 * LP-MSPM0L2228 has no indicator pins so decorators do not light
	 * physical segments.  The API must still return 0 and must NOT advance
	 * the cursor for the decorator character itself.
	 */

	/* "1.2" → two alphanumeric positions written, cursor at (2,0) */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"1.2", 3));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 2, "'.' must not advance cursor: x=%d, want 2", cx);
	k_sleep(K_MSEC(STEP_MS));

	/* "3:4" → cursor at (2,0) */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"3:4", 3));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 2, "':' must not advance cursor: x=%d, want 2", cx);
	k_sleep(K_MSEC(STEP_MS));

	/* Leading '.' at position 0 is silently dropped (not a decorator). */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)".5", 2));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	/*
	 * Leading '.' is consumed as a regular character (the symbol guard
	 * i != 0 skips it).  It maps to SLCD_BLANK (0) so no segments light,
	 * and the cursor advances by 1 for the '.' then by 1 for '5'.
	 */
	zassert_equal(cx, 2, "leading '.' + '5': cursor x=%d, want 2", cx);
	k_sleep(K_MSEC(STEP_MS));
}

ZTEST(auxdisplay_mspm0, test_08_indicators)
{
	/*
	 * LP-MSPM0L2228 has 9 indicators (idx 0-8):
	 *   0=P1, 1=P2, 2=P3, 3=P4, 4=P5, 5=COL2, 6=COL3, 7=COL4, 8=DP
	 */

	/* Turn all indicators on — observe all annunciators and colons lit */
	for (int i = 0; i < 9; i++) {
		zassert_ok(auxdisplay_custom_indicator_set(lcd_dev, i, true),
			   "indicator(%d) on should return 0", i);
	}
	k_sleep(K_MSEC(HOLD_MS));

	/* Turn all indicators off */
	for (int i = 0; i < 9; i++) {
		zassert_ok(auxdisplay_custom_indicator_set(lcd_dev, i, false),
			   "indicator(%d) off should return 0", i);
	}
	k_sleep(K_MSEC(STEP_MS));

	/* Toggle each indicator individually — visual scan */
	for (int i = 0; i < 9; i++) {
		zassert_ok(auxdisplay_custom_indicator_set(lcd_dev, i, true));
		k_sleep(K_MSEC(300));
		zassert_ok(auxdisplay_custom_indicator_set(lcd_dev, i, false));
	}

	/* Out-of-range index must return -EINVAL */
	zassert_equal(auxdisplay_custom_indicator_set(lcd_dev, 9, true),
		      -EINVAL, "index 9 (out of range) must return -EINVAL");
	zassert_equal(auxdisplay_custom_indicator_set(lcd_dev, 255, true),
		      -EINVAL, "index 255 must return -EINVAL");
}

ZTEST(auxdisplay_mspm0, test_09_decorators_with_indicators)
{
	int16_t cx, cy;

	/* "12:34" — COL2 (gap1, between dig2 and dig3) lights, cursor at 4 */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"12:34", 5));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 4, "':' must not advance cursor: x=%d want 4", cx);
	k_sleep(K_MSEC(HOLD_MS));

	/* "1234:5" — COL3 (gap3, between dig4 and dig5) lights, cursor at 5 */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"1234:5", 6));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 5, "':' must not advance cursor: x=%d want 5", cx);
	k_sleep(K_MSEC(HOLD_MS));

	/* "1.2" — P1 (gap0, after dig1) lights, cursor at 2 */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"1.2", 3));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 2, "'.' must not advance cursor: x=%d want 2", cx);
	k_sleep(K_MSEC(STEP_MS));

	/* "12.3" — P2 (gap1, after dig2) lights alongside COL2 independently */
	zassert_ok(auxdisplay_clear(lcd_dev));
	zassert_ok(auxdisplay_cursor_position_set(
		lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"12.3", 4));
	zassert_ok(auxdisplay_cursor_position_get(lcd_dev, &cx, &cy));
	zassert_equal(cx, 3, "'.' at gap1 must not advance cursor: x=%d want 3", cx);
	k_sleep(K_MSEC(STEP_MS));

	zassert_ok(auxdisplay_clear(lcd_dev));
}

ZTEST(auxdisplay_mspm0, test_10_blinking)
{
	/*
	 * LP-MSPM0L2228 is 4-mux (mux_mode = 3 ≤ 3), so hardware blinking
	 * is supported.  A 1.5-second window lets the observer see the glass
	 * toggle.
	 */
	show("BLINK ");
	k_sleep(K_MSEC(STEP_MS));

	zassert_ok(auxdisplay_position_blinking_set_enabled(lcd_dev, true));
	/* observe blinking */
	k_sleep(K_MSEC(HOLD_MS));

	zassert_ok(auxdisplay_position_blinking_set_enabled(lcd_dev, false));
	/* observe steady state restored */
	k_sleep(K_MSEC(STEP_MS));

	/* Enable, then clear, then disable — must not crash or corrupt. */
	zassert_ok(auxdisplay_position_blinking_set_enabled(lcd_dev, true));
	zassert_ok(auxdisplay_clear(lcd_dev));
	show("NOBL  ");
	k_sleep(K_MSEC(STEP_MS));
	zassert_ok(auxdisplay_position_blinking_set_enabled(lcd_dev, false));
}

ZTEST(auxdisplay_mspm0, test_11_all_segments)
{
	/* 8 activates all 14 segments on a 14-seg display. */
	show("888888");
	k_sleep(K_MSEC(HOLD_MS));
}

ZTEST(auxdisplay_mspm0, test_12_rotated_encoding)
{
	/*
	 * Symmetric digits look the same right-side-up and upside-down.
	 * Asymmetric ones differ.  6 ↔ 9 swap when rotated.
	 * Build with lp_mspm0l2228_rotated.overlay to activate the rotated
	 * tables and flip the board to verify.
	 */
	show("0 2 8 ");  /* symmetric */
	k_sleep(K_MSEC(STEP_MS));
	show("1 3 4 ");  /* asymmetric */
	k_sleep(K_MSEC(STEP_MS));
	show("5 6 9 ");  /* 6↔9 swap when rotated */
	k_sleep(K_MSEC(STEP_MS));
	show("7     ");  /* A+B+C → D+E+F when upside-down */
	k_sleep(K_MSEC(STEP_MS));
}

ZTEST(auxdisplay_mspm0, test_13_write_display_off)
{
	/*
	 * Writing while display is off succeeds — data goes to LCDM shadow
	 * and becomes visible when display_on() is called.
	 */
	zassert_ok(auxdisplay_display_off(lcd_dev));
	zassert_ok(auxdisplay_write(lcd_dev, (const uint8_t *)"OFFWRT", 6));
	k_sleep(K_MSEC(STEP_MS));

	/* Turn on — "OFFWRT" should appear */
	zassert_ok(auxdisplay_display_on(lcd_dev));
	k_sleep(K_MSEC(STEP_MS));
	zassert_ok(auxdisplay_clear(lcd_dev));
}

ZTEST(auxdisplay_mspm0, test_14_counter_stress)
{
	const uint32_t iterations = 500;
	char buf[DISPLAY_COLS + 1];

	for (uint32_t i = 0; i < iterations; i++) {
		snprintf(buf, sizeof(buf), "%06u", i % 1000000U);
		zassert_ok(auxdisplay_cursor_position_set(
			lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0));
		zassert_ok(auxdisplay_write(
			lcd_dev, (const uint8_t *)buf, DISPLAY_COLS));
		k_sleep(K_MSEC(20));
	}
}
