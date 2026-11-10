/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Motor constants used by mtpa.c to build its inductance tables. The SDK takes
 * them from the application, here they come from Kconfig (in uH).
 */

#ifndef MTPA_MTR_CONFIG_H
#define MTPA_MTR_CONFIG_H

#include <zephyr/autoconf.h>

#define USER_MOTOR_Ls_d_H (CONFIG_TI_REALTIMELIB_MTPA_LS_D_UH * 1.0e-6f)
#define USER_MOTOR_Ls_q_H (CONFIG_TI_REALTIMELIB_MTPA_LS_Q_UH * 1.0e-6f)

#endif /* MTPA_MTR_CONFIG_H */
