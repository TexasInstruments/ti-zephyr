/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <lvgl.h>

#include "motor_control.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

int main(void)
{
	const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	int ret;

	if (!device_is_ready(display_dev)) {
		LOG_ERR("Display device not ready");
		return 0;
	}

	/*
	 * Build the motor control UI (subjects, screens, logic + simulator)
	 * and load the run screen. Assets are compiled in, so the asset-path
	 * argument is unused on this target.
	 */
	motor_control_init("A:");

	/* Render one frame, then unblank the panel. */
	lv_timer_handler();
	ret = display_blanking_off(display_dev);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn display blanking off (%d)", ret);
		return 0;
	}

	while (1) {
		uint32_t sleep_ms = lv_timer_handler();

		if (sleep_ms == LV_NO_TIMER_READY) {
			sleep_ms = LV_DEF_REFR_PERIOD;
		}
		k_sleep(K_MSEC(sleep_ms));
	}

	return 0;
}
