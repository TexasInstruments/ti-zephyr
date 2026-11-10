/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <mtpa.h>

#define TOL 1.0e-4f

#define LS_D_H (CONFIG_TI_REALTIMELIB_MTPA_LS_D_UH * 1.0e-6f)
#define LS_Q_H (CONFIG_TI_REALTIMELIB_MTPA_LS_Q_UH * 1.0e-6f)

/* kconst = 0.25 * flux / (Lq - Ld), or 0 for a non-salient motor */
ZTEST(realtimelib, test_mtpa_constant)
{
	MTPA_Obj obj = {0};
	MTPA_Handle h = MTPA_init(&obj, sizeof(obj));

	zassert_not_null(h, "MTPA_init");

	MTPA_computeParameters(h, 1.0e-3f, 2.0e-3f, 0.04f);
	zassert_within(MTPA_getKconst(h), 10.0f, TOL, "kconst %f", (double)MTPA_getKconst(h));

	MTPA_computeParameters(h, 2.0e-3f, 1.0e-3f, 0.04f);
	zassert_within(MTPA_getKconst(h), -10.0f, TOL, "Lq < Ld gives a negative constant");

	MTPA_computeParameters(h, 1.5e-3f, 1.5e-3f, 0.04f);
	zassert_within(MTPA_getKconst(h), 0.0f, TOL, "no saliency, no constant");
}

/* Id = k - sqrt(k^2 + Is^2 / 2), Iq = sqrt(Is^2 - Id^2) */
ZTEST(realtimelib, test_mtpa_current_reference)
{
	MTPA_Obj obj = {0};
	MTPA_Handle h = MTPA_init(&obj, sizeof(obj));
	const float k = 10.0f, is = 5.0f;
	const float id = k - sqrtf(k * k + 0.5f * is * is);

	MTPA_computeParameters(h, 1.0e-3f, 2.0e-3f, 0.04f);
	MTPA_enable(h);

	MTPA_computeCurrentReference(h, is);
	zassert_within(MTPA_getId_ref_A(h), id, TOL, "Id %f", (double)MTPA_getId_ref_A(h));
	zassert_true(MTPA_getId_ref_A(h) < 0.0f, "Id is negative for Lq > Ld");
	zassert_within(MTPA_getIq_ref_A(h), sqrtf(is * is - id * id), TOL, "Iq");
	zassert_within(MTPA_getIs_ref_A(h), is, TOL, "magnitude");

	/* The d and q references together keep the requested magnitude */
	zassert_within(MTPA_getId_ref_A(h) * MTPA_getId_ref_A(h) +
			       MTPA_getIq_ref_A(h) * MTPA_getIq_ref_A(h),
		       is * is, 1.0e-3f, "|Idq| is Is");

	/* A negative request flips Iq only */
	MTPA_computeCurrentReference(h, -is);
	zassert_within(MTPA_getId_ref_A(h), id, TOL, "Id unchanged for a negative request");
	zassert_true(MTPA_getIq_ref_A(h) < 0.0f, "Iq follows the sign");
	zassert_within(MTPA_getIs_ref_A(h), is, TOL, "magnitude is absolute");
}

ZTEST(realtimelib, test_mtpa_disabled_or_degenerate)
{
	MTPA_Obj obj = {0};
	MTPA_Handle h = MTPA_init(&obj, sizeof(obj));

	MTPA_computeParameters(h, 1.0e-3f, 2.0e-3f, 0.04f);

	/* Disabled: all of the request goes to the q axis, the angle is 90 degrees */
	MTPA_disable(h);
	MTPA_computeCurrentReference(h, 3.0f);
	zassert_within(MTPA_getId_ref_A(h), 0.0f, TOL, "Id");
	zassert_within(MTPA_getIq_ref_A(h), 3.0f, TOL, "Iq");
	zassert_within(MTPA_computeCurrentAngle(h, 3.0f), MATH_PI_OVER_TWO, TOL, "angle");

	/* Enabled, but zero current: same result */
	MTPA_enable(h);
	zassert_true(MTPA_getFlagEnable(h), "enabled");
	MTPA_computeCurrentReference(h, 0.0f);
	zassert_within(MTPA_getIq_ref_A(h), 0.0f, TOL, "Iq at zero current");
	zassert_within(MTPA_computeCurrentAngle(h, 0.0f), MATH_PI_OVER_TWO, TOL, "angle at zero");

	/* Enabled, but no saliency: same result */
	MTPA_computeParameters(h, 1.0e-3f, 1.0e-3f, 0.04f);
	MTPA_computeCurrentReference(h, 3.0f);
	zassert_within(MTPA_getId_ref_A(h), 0.0f, TOL, "Id without saliency");
	zassert_within(MTPA_getIq_ref_A(h), 3.0f, TOL, "Iq without saliency");
}

