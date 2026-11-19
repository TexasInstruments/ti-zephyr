/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <clarke.h>
#include <filter_fo.h>
#include <ipark.h>
#include <park.h>
#include <pi.h>
#include <svgen.h>

#define TOL 1.0e-4f

ZTEST(realtimelib, test_clarke_balanced)
{
	const float theta[] = {0.0f, 0.7f, 1.9f, -2.4f, 3.0f};

	for (size_t i = 0; i < ARRAY_SIZE(theta); i++) {
		float ia = cosf(theta[i]);
		float ib = cosf(theta[i] - 2.0f * MATH_PI / 3.0f);
		float ic = cosf(theta[i] + 2.0f * MATH_PI / 3.0f);
		float alpha, beta;

		CLARKE_run_threeInput(ia, ib, ic, &alpha, &beta);
		zassert_within(alpha, ia, TOL, "alpha %f for theta %f", (double)alpha,
			       (double)theta[i]);
		zassert_within(beta, sinf(theta[i]), TOL, "beta %f for theta %f", (double)beta,
			       (double)theta[i]);
	}
}

ZTEST(realtimelib, test_clarke_two_input_matches_three)
{
	float ia = 0.8f, ib = -0.3f, ic = -(0.8f - 0.3f);
	float a3, b3, a2, b2;

	CLARKE_run_threeInput(ia, ib, ic, &a3, &b3);
	CLARKE_run_twoInput(ia, ib, &a2, &b2);
	zassert_within(a2, a3, TOL, "alpha %f vs %f", (double)a2, (double)a3);
	zassert_within(b2, b3, TOL, "beta %f vs %f", (double)b2, (double)b3);
}

ZTEST(realtimelib, test_park_ipark_roundtrip)
{
	PARK_Obj park_obj;
	IPARK_Obj ipark_obj;
	PARK_Handle park = PARK_init(&park_obj, sizeof(park_obj));
	IPARK_Handle ipark = IPARK_init(&ipark_obj, sizeof(ipark_obj));
	const float theta[] = {0.0f, 0.5f, 1.5f, 3.0f, -1.0f, -3.0f};

	zassert_not_null(park, "PARK_init");
	zassert_not_null(ipark, "IPARK_init");

	for (size_t i = 0; i < ARRAY_SIZE(theta); i++) {
		MATH_Vec2 in = {{0.6f, -0.4f}};
		MATH_Vec2 dq, out;

		PARK_setup(park, theta[i]);
		IPARK_setup(ipark, theta[i]);
		PARK_run(park, &in, &dq);
		IPARK_run(ipark, &dq, &out);

		zassert_within(out.value[0], in.value[0], TOL, "alpha %f", (double)out.value[0]);
		zassert_within(out.value[1], in.value[1], TOL, "beta %f", (double)out.value[1]);
	}
}

ZTEST(realtimelib, test_park_aligned_axis)
{
	PARK_Obj obj;
	PARK_Handle park = PARK_init(&obj, sizeof(obj));
	MATH_Vec2 in, dq;

	/* A vector on the rotor angle is purely d-axis */
	PARK_setup(park, 0.9f);
	in.value[0] = cosf(0.9f);
	in.value[1] = sinf(0.9f);
	PARK_run(park, &in, &dq);
	zassert_within(dq.value[0], 1.0f, TOL, "d %f", (double)dq.value[0]);
	zassert_within(dq.value[1], 0.0f, TOL, "q %f", (double)dq.value[1]);
}

ZTEST(realtimelib, test_park_init_rejects_small_buffer)
{
	PARK_Obj obj;

	zassert_is_null(PARK_init(&obj, 1), "a too small buffer must be rejected");
}

ZTEST(realtimelib, test_svgen_common_mode)
{
	const float va_in[] = {0.0f, 0.3f, -0.4f, 0.2f, 0.5f};
	const float vb_in[] = {0.0f, 0.1f, 0.3f, -0.5f, 0.4f};

	for (size_t i = 0; i < ARRAY_SIZE(va_in); i++) {
		float a = va_in[i];
		float b = -0.5f * va_in[i] + 0.8660254f * vb_in[i];
		float c = -0.5f * va_in[i] - 0.8660254f * vb_in[i];
		float va, vb, vc, hi, lo;

		SVGEN_runCom(1.0f, va_in[i], vb_in[i], &va, &vb, &vc);

		/* Line-to-line voltages are unchanged by the common-mode term */
		zassert_within(va - vb, a - b, TOL, "a-b %f vs %f", (double)(va - vb),
			       (double)(a - b));
		zassert_within(vb - vc, b - c, TOL, "b-c %f vs %f", (double)(vb - vc),
			       (double)(b - c));

		/* The outputs are centered, so the largest and smallest cancel */
		hi = fmaxf(va, fmaxf(vb, vc));
		lo = fminf(va, fminf(vb, vc));
		zassert_within(hi + lo, 0.0f, TOL, "max %f min %f", (double)hi, (double)lo);
	}
}

