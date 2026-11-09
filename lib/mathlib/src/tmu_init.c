/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/init.h>
#include <cmsis_core.h>

/* CPACR field for coprocessor 0 (the TMU): bits [1:0] */
#define CPACR_CP0_FULL_ACCESS 0x3UL

static int ti_tmu_init(void)
{
	SCB->CPACR |= CPACR_CP0_FULL_ACCESS;
	__DSB();
	__ISB();

	return 0;
}

SYS_INIT(ti_tmu_init, PRE_KERNEL_1, 0);
