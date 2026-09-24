/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/comparator.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

K_SEM_DEFINE(wake_sem, 0, 1);

static void comparator_wakeup_cb(const struct device *dev, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	k_sem_give(&wake_sem);
}

int main(void)
{
	const struct device *dev = DEVICE_DT_GET(DT_ALIAS(wakeup_comp));
	uint32_t wake_count = 0;
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

	while (true) {
		printf("going to sleep\n");

		k_sem_take(&wake_sem, K_FOREVER);

		printf("woke up (count=%u)\n", ++wake_count);
	}

	return 0;
}
