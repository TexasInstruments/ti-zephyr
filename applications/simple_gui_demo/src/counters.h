/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#ifndef SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_H_
#define SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_H_

#include <stdbool.h>
#include <stdint.h>

void counters_init(void);

void counters_set_enable(bool enable);

void counters_set_increment(uint16_t increment);

#endif /* SAMPLE_SIMPLE_GUI_DEMO_COUNTERS_H_ */
