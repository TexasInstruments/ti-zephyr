/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#ifndef SENSOR_MONITOR_H
#define SENSOR_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the sensor monitor.
 *
 * Initializes the HDC2080 temperature/humidity sensor and RTC, then
 * creates a periodic LVGL timer to update UI subjects with real data.
 *
 * @retval 0 on success.
 * @retval -ENODEV if a required device is not ready.
 * @retval -ENOMEM if the LVGL timer could not be created.
 */
int sensor_monitor_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MONITOR_H */
