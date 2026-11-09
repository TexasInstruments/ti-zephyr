/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include <stdio.h>

#include <zephyr/kernel.h>

#include <mathlib/fastrts_tmu.h>

#define TOLERANCE 1.0e-3f

struct test_case {
	const char *name;
	float in_a;
	float in_b;
	float (*tmu)(float a, float b);
	float (*ref)(float a, float b);
};

#define UNARY(fn)                                                                                  \
	static float tmu_##fn(float a, float b) { ARG_UNUSED(b); return fn##_tmu(a); }             \
	static float ref_##fn(float a, float b) { ARG_UNUSED(b); return fn(a); }

UNARY(sinf)
UNARY(cosf)
UNARY(atanf)
UNARY(asinf)
UNARY(acosf)
UNARY(sqrtf)
UNARY(expf)
UNARY(logf)

static float tmu_atan2f(float y, float x) { return atan2f_tmu(y, x); }
static float ref_atan2f(float y, float x) { return atan2f(y, x); }
static float tmu_divf(float y, float x) { return divf_tmu(y, x); }
static float ref_divf(float y, float x) { return y / x; }

#define CASE(fn, a, b) { #fn "(" #a ", " #b ")", a, b, tmu_##fn, ref_##fn }

static const struct test_case cases[] = {
	CASE(sinf, 0.5f, 0.0f),     CASE(sinf, -2.0f, 0.0f),   CASE(sinf, 3.0f, 0.0f),
	CASE(cosf, 0.5f, 0.0f),     CASE(cosf, -2.0f, 0.0f),   CASE(cosf, 3.0f, 0.0f),
	CASE(atanf, 0.5f, 0.0f),    CASE(atanf, -3.0f, 0.0f),   CASE(atanf, 8.0f, 0.0f),
	CASE(asinf, 0.5f, 0.0f),    CASE(asinf, -0.9f, 0.0f),
	CASE(acosf, 0.5f, 0.0f),    CASE(acosf, -0.9f, 0.0f),
	CASE(sqrtf, 2.0f, 0.0f),    CASE(sqrtf, 100.0f, 0.0f),
	CASE(expf, 1.0f, 0.0f),     CASE(expf, -2.5f, 0.0f),
	CASE(logf, 2.0f, 0.0f),     CASE(logf, 10.0f, 0.0f),
	CASE(atan2f, 1.0f, 2.0f),   CASE(atan2f, -1.0f, -2.0f),
	CASE(divf, 7.0f, 2.0f),     CASE(divf, -9.0f, 4.0f),
};

int main(void)
{
	int passed = 0;

	fastrts_tmu_clear_flags(FASTRTS_TMU_FLAGS_LUF | FASTRTS_TMU_FLAGS_LVF);

	for (size_t i = 0; i < ARRAY_SIZE(cases); i++) {
		const struct test_case *c = &cases[i];
		float got = c->tmu(c->in_a, c->in_b);
		float want = c->ref(c->in_a, c->in_b);
		float err = fabsf(got - want) / fmaxf(fabsf(want), 1.0f);
		bool ok = err <= TOLERANCE;

		printf("%-4s %-18s tmu=%f ref=%f rel_err=%e\n", ok ? "PASS" : "FAIL", c->name,
		       (double)got, (double)want, (double)err);
		passed += ok;
	}

	printf("TMU flags: 0x%x\n", fastrts_tmu_get_flags());
	printf("Passed %d of %d\n", passed, (int)ARRAY_SIZE(cases));
	printf("Overall Result: %s\n", passed == ARRAY_SIZE(cases) ? "PASS" : "FAIL");

	return 0;
}