ZTEST(realtimelib, test_svgen_scale_factor)
{
	float va1, vb1, vc1, va2, vb2, vc2;

	/* The first argument scales the inputs, here volts to per unit for a 24 V bus */
	SVGEN_runCom(1.0f, 0.5f, 0.2f, &va1, &vb1, &vc1);
	SVGEN_runCom(1.0f / 24.0f, 12.0f, 4.8f, &va2, &vb2, &vc2);
	zassert_within(va1, va2, TOL, "va %f vs %f", (double)va1, (double)va2);
	zassert_within(vb1, vb2, TOL, "vb %f vs %f", (double)vb1, (double)vb2);
	zassert_within(vc1, vc2, TOL, "vc %f vs %f", (double)vc1, (double)vc2);
}

ZTEST(realtimelib, test_pi_proportional_and_limits)
{
	PI_Obj obj;
	PI_Handle pi = PI_init(&obj, sizeof(obj));
	float out;

	zassert_not_null(pi, "PI_init");
	PI_setGains(pi, 2.0f, 0.0f);
	PI_setUi(pi, 0.0f);
	PI_setMinMax(pi, -1.0f, 1.0f);

	PI_run_parallel(pi, 0.3f, 0.1f, 0.0f, &out);
	zassert_within(out, 0.4f, TOL, "P only, out %f", (double)out);

	/* The output saturates at the configured limits */
	PI_run_parallel(pi, 5.0f, 0.0f, 0.0f, &out);
	zassert_within(out, 1.0f, TOL, "upper limit, out %f", (double)out);
	PI_run_parallel(pi, -5.0f, 0.0f, 0.0f, &out);
	zassert_within(out, -1.0f, TOL, "lower limit, out %f", (double)out);
}

ZTEST(realtimelib, test_pi_integrator_accumulates_and_saturates)
{
	PI_Obj obj;
	PI_Handle pi = PI_init(&obj, sizeof(obj));
	float out = 0.0f;

	PI_setGains(pi, 0.0f, 0.1f);
	PI_setUi(pi, 0.0f);
	PI_setMinMax(pi, -0.5f, 0.5f);

	for (int i = 0; i < 3; i++) {
		PI_run_parallel(pi, 1.0f, 0.0f, 0.0f, &out);
	}
	zassert_within(out, 0.3f, TOL, "3 steps of Ki*err, out %f", (double)out);

	for (int i = 0; i < 50; i++) {
		PI_run_parallel(pi, 1.0f, 0.0f, 0.0f, &out);
	}
	zassert_within(out, 0.5f, TOL, "anti-windup clamp, out %f", (double)out);
	zassert_within(PI_getUi(pi), 0.5f, TOL, "integrator clamped, Ui %f", (double)PI_getUi(pi));
}

ZTEST(realtimelib, test_filter_fo_step_response)
{
	FILTER_FO_Obj obj;
	FILTER_FO_Handle f = FILTER_FO_init(&obj, sizeof(obj));
	float y = 0.0f;

	zassert_not_null(f, "FILTER_FO_init");

	/* y[n] = 0.1 x[n] + 0.9 y[n-1]: unity DC gain, exponential rise */
	FILTER_FO_set_a1(f, -0.9f);
	FILTER_FO_set_b0(f, 0.1f);
	FILTER_FO_set_b1(f, 0.0f);
	FILTER_FO_set_x1(f, 0.0f);
	FILTER_FO_set_y1(f, 0.0f);

	y = FILTER_FO_run(f, 1.0f);
	zassert_within(y, 0.1f, TOL, "first sample %f", (double)y);
	y = FILTER_FO_run(f, 1.0f);
	zassert_within(y, 0.19f, TOL, "second sample %f", (double)y);

	for (int i = 0; i < 200; i++) {
		y = FILTER_FO_run(f, 1.0f);
	}
	zassert_within(y, 1.0f, 1.0e-3f, "settles to the DC input, y %f", (double)y);
}
