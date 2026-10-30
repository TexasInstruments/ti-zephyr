/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/comparator.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

static void comparator_wakeup_cb(const struct device *dev, void *user_data)
{
	static uint32_t wake_count = 0;

	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	printf("woke up (count=%u)\n", ++wake_count);
}

int main(void)
{
	const struct device *dev = DEVICE_DT_GET(DT_ALIAS(wakeup_comp));
	int ret = 0;

	if (!device_is_ready(dev)) {
		printf("comparator device not ready\n");
		return 0;
	}

	ret = comparator_set_trigger_callback(dev, comparator_wakeup_cb, NULL);
	if (ret < 0) {
		printf("failed to set trigger callback (%d)\n", ret);
		return 0;
	}

	ret = comparator_set_trigger(dev, COMPARATOR_TRIGGER_RISING_EDGE);
	if (ret < 0) {
		printf("failed to enable comparator trigger (%d)\n", ret);
		return 0;
	}

	return 0;
}
