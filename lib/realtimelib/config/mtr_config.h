/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Motor and system constants that the SDK sources (mtpa.c and vsf.c) take from
 * the application header "mtr_config.h". Here they come from Kconfig.
 */

#ifndef REALTIMELIB_MTR_CONFIG_H
#define REALTIMELIB_MTR_CONFIG_H

#include <zephyr/autoconf.h>

/* mtpa: stator inductances in H, from uH */
#define USER_MOTOR_Ls_d_H (CONFIG_TI_REALTIMELIB_MTPA_LS_D_UH * 1.0e-6f)
#define USER_MOTOR_Ls_q_H (CONFIG_TI_REALTIMELIB_MTPA_LS_Q_UH * 1.0e-6f)

/* vsf: PWM frequency in kHz */
#define USER_M1_PWM_FREQ_kHz CONFIG_TI_REALTIMELIB_VSF_PWM_FREQ_KHZ

#endif /* REALTIMELIB_MTR_CONFIG_H */
