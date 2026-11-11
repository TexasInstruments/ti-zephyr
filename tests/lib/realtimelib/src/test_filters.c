/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <filter_notch.h>
#include <filter_so.h>
#include <offset.h>
#include <volt_calc.h>
#include <volt_recons.h>

#define TOL 1.0e-4f

/* filter_so: y[n] = b0 x[n] + b1 x[n-1] + b2 x[n-2] - a1 y[n-1] - a2 y[n-2] */
ZTEST(realtimelib, test_filter_so_step_response)
{
	FILTER_SO_Obj obj;
	FILTER_SO_Handle f = FILTER_SO_init(&obj, sizeof(obj));
	float y;

	zassert_not_null(f, "FILTER_SO_init");

	/* A pure 3-tap average: unity DC gain, no feedback */
	FILTER_SO_set_a1(f, 0.0f);
	FILTER_SO_set_a2(f, 0.0f);
	FILTER_SO_set_b0(f, 1.0f / 3.0f);
	FILTER_SO_set_b1(f, 1.0f / 3.0f);
	FILTER_SO_set_b2(f, 1.0f / 3.0f);
	FILTER_SO_set_x1(f, 0.0f);
	FILTER_SO_set_x2(f, 0.0f);
	FILTER_SO_set_y1(f, 0.0f);
	FILTER_SO_set_y2(f, 0.0f);

	y = FILTER_SO_run(f, 3.0f);
	zassert_within(y, 1.0f, TOL, "first sample %f", (double)y);
	y = FILTER_SO_run(f, 3.0f);
	zassert_within(y, 2.0f, TOL, "second sample %f", (double)y);
	y = FILTER_SO_run(f, 3.0f);
	zassert_within(y, 3.0f, TOL, "third sample %f", (double)y);
	y = FILTER_SO_run(f, 3.0f);
	zassert_within(y, 3.0f, TOL, "holds the DC level %f", (double)y);
}

ZTEST(realtimelib, test_filter_so_feedback)
{
	FILTER_SO_Obj obj;
	FILTER_SO_Handle f = FILTER_SO_init(&obj, sizeof(obj));
	float y = 0.0f;

	/* y[n] = x[n] + 0.5 y[n-1]: an impulse decays by half each sample */
	FILTER_SO_set_a1(f, -0.5f);
	FILTER_SO_set_a2(f, 0.0f);
	FILTER_SO_set_b0(f, 1.0f);
	FILTER_SO_set_b1(f, 0.0f);
	FILTER_SO_set_b2(f, 0.0f);
	FILTER_SO_set_x1(f, 0.0f);
	FILTER_SO_set_x2(f, 0.0f);
	FILTER_SO_set_y1(f, 0.0f);
	FILTER_SO_set_y2(f, 0.0f);

	y = FILTER_SO_run(f, 1.0f);
	zassert_within(y, 1.0f, TOL, "impulse %f", (double)y);
	y = FILTER_SO_run(f, 0.0f);
	zassert_within(y, 0.5f, TOL, "decay 1 %f", (double)y);
	y = FILTER_SO_run(f, 0.0f);
	zassert_within(y, 0.25f, TOL, "decay 2 %f", (double)y);
}

ZTEST(realtimelib, test_filter_so_second_order_feedback)
{
	FILTER_SO_Obj obj;
	FILTER_SO_Handle f = FILTER_SO_init(&obj, sizeof(obj));
	float y;

	/* y[n] = x[n] + 0.5 y[n-1] - 0.25 y[n-2], so an impulse gives 1, 0.5, 0, -0.125 */
	FILTER_SO_set_a1(f, -0.5f);
	FILTER_SO_set_a2(f, 0.25f);
	FILTER_SO_set_b0(f, 1.0f);
	FILTER_SO_set_b1(f, 0.0f);
	FILTER_SO_set_b2(f, 0.0f);
	FILTER_SO_set_x1(f, 0.0f);
	FILTER_SO_set_x2(f, 0.0f);
	FILTER_SO_set_y1(f, 0.0f);
	FILTER_SO_set_y2(f, 0.0f);

	y = FILTER_SO_run(f, 1.0f);
	zassert_within(y, 1.0f, TOL, "n=0 %f", (double)y);
	y = FILTER_SO_run(f, 0.0f);
	zassert_within(y, 0.5f, TOL, "n=1 %f", (double)y);
	y = FILTER_SO_run(f, 0.0f);
	zassert_within(y, 0.0f, TOL, "n=2 %f", (double)y);
	y = FILTER_SO_run(f, 0.0f);
	zassert_within(y, -0.125f, TOL, "n=3 %f", (double)y);
}