/* angle = acos(g - sqrt(g^2 + 1/2)) with g = k / Is */
ZTEST(realtimelib, test_mtpa_current_angle)
{
	MTPA_Obj obj = {0};
	MTPA_Handle h = MTPA_init(&obj, sizeof(obj));
	const float k = 10.0f, is = 5.0f;
	const float g = k / is;
	const float want = acosf(g - sqrtf(g * g + 0.5f));

	MTPA_computeParameters(h, 1.0e-3f, 2.0e-3f, 0.04f);
	MTPA_enable(h);

	zassert_within(MTPA_computeCurrentAngle(h, is), want, TOL, "angle %f", (double)want);
	zassert_within(MTPA_getCurrentAngle_rad(h), want, TOL, "getter");
	zassert_true(want > MATH_PI_OVER_TWO, "past 90 degrees, so Id < 0");

	/* The angle is consistent with the current reference: tan(angle) = Iq / Id */
	MTPA_computeCurrentReference(h, is);
	zassert_within(cosf(want) * is, MTPA_getId_ref_A(h), 1.0e-3f, "Id = Is cos(angle)");
}

ZTEST(realtimelib, test_mtpa_init_rejects_small_buffer)
{
	MTPA_Obj obj;

	zassert_is_null(MTPA_init(&obj, 1), "a too small buffer must be rejected");
}

/* The tables are the motor inductances scaled by a current dependent factor */
ZTEST(realtimelib, test_mtpa_tables_and_lookup)
{
	MTPA_Obj obj = {0};
	MTPA_Handle h = MTPA_init(&obj, sizeof(obj));
	const float tol = 1.0e-9f;

	zassert_within(MTPA_Ld_tableData_H[0], LS_D_H * 1.025f, tol, "Ld[0]");
	zassert_within(MTPA_Ld_tableData_H[5], LS_D_H, tol, "Ld[5]");
	zassert_within(MTPA_Ld_tableData_H[20], LS_D_H * 0.925f, tol, "Ld[20]");
	zassert_within(MTPA_Lq_tableData_H[0], LS_Q_H * 1.050f, tol, "Lq[0]");
	zassert_within(MTPA_Lq_tableData_H[5], LS_Q_H, tol, "Lq[5]");
	zassert_within(MTPA_Lq_tableData_H[20], LS_Q_H * 0.850f, tol, "Lq[20]");

	obj.deltaIs_Ld_A = 0.5f;
	obj.deltaIs_Lq_A = 0.5f;
	obj.indexMax_Ld = MTPA_LUT_INDEX_LD_MAX;
	obj.indexMax_Lq = MTPA_LUT_INDEX_LQ_MAX;

	zassert_within(MTPA_updateLs_d_withLUT(h, 2.5f), LS_D_H, tol, "d at 2.5 A");
	zassert_within(MTPA_updateLs_d_withLUT(h, 100.0f), LS_D_H * 0.925f, tol, "d clamps at the end");

	/* The q lookup must use the q table, which differs from the d table at every index */
	zassert_within(MTPA_updateLs_q_withLUT(h, 0.0f), LS_Q_H * 1.050f, tol, "q at 0 A");
	zassert_within(MTPA_updateLs_q_withLUT(h, 2.5f), LS_Q_H, tol, "q at 2.5 A");
	zassert_within(MTPA_updateLs_q_withLUT(h, 100.0f), LS_Q_H * 0.850f, tol, "q clamps at the end");
}
