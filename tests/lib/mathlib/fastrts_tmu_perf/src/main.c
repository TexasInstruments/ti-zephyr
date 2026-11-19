/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <stdio.h>

#include <zephyr/ztest.h>
#include <cmsis_core.h>

#include <mathlib/fastrts_tmu.h>

/* Calls per measurement, spread over the input table */
#define ITERATIONS 1024
#define NUM_INPUTS 16

typedef float (*fn_t)(float a, float b);

struct perf_case {
	const char *name;
	fn_t tmu;
	fn_t ref;
	/* TMU must be faster than the C library; false where the FPU has an instruction */
	bool expect_faster;
};

static float in_a[NUM_INPUTS];
static float in_b[NUM_INPUTS];
static volatile float sink;

static float f_none(float a, float b)
{
	return a + b;
}

#define UNARY(fn)                                                                                  \
	static float tmu_##fn(float a, float b) { ARG_UNUSED(b); return fn##_tmu(a); }             \
	static float ref_##fn(float a, float b) { ARG_UNUSED(b); return fn(a); }

UNARY(sinf)
UNARY(cosf)
UNARY(atanf)
UNARY(sqrtf)
UNARY(expf)
UNARY(logf)

static float tmu_atan2f(float y, float x) { return atan2f_tmu(y, x); }
static float ref_atan2f(float y, float x) { return atan2f(y, x); }
static float tmu_divf(float y, float x) { return divf_tmu(y, x); }
static float ref_divf(float y, float x) { return y / x; }

static const struct perf_case cases[] = {
	{"sinf", tmu_sinf, ref_sinf, true},
	{"cosf", tmu_cosf, ref_cosf, true},
	{"atanf", tmu_atanf, ref_atanf, true},
	{"atan2f", tmu_atan2f, ref_atan2f, true},
	{"expf", tmu_expf, ref_expf, true},
	{"logf", tmu_logf, ref_logf, true},
	{"sqrtf", tmu_sqrtf, ref_sqrtf, false},
	{"divf", tmu_divf, ref_divf, false},
};

/* Total cycles for ITERATIONS calls of fn */
static uint32_t measure(fn_t fn)
{
	uint32_t start;
	uint32_t end;
	float acc = 0.0f;

	start = DWT->CYCCNT;
	for (int i = 0; i < ITERATIONS; i++) {
		acc += fn(in_a[i % NUM_INPUTS], in_b[i % NUM_INPUTS]);
	}
	end = DWT->CYCCNT;
	sink = acc;

	return end - start;
}

static void *setup(void)
{
	/* Inputs in (0.1, 1.0], valid for every function including log and atan */
	for (int i = 0; i < NUM_INPUTS; i++) {
		in_a[i] = 0.1f + 0.9f * (float)(i + 1) / NUM_INPUTS;
		in_b[i] = 0.2f + 0.8f * (float)(NUM_INPUTS - i) / NUM_INPUTS;
	}

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	zassert_true((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) != 0, "DWT cycle counter unavailable");

	return NULL;
}

ZTEST_SUITE(fastrts_tmu_perf, NULL, setup, NULL, NULL, NULL);

ZTEST(fastrts_tmu_perf, test_tmu_vs_libm)
{
	/* Cycles per call, including the loop and the indirect call, which both sides share */
	printf("\n%-8s %10s %10s %8s\n", "function", "libm", "TMU", "speedup");
	printf("%-8s %7u cy\n", "(empty)", measure(f_none) / ITERATIONS);

	for (size_t i = 0; i < ARRAY_SIZE(cases); i++) {
		uint32_t ref = measure(cases[i].ref) / ITERATIONS;
		uint32_t tmu = measure(cases[i].tmu) / ITERATIONS;

		printf("%-8s %7u cy %7u cy %7.2fx\n", cases[i].name, ref, tmu,
		       (double)ref / (double)(tmu ? tmu : 1));

		if (cases[i].expect_faster) {
			zassert_true(tmu < ref, "%s: TMU %u cycles not faster than libm %u",
				     cases[i].name, tmu, ref);
		}
	}
}
