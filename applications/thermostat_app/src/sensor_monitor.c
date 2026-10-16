/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

#include "ti_thermostat.h"
#include "sensor_monitor.h"

LOG_MODULE_REGISTER(sensor_monitor, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *hdc_dev;
static const struct device *rtc_dev;
static lv_timer_t *sensor_timer;

static void sensor_tick_cb(lv_timer_t *timer)
{
	struct sensor_value temp, humidity;
	struct rtc_time tm;
	char time_str[6];
	int rc;

	LV_UNUSED(timer);

	rc = sensor_sample_fetch(hdc_dev);
	if (rc != 0) {
		LOG_WRN("Failed to fetch sensor sample: %d", rc);
		return;
	}

	rc = sensor_channel_get(hdc_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
	if (rc != 0) {
		LOG_WRN("Failed to get temperature: %d", rc);
		return;
	}

	rc = sensor_channel_get(hdc_dev, SENSOR_CHAN_HUMIDITY, &humidity);
	if (rc != 0) {
		LOG_WRN("Failed to get humidity: %d", rc);
		return;
	}

	float temp_c = sensor_value_to_float(&temp);
	int hum_pct = (int)(sensor_value_to_double(&humidity) + 0.5);

	rc = rtc_get_time(rtc_dev, &tm);
	if (rc != 0) {
		LOG_WRN("Failed to get RTC time: %d", rc);
		return;
	}

	snprintf(time_str, sizeof(time_str), "%02d:%02d", tm.tm_hour, tm.tm_min);

	lv_subject_set_float(&subject_now_temp, temp_c);
	lv_subject_set_int(&subject_humidity, hum_pct);
	lv_subject_copy_string(&subject_time, time_str);
}

int sensor_monitor_init(void)
{
	hdc_dev = DEVICE_DT_GET_ONE(ti_hdc2080);
	if (!device_is_ready(hdc_dev)) {
		LOG_ERR("HDC2080 device not ready");
		return -ENODEV;
	}

	rtc_dev = DEVICE_DT_GET(DT_ALIAS(rtc));
	if (!device_is_ready(rtc_dev)) {
		LOG_ERR("RTC device not ready");
		return -ENODEV;
	}

	sensor_timer = lv_timer_create(sensor_tick_cb, 1000, NULL);
	if (sensor_timer == NULL) {
		LOG_ERR("Failed to create sensor timer");
		return -ENOMEM;
	}

	LOG_INF("Sensor monitor initialized (HDC2080 + RTC)");
	return 0;
}