ZTEST(realtimelib, test_filter_so_init_rejects_small_buffer)
{
	FILTER_SO_Obj obj;

	zassert_is_null(FILTER_SO_init(&obj, 1), "a too small buffer must be rejected");
}

/* notch: coefficients from FILTER_COEFF_update, then a difference equation */
ZTEST(realtimelib, test_notch_coefficients)
{
	FILTER_NOTCH_CoeffObj cobj;
	FILTER_NOTCH_Coeff_Handle coeff = FILTER_COEFF_init(&cobj, sizeof(cobj));
	const float ts = 1.0e-4f, freq = 50.0f, c2 = 0.1f, c1 = 1.0f;
	const float wn = 2.0f * MATH_TWO_PI * freq;
	const float x = 2.0f * c2 * wn * ts;
	const float y = 2.0f * c1 * wn * ts;
	const float z = wn * ts * wn * ts;

	zassert_not_null(coeff, "FILTER_COEFF_init");
	FILTER_COEFF_update(coeff, ts, freq, c2, c1);

	zassert_within(cobj.b0, 1.0f, TOL, "b0");
	zassert_within(cobj.b1, x - 2.0f, TOL, "b1 %f", (double)cobj.b1);
	zassert_within(cobj.b2, z - x + 1.0f, TOL, "b2 %f", (double)cobj.b2);
	zassert_within(cobj.a1, -(y - 2.0f), TOL, "a1 %f", (double)cobj.a1);
	zassert_within(cobj.a2, -(z - y + 1.0f), TOL, "a2 %f", (double)cobj.a2);
}

ZTEST(realtimelib, test_notch_run_matches_difference_equation)
{
	FILTER_NOTCH_CoeffObj cobj;
	FILTER_NOTCH_Obj fobj;
	FILTER_NOTCH_Coeff_Handle coeff = FILTER_COEFF_init(&cobj, sizeof(cobj));
	FILTER_NOTCH_Handle notch = FILTER_NOTCH_init(&fobj, sizeof(fobj));
	const float input[] = {1.0f, 0.5f, -0.25f, 0.8f, 0.0f, -1.0f};
	float in1 = 0.0f, in2 = 0.0f, out1 = 0.0f, out2 = 0.0f;

	FILTER_COEFF_update(coeff, 1.0e-4f, 50.0f, 0.1f, 1.0f);
	FILTER_NOTCH_reset(notch);

	for (size_t i = 0; i < ARRAY_SIZE(input); i++) {
		float want = cobj.a1 * out1 + cobj.a2 * out2 + cobj.b0 * input[i] +
			     cobj.b1 * in1 + cobj.b2 * in2;
		float got;

		FILTER_NOTCH_set_in(notch, input[i]);
		got = FILTER_NOTCH_run(notch, coeff);
		zassert_within(got, want, TOL, "sample %u: got %f, want %f", (unsigned int)i,
			       (double)got, (double)want);

		in2 = in1;
		in1 = input[i];
		out2 = out1;
		out1 = want;
	}
}

/* offset: a low-pass that tracks a slowly varying offset, unity DC gain */
ZTEST(realtimelib, test_offset_settles_to_constant_input)
{
	OFFSET_Obj obj;
	OFFSET_Handle off = OFFSET_init(&obj, sizeof(obj));

	zassert_not_null(off, "OFFSET_init");
	OFFSET_setInitCond(off, 0.0f);
	OFFSET_setBeta(off, 0.5f);

	for (int i = 0; i < 400; i++) {
		OFFSET_run(off, 2.0f);
	}
	zassert_within(obj.value, 2.0f, 1.0e-3f, "offset %f", (double)obj.value);
}

