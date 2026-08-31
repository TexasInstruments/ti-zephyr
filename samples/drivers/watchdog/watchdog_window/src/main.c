/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief TI MSPM0 WWDT Window Mode
 *
 * Demonstrates the MSPM0 Window Watchdog Timer (WWDT) closed-window
 * constraint using the standard Zephyr watchdog API.
 *
 * The WWDT enforces a feed window:
 *
 *   t=0 ---- closed window ---- t=min == open window == t=max ---- RESET
 *
 * - Feed before t=min: immediate SYSRST (closed-window violation)
 * - Feed after  t=max: immediate SYSRST (timeout)
 * - Feed in [t=min, t=max): safe zone
 *
 * The upstream watchdog sample sets WDT_MIN_WINDOW=0 for MSPM0 so the closed
 * window is never exercised. This sample demonstrates all three zones across
 * two boots:
 *
 *   Boot 1: five correct feeds at 1000 ms, then a deliberate early feed at
 *           100 ms triggers a SYSRST.
 *   Boot 2: hwinfo_get_reset_cause() returns RESET_WATCHDOG confirming the
 *           window violation; sample then feeds correctly.
 *
 * SDK analog: wwdt_window_mode_periodic_reset
 *
 * Note: MSPM0 WWDT violations route through the ESM safety block, not the
 * NVIC, so a pre-reset callback is architecturally impossible.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/hwinfo.h>
#include <stdio.h>

#if !DT_HAS_COMPAT_STATUS_OKAY(ti_mspm0_watchdog)
#error "No ti,mspm0-watchdog node. Add 'watchdog0 = &wdt0' alias to the board overlay."
#endif

#define WDT_NODE DT_ALIAS(watchdog0)

/*
 * The WWDT reset action (CPU core vs full SoC) is a DT property that differs
 * between MSPM0 G-series (<0> = CPU_CORE) and L-series (<1> = SOC).
 * Derive the flag from the same DT property the driver uses so that
 * wdt_install_timeout() does not reject a mismatched flag.
 */
#define WDT_RESET_FLAG                                                                             \
	((DT_PROP(WDT_NODE, ti_watchdog_reset_action) != 0) ? WDT_FLAG_RESET_SOC                   \
							    : WDT_FLAG_RESET_CPU_CORE)

#define WIN_MIN_MS    500U  /* closed window: feed inside here = SYSRST  */
#define WIN_MAX_MS    2000U /* open window:   feed after this  = SYSRST  */
#define FEED_SAFE_MS  1000U /* safe feed interval (inside open window)   */
#define FEED_EARLY_MS 100U  /* intentional violation: inside closed window */
#define CORRECT_FEEDS 5     /* correct feeds before triggering violation  */

int main(void)
{
	const struct device *wdt = DEVICE_DT_GET(WDT_NODE);
	uint32_t cause = 0;
	uint32_t t_start;
	uint32_t t_feed;
	int wdt_ch;
	int ret;

	if (!device_is_ready(wdt)) {
		printf("Error: %s not ready\n", wdt->name);
		return -ENODEV;
	}

	ret = hwinfo_get_reset_cause(&cause);
	if (ret < 0) {
		printf("Warning: hwinfo not available (%d)\n", ret);
	}
	hwinfo_clear_reset_cause();

	printf("TI MSPM0 WWDT Window Mode\n");

	if (cause & RESET_WATCHDOG) {
		printf("*** Rebooted by WWDT window violation ***\n\n");
	} else {
		printf("Clean boot.\n\n");
	}

	struct wdt_timeout_cfg wdt_cfg = {
		.flags = WDT_RESET_FLAG,
		.window = {
			.min = WIN_MIN_MS,
			.max = WIN_MAX_MS,
		},
		.callback = NULL,
	};

	wdt_ch = wdt_install_timeout(wdt, &wdt_cfg);
	if (wdt_ch < 0) {
		printf("Error: wdt_install_timeout: %d\n", wdt_ch);
		return wdt_ch;
	}

	ret = wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG);
	if (ret < 0) {
		printf("Error: wdt_setup: %d\n", ret);
		return ret;
	}

	/*
	 * Record the start time immediately after wdt_setup() so that elapsed
	 * measurements track the WWDT window precisely from the point the
	 * counter began running.
	 */
	t_start = k_uptime_get_32();
	t_feed = t_start;

	printf("WWDT window:  min=%u ms (closed)  max=%u ms (open)\n", WIN_MIN_MS, WIN_MAX_MS);
	printf("Feed interval: %u ms (inside open window)\n\n", FEED_SAFE_MS);

	/* ------------------------------------------------------------------ */
	/* Phase 2: recovered from violation -- feed correctly                 */
	/* ------------------------------------------------------------------ */
	if (cause & RESET_WATCHDOG) {
		printf("Phase 2: recovered -- feeding correctly.\n");

		for (int n = 1; n <= CORRECT_FEEDS; n++) {
			k_msleep(FEED_SAFE_MS);
			uint32_t elapsed = k_uptime_get_32() - t_feed;

			wdt_feed(wdt, wdt_ch);
			t_feed = k_uptime_get_32();

			printf("  Feed %2d: %u ms (window=[%u..%u) ms)\n", n, elapsed, WIN_MIN_MS,
			       WIN_MAX_MS);
		}

		printf("Window-mode recovery confirmed. Done.\n");

		while (1) {
			k_msleep(FEED_SAFE_MS);
			wdt_feed(wdt, wdt_ch);
		}
	}

	/* ------------------------------------------------------------------ */
	/* Phase 1: correct feeds, then deliberate closed-window violation     */
	/* ------------------------------------------------------------------ */
	printf("Phase 1: %d correct feeds, then deliberate early feed.\n\n", CORRECT_FEEDS);

	for (int i = 0; i < CORRECT_FEEDS; i++) {
		k_msleep(FEED_SAFE_MS);
		uint32_t elapsed = k_uptime_get_32() - t_feed;

		wdt_feed(wdt, wdt_ch);
		t_feed = k_uptime_get_32();

		printf("  Feed %d/%d: %u ms -- OK (inside window)\n", i + 1, CORRECT_FEEDS,
		       elapsed);
	}

	printf("\nFeeding at %u ms (before min=%u ms) -- expects SYSRST...\n", FEED_EARLY_MS,
	       WIN_MIN_MS);

	k_msleep(FEED_EARLY_MS);
	wdt_feed(wdt, wdt_ch); /* inside closed window: t < min -> SYSRST */

	/* Should not reach here */
	printf("Error: early feed did not trigger reset\n");

	return 0;
}
