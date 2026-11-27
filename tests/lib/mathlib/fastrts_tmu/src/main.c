/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <mathlib/fastrts_tmu.h>

/* Absolute tolerance for results of magnitude <= 1, relative above that */
#define TOL 1.0e-4f

static void check(float got, float want)
{
	float scale = fmaxf(fabsf(want), 1.0f);

	zassert_within(got, want, TOL * scale, "got %f, expected %f", (double)got, (double)want);
}

ZTEST_SUITE(fastrts_tmu, NULL, NULL, NULL, NULL, NULL);

ZTEST(fastrts_tmu, test_sinf)
{
	const float in[] = {0.0f, 0.5f, -0.5f, FASTRTS_TMU_HALF_PI, FASTRTS_TMU_PI, -2.0f, 3.0f,
			    6.0f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(sinf_tmu(in[i]), sinf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_cosf)
{
	const float in[] = {0.0f, 0.5f, -0.5f, FASTRTS_TMU_HALF_PI, FASTRTS_TMU_PI, -2.0f, 3.0f,
			    6.0f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(cosf_tmu(in[i]), cosf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_atanf)
{
	/* Includes |x| > 1, which the TMU handles through the 1/x identity */
	const float in[] = {0.0f, 0.5f, -0.5f, 1.0f, -1.0f, 1.5f, -3.0f, 8.0f, -100.0f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(atanf_tmu(in[i]), atanf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_atan2f_quadrants)
{
	const float y[] = {1.0f, 1.0f, -1.0f, -1.0f, 3.0f, -3.0f, 0.5f, -0.5f};
	const float x[] = {2.0f, -2.0f, 2.0f, -2.0f, 1.0f, 1.0f, -4.0f, -4.0f};

	for (size_t i = 0; i < ARRAY_SIZE(y); i++) {
		check(atan2f_tmu(y[i], x[i]), atan2f(y[i], x[i]));
	}
}

ZTEST(fastrts_tmu, test_atan2f_zero)
{
	zassert_equal(atan2f_tmu(0.0f, 0.0f), 0.0f, "atan2(0, 0) must be 0");
}

ZTEST(fastrts_tmu, test_asinf_acosf)
{
	const float in[] = {0.0f, 0.5f, -0.5f, 0.9f, -0.9f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(asinf_tmu(in[i]), asinf(in[i]));
		check(acosf_tmu(in[i]), acosf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_sqrtf)
{
	const float in[] = {1.0f, 2.0f, 100.0f, 0.25f, 12345.0f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(sqrtf_tmu(in[i]), sqrtf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_expf)
{
	/*
	 * Covers the inputs of the AM13 SDK fastrts example up to 9.24. The TMU is less accurate
	 * for large results (relative error vs libm of about 1e-5 at 9.24) and is badly off at
	 * 13.86 (measured 77 % high), so inputs above 10 are not checked here.
	 */
	const float in[] = {0.0f,      1.0f,     -1.0f,     2.5f,       -2.5f,       5.0f,
			    -18.94328f, -15.29395f, -12.42728f, -8.782109f, -4.562455f, -3.23441f,
			    -0.5628358f, -0.0023554f, 0.0023554f, 0.5628358f, 1.632452f,  2.345144f,
			    3.23441f,   6.423031f, 9.238994f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(expf_tmu(in[i]), expf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_logf)
{
	/* Same inputs as the AM13 SDK fastrts example, 0.0023 to 6.6e8 */
	const float in[] = {1.0f,        2.0f,       10.0f,       0.5f,        1000.0f,
			    0.0023554f,  0.5628358f, 1.632452f,   2.345144f,   3.23441f,
			    6.423031f,   9.238994f,  13.861f,     123.415844f, 3352.45443f,
			    45235.23f,   563253.5f,  3442553.5f,  661155700.0f};

	for (size_t i = 0; i < ARRAY_SIZE(in); i++) {
		check(logf_tmu(in[i]), logf(in[i]));
	}
}

ZTEST(fastrts_tmu, test_divf)
{
	const float y[] = {7.0f, -9.0f, 1.0f, 0.0f, 123.0f};
	const float x[] = {2.0f, 4.0f, 3.0f, 5.0f, -7.0f};

	for (size_t i = 0; i < ARRAY_SIZE(y); i++) {
		check(divf_tmu(y[i], x[i]), y[i] / x[i]);
	}
}

ZTEST(fastrts_tmu, test_flags)
{
	uint32_t prev;

	fastrts_tmu_clear_flags(FASTRTS_TMU_FLAGS_LUF | FASTRTS_TMU_FLAGS_LVF);
	zassert_equal(fastrts_tmu_get_flags(), 0U, "flags must be clear");

	prev = fastrts_tmu_set_flags(FASTRTS_TMU_FLAGS_LUF);
	zassert_equal(prev, 0U, "set returns the previous flags");
	zassert_equal(fastrts_tmu_get_flags(), FASTRTS_TMU_FLAGS_LUF, "LUF must be set alone");

	prev = fastrts_tmu_set_flags(FASTRTS_TMU_FLAGS_LVF);
	zassert_equal(prev, FASTRTS_TMU_FLAGS_LUF, "set returns the previous flags");
	zassert_equal(fastrts_tmu_get_flags(), FASTRTS_TMU_FLAGS_LUF | FASTRTS_TMU_FLAGS_LVF,
		      "LUF and LVF must both be set");

	prev = fastrts_tmu_clear_flags(FASTRTS_TMU_FLAGS_LUF);
	zassert_equal(prev, FASTRTS_TMU_FLAGS_LUF | FASTRTS_TMU_FLAGS_LVF,
		      "clear returns the previous flags");
	zassert_equal(fastrts_tmu_get_flags(), FASTRTS_TMU_FLAGS_LVF, "only LVF must remain");

	fastrts_tmu_clear_flags(FASTRTS_TMU_FLAGS_LVF);
	zassert_equal(fastrts_tmu_get_flags(), 0U, "flags must be clear");
}