ZTEST(realtimelib, test_offset_init_condition_and_override)
{
	OFFSET_Obj obj;
	OFFSET_Handle off = OFFSET_init(&obj, sizeof(obj));

	OFFSET_setBeta(off, 0.5f);
	OFFSET_setInitCond(off, 1.5f);
	zassert_within(obj.value, 1.5f, TOL, "initial condition");

	/* Held at the initial condition, a matching input does not move it */
	OFFSET_run(off, 1.5f);
	zassert_within(obj.value, 1.5f, TOL, "stays put %f", (double)obj.value);

	OFFSET_setOffset(off, -0.75f);
	zassert_within(obj.value, -0.75f, TOL, "forced value");
}

/* volts: phase voltages from the duty cycles, Vphase_a = Vdc/3 * (2a - b - c) */
ZTEST(realtimelib, test_phvolc_phase_voltages)
{
	PHVOLC_Obj obj = {0};
	PHVOLC_Handle h = (PHVOLC_Handle)&obj;
	MATH_Vec3 duty = {{0.8f, 0.2f, 0.5f}};
	MATH_Vec2 unused;
	const float vdc = 24.0f;

	/* The results are stored in the object (the Vab output argument is not written) */
	PHVOLC_run(h, vdc, &duty, &unused);

	zassert_within(obj.VphaseA, vdc / 3.0f * (2.0f * 0.8f - 0.2f - 0.5f), TOL, "A %f",
		       (double)obj.VphaseA);
	zassert_within(obj.VphaseB, vdc / 3.0f * (2.0f * 0.2f - 0.8f - 0.5f), TOL, "B %f",
		       (double)obj.VphaseB);
	zassert_within(obj.VphaseC, vdc / 3.0f * (2.0f * 0.5f - 0.2f - 0.8f), TOL, "C %f",
		       (double)obj.VphaseC);

	/* The three phase voltages of a balanced star load sum to zero */
	zassert_within(obj.VphaseA + obj.VphaseB + obj.VphaseC, 0.0f, TOL, "sum");
	zassert_within(obj.Valpha, obj.VphaseA, TOL, "alpha");
	zassert_within(obj.Vbeta, (obj.VphaseA + 2.0f * obj.VphaseB) / 1.7320508f, TOL, "beta");
}

ZTEST(realtimelib, test_phvolc_out_of_phase_flips_sign)
{
	PHVOLC_Obj obj = {0};
	PHVOLC_Handle h = (PHVOLC_Handle)&obj;
	MATH_Vec3 duty = {{0.8f, 0.2f, 0.5f}};
	MATH_Vec2 unused;
	float alpha, beta;

	PHVOLC_run(h, 24.0f, &duty, &unused);
	alpha = obj.Valpha;
	beta = obj.Vbeta;
	obj.flagOutOfPhase = 1;
	PHVOLC_run(h, 24.0f, &duty, &unused);

	zassert_within(obj.Valpha, -alpha, TOL, "alpha flipped");
	zassert_within(obj.Vbeta, -beta, TOL, "beta flipped");
}

ZTEST(realtimelib, test_volrec_init_reset_and_params)
{
	VOLREC_Obj obj;
	VOLREC_Handle h = VOLREC_init(&obj, sizeof(obj));

	zassert_not_null(h, "VOLREC_init");
	zassert_is_null(VOLREC_init(&obj, 1), "a too small buffer must be rejected");

	VOLREC_setParams(h, 1000.0f, 10000.0f);
	/* Low-pass coefficients: beta = pole / rate, a1 = (b-2)/(b+2), b0 = b1 = b/(b+2) */
	zassert_within(obj.a1, (0.1f - 2.0f) / (0.1f + 2.0f), TOL, "a1 %f", (double)obj.a1);
	zassert_within(obj.b0, 0.1f / (0.1f + 2.0f), TOL, "b0 %f", (double)obj.b0);
	zassert_within(obj.b1, obj.b0, TOL, "b1");
	zassert_within(obj.sf, 0.925f, TOL, "default scale factor %f", (double)obj.sf);

	obj.sf = 0.9f;
	obj.numSamples = 7;
	VOLREC_reset(h);
	zassert_within(obj.sf, 1.0f, TOL, "reset restores the scale factor");
	zassert_equal(obj.numSamples, 0, "reset clears the sample count");
}
