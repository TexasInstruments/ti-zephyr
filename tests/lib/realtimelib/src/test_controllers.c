/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <dclink_ss.h>
#include <fwc.h>
#include <pid.h>
#include <pr.h>
#include <vib_comp.h>
#include <vs_freq.h>

#define TOL 1.0e-4f

/* pid: parallel form, Up = Kp e, Ui += Ki e (clamped), Ud = filtered Kd e */
ZTEST(realtimelib, test_pid_proportional_integral_and_limits)
{
	PID_Obj obj;
	PID_Handle pid = PID_init(&obj, sizeof(obj));
	float out;

	zassert_not_null(pid, "PID_init");
	zassert_is_null(PID_init(&obj, 1), "a too small buffer must be rejected");

	PID_setGains(pid, 2.0f, 0.1f, 0.0f);
	PID_setMinMax(pid, -1.0f, 1.0f);
	PID_setUi(pid, 0.0f);
	/* Derivative filter with zero gain: the derivative path outputs nothing */
	PID_setDerFilterParams(pid, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	/* error = 0.2: Up = 0.4, Ui = 0.02 */
	PID_run_parallel(pid, 0.3f, 0.1f, 0.0f, &out);
	zassert_within(out, 0.42f, TOL, "first run %f", (double)out);
	PID_run_parallel(pid, 0.3f, 0.1f, 0.0f, &out);
	zassert_within(out, 0.44f, TOL, "integrator adds Ki*e each run %f", (double)out);

	/* The feedforward adds to the output */
	PID_setUi(pid, 0.0f);
	PID_run_parallel(pid, 0.0f, 0.0f, 0.25f, &out);
	zassert_within(out, 0.25f, TOL, "feedforward %f", (double)out);

	/* The output and the integrator are limited */
	for (int i = 0; i < 100; i++) {
		PID_run_parallel(pid, 5.0f, 0.0f, 0.0f, &out);
	}
	zassert_within(out, 1.0f, TOL, "upper limit %f", (double)out);
	PID_run_parallel(pid, -5.0f, 0.0f, 0.0f, &out);
	zassert_true(out >= -1.0f - TOL, "never below the lower limit %f", (double)out);
}

/* pr: y = (kr (e - e[n-2]) + c1 y[n-1] + c2 y[n-2]) / den, clamped */
ZTEST(realtimelib, test_pr_difference_equation_and_limits)
{
	PR_Obj obj = {0};
	PR_Handle pr = PR_init(&obj, sizeof(obj));
	const float inv_den = 0.5f, c1 = 1.0f, c2 = -0.5f;
	float prev = 0.0f, prev2 = 0.0f, err1 = 0.0f, err2 = 0.0f;
	const float feedback[] = {-1.0f, -0.5f, 0.25f, 0.75f, -0.1f};

	zassert_not_null(pr, "PR_init");
	zassert_is_null(PR_init(&obj, 1), "a too small buffer must be rejected");

	obj.kr = 2.0f;
	obj.outMin = -10.0f;
	obj.outMax = 10.0f;

	for (size_t i = 0; i < ARRAY_SIZE(feedback); i++) {
		float error = -feedback[i];
		float want = (obj.kr * (error - err2) + prev * c1 + prev2 * c2) * inv_den;

		obj.Feedback = feedback[i];
		PR_run(pr, inv_den, c1, c2);
		zassert_within(obj.out, want, TOL, "sample %u: got %f, want %f", (unsigned int)i,
			       (double)obj.out, (double)want);

		prev2 = prev;
		prev = want;
		err2 = err1;
		err1 = error;
	}

	/* The output is limited */
	obj.outMax = 0.1f;
	obj.outMin = -0.1f;
	obj.Feedback = -100.0f;
	PR_run(pr, inv_den, c1, c2);
	zassert_within(obj.out, 0.1f, TOL, "upper limit %f", (double)obj.out);
}

/* vs_freq: output voltage follows a V/f line between a boost and a rated point */
ZTEST(realtimelib, test_vs_freq_profile)
{
	VS_FREQ_Obj obj = {0};
	VS_FREQ_Handle vf = VS_FREQ_init(&obj, sizeof(obj));

	zassert_not_null(vf, "VS_FREQ_init");
	obj.maxVsMag_pu = 0.9f;
	VS_FREQ_setProfile(vf, 0.1f, 0.5f, 0.05f, 0.45f);

	/* Slope: (0.45 - 0.05) / (0.5 - 0.1) = 1.0 */
	zassert_within(obj.VfSlope, 1.0f, TOL, "slope %f", (double)obj.VfSlope);

	VS_FREQ_run(vf, 0.0f);
	zassert_within(obj.Vs_out, 0.05f, TOL, "below the low point: boost voltage");
	VS_FREQ_run(vf, 0.1f);
	zassert_within(obj.Vs_out, 0.05f, TOL, "at the low point");
	VS_FREQ_run(vf, 0.3f);
	zassert_within(obj.Vs_out, 0.25f, TOL, "on the line %f", (double)obj.Vs_out);
	VS_FREQ_run(vf, 0.5f);
	zassert_within(obj.Vs_out, 0.45f, TOL, "at the rated point");
	VS_FREQ_run(vf, 0.9f);
	zassert_within(obj.Vs_out, 0.45f, TOL, "above the rated point: held");
}

ZTEST(realtimelib, test_vs_freq_direction_and_dq_split)
{
	VS_FREQ_Obj obj = {0};
	VS_FREQ_Handle vf = VS_FREQ_init(&obj, sizeof(obj));
	float vd, vq_fwd, vq_rev;

	obj.maxVsMag_pu = 0.9f;
	VS_FREQ_setProfile(vf, 0.1f, 0.5f, 0.05f, 0.45f);

	VS_FREQ_run(vf, 0.3f);
	vd = VS_FREQ_getVd_out(vf);
	vq_fwd = VS_FREQ_getVq_out(vf);
	VS_FREQ_run(vf, -0.3f);
	vq_rev = VS_FREQ_getVq_out(vf);

	/* The d axis is a fixed 0.3 share of the magnitude */
	zassert_within(vd, 0.25f * 0.3f, TOL, "d share %f", (double)vd);
	zassert_within(VS_FREQ_getVd_out(vf), vd, TOL, "d is unchanged");

	/*
	 * The magnitude depends on |frequency|. VS_FREQ_run() takes fabsf() of the input and
	 * then tests Freq > 0 to pick the q sign, so the negative branch is only reachable at
	 * exactly 0 Hz and a negative frequency gives the same Vq as a positive one. This is
	 * how the module behaves as delivered, and it is checked here so a change is noticed.
	 */
	zassert_within(vq_rev, vq_fwd, TOL, "q is the same for either direction");

	VS_FREQ_run(vf, 0.0f);
	zassert_true(VS_FREQ_getVq_out(vf) <= 0.0f, "q sign flips only at zero frequency");
}

/* fwc: no run function, only parameter storage around a PI controller */
ZTEST(realtimelib, test_fwc_params_and_enable)
{
	FWC_Obj obj;
	FWC_Handle fwc = FWC_init(&obj, sizeof(obj));

	zassert_not_null(fwc, "FWC_init");
	zassert_is_null(FWC_init(&obj, 1), "a too small buffer must be rejected");

	FWC_setParams(fwc, 1.5f, 0.25f, 0.1f, 1.2f);
	zassert_within(obj.pi.Kp, 1.5f, TOL, "Kp");
	zassert_within(obj.pi.Ki, 0.25f, TOL, "Ki");
	zassert_within(obj.pi.Ui, 0.0f, TOL, "integrator starts at zero");

	FWC_setCurrentAngle_rad(fwc, 0.6f);
	zassert_within(FWC_getCurrentAngle_rad(fwc), 0.6f, TOL, "angle");

	FWC_setFlagEnable(fwc, true);
	zassert_true(FWC_getFlagEnable(fwc), "enabled");
	FWC_disable(fwc);
	zassert_false(FWC_getFlagEnable(fwc), "disabled");
	FWC_enable(fwc);
	zassert_true(FWC_getFlagEnable(fwc), "enabled again");
}

/* vib_comp: state only, the learning table is not checked */
ZTEST(realtimelib, test_vib_comp_init_and_disabled_output)
{
	/* The object holds a 361 entry table, too big for the test stack */
	static VIB_COMP_Obj obj;
	VIB_COMP_Handle vc = VIB_COMP_init(&obj, sizeof(obj));
	float out;

	zassert_not_null(vc, "VIB_COMP_init");
	zassert_is_null(VIB_COMP_init(&obj, 1), "a too small buffer must be rejected");

	VIB_COMP_reset(vc);
	VIB_COMP_setFlag_enableOutput(vc, false);
	VIB_COMP_setAlphaGain(vc, 0.9f, 0.1f);

	/* With the output disabled the compensation current is zero */
	out = VIB_COMP_run(vc, 0.5f, 1.0f);
	(void)out;
	zassert_within(obj.Iq_outFF_A, 0.0f, TOL, "no feedforward while disabled");
}

/* dclink_ss: state only, the PWM timing algorithms are not checked */
ZTEST(realtimelib, test_dclink_ss_initial_conditions)
{
	DCLINK_SS_Obj obj;
	DCLINK_SS_Handle ss = DCLINK_SS_init(&obj, sizeof(obj));

	zassert_not_null(ss, "DCLINK_SS_init");
	zassert_is_null(DCLINK_SS_init(&obj, 1), "a too small buffer must be rejected");

	DCLINK_SS_setInitialConditions(ss, 2000, 0.75f);
	zassert_equal(obj.pwmPeriod, 2000, "PWM period");
	zassert_within(obj.SSTOffThrVs_pu, 0.75f, TOL, "threshold");
	zassert_equal(obj.minAvDuration, 90, "default minimum AV duration");
	zassert_equal(obj.sampleDelay, 50, "default sample delay");
	zassert_within(obj.I_A.value[0], 0.0f, TOL, "currents start at zero");

	DCLINK_SS_setMinAVDuration(ss, 120);
	DCLINK_SS_setSampleDelay(ss, 70);
	zassert_equal(obj.minAvDuration, 120, "minimum AV duration");
	zassert_equal(obj.sampleDelay, 70, "sample delay");
}
