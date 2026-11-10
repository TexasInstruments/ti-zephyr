/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <dead_time_comp.h>
#include <est.h>
#include <esmo.h>
#include <flux_obs.h>
#include <speedfr.h>
#include <ti_tune_rs.h>
#include <vsf.h>

#define TOL 1.0e-4f

static USER_Params up;

static void fill_user_params(void)
{
	memset(&up, 0, sizeof(up));
	up.systemFreq_MHz = 200.0f;
	up.maxFrequency_Hz = 200.0f;
	up.voltage_sf = 24.0f;
	up.current_sf = 10.0f;
	up.ctrlPeriod_sec = 50.0e-6f;
	up.motor_Rs_Ohm = 0.5f;
	up.motor_Ls_d_H = 1.0e-3f;
	up.motor_Ls_q_H = 1.25e-3f;
	up.motor_ratedFlux_Wb = 0.01f;
	up.motor_numPolePairs = 4;
	up.numIsrTicksPerCtrlTick = 1;
	up.numIsrTicksPerEstTick = 1;
	up.numIsrTicksPerTrajTick = 1;
	up.numCtrlTicksPerCurrentTick = 1;
	up.numCtrlTicksPerSpeedTick = 10;
}

/* vsf: steps the PWM frequency towards the allowed range, one delta per call */
ZTEST(realtimelib, test_vsf_init_and_frequency_step)
{
	VSF_Obj obj;
	VSF_Handle vsf = VSF_init(&obj, sizeof(obj));

	zassert_not_null(vsf, "VSF_init");
	zassert_is_null(VSF_init(&obj, 1), "a too small buffer must be rejected");

	fill_user_params();
	VSF_initParams(vsf, &up);
	zassert_equal(obj.cpuFreq_Hz, 200000000U, "cpu clock");
	zassert_equal(obj.pwmFreqSet_Hz, CONFIG_TI_REALTIMELIB_VSF_PWM_FREQ_KHZ * 1000,
		      "start frequency from Kconfig");
	/* The counter is up-down, so the period is half the clock over the PWM frequency */
	zassert_equal(obj.pwmPeriod, (200000000U / 2U) / obj.pwmFreqNow_Hz, "period %u",
		      obj.pwmPeriod);
	zassert_equal(obj.state, VSF_STATE_IDLE, "idle");

	/* 50 kHz is above the 25 kHz limit: the target is clamped and one step is taken */
	VSF_computeFreqParams(vsf);
	zassert_equal(obj.pwmFreqTarget_Hz, VSF_NUM_MAX_FREQ_HZ, "target clamped to max");
	zassert_equal(obj.pwmFreqNow_Hz, 50000 - VSF_NUM_DELTA, "one step down");
	zassert_equal(obj.state, VSF_STATE_PERIOD_SET, "period update pending");

	/* While the update is pending a second call changes nothing */
	VSF_computeFreqParams(vsf);
	zassert_equal(obj.pwmFreqNow_Hz, 50000 - VSF_NUM_DELTA, "waits for the ISR");
}

ZTEST(realtimelib, test_vsf_steps_up_and_stops_at_target)
{
	VSF_Obj obj;
	VSF_Handle vsf = VSF_init(&obj, sizeof(obj));

	fill_user_params();
	VSF_initParams(vsf, &up);

	/* Below the allowed range the target is the minimum, and the step goes up */
	obj.pwmFreqSet_Hz = 1000;
	obj.pwmFreqNow_Hz = 4000;
	VSF_computeFreqParams(vsf);
	zassert_equal(obj.pwmFreqTarget_Hz, VSF_NUM_MIN_FREQ_HZ, "target clamped to min");
	zassert_equal(obj.pwmFreqNow_Hz, 4000 + VSF_NUM_DELTA, "one step up");

	/* A step larger than the distance stops at the target and sets the matching period */
	obj.state = VSF_STATE_IDLE;
	obj.pwmFreqDelta_Hz = 5000;
	VSF_computeFreqParams(vsf);
	zassert_equal(obj.pwmFreqNow_Hz, VSF_NUM_MIN_FREQ_HZ, "stops at the target");
	zassert_equal(obj.pwmPeriod, (200000000U / 2U) / VSF_NUM_MIN_FREQ_HZ, "period %u",
		      obj.pwmPeriod);

	/* Inside the range, at the target, nothing happens */
	obj.state = VSF_STATE_IDLE;
	obj.pwmFreqSet_Hz = VSF_NUM_MIN_FREQ_HZ;
	VSF_computeFreqParams(vsf);
	zassert_equal(obj.state, VSF_STATE_IDLE, "no update needed");
}

/* speedfr: speed from the angle difference, low-pass filtered */
ZTEST(realtimelib, test_speedfr_tracks_constant_speed)
{
	SPDFR_Obj obj;
	SPDFR_Handle sp = SPDFR_init(&obj, sizeof(obj));
	const float freq = 50.0f;
	float theta = 0.0f;

	zassert_not_null(sp, "SPDFR_init");
	fill_user_params();
	SPDFR_reset(sp);
	SPDFR_setParams(sp, &up);

	/* K2 = 1 / (1 + Ts 2 pi 10), K3 = (1 - K2) / (maxFreq Ts) */
	zassert_within(obj.K2, 1.0f / (1.0f + 50.0e-6f * MATH_TWO_PI * 10.0f), TOL, "K2");
	zassert_within(obj.K3, (1.0f - obj.K2) / (200.0f * 50.0e-6f), TOL, "K3");

	for (int i = 0; i < 4000; i++) {
		theta += MATH_TWO_PI * freq * 50.0e-6f;
		if (theta > MATH_PI) {
			theta -= MATH_TWO_PI;
		}
		SPDFR_run(sp, theta);
	}
	zassert_within(SPDFR_getSpeedHz(sp), freq, 0.05f, "speed %f Hz",
		       (double)SPDFR_getSpeedHz(sp));

	/* A constant angle gives zero speed again */
	for (int i = 0; i < 8000; i++) {
		SPDFR_run(sp, theta);
	}
	zassert_within(SPDFR_getSpeedHz(sp), 0.0f, 0.05f, "speed decays to zero");
}

/* esmo: the plant and scale factors follow from the motor and system data */
ZTEST(realtimelib, test_esmo_set_params)
{
	ESMO_Obj obj;
	ESMO_Handle esmo = ESMO_init(&obj, sizeof(obj));
	const float ts = 50.0e-6f;

	zassert_not_null(esmo, "ESMO_init");
	zassert_is_null(ESMO_init(&obj, 1), "a too small buffer must be rejected");

	fill_user_params();
	ESMO_resetParams(esmo);
	ESMO_setParams(esmo, &up);

	zassert_within(obj.Ts, ts, 1.0e-9f, "Ts");
	zassert_within(obj.speed_sf, 1.0f / 200.0f, 1.0e-9f, "speed scale");
	zassert_within(obj.voltage_sf, (MATH_ONE_OVER_THREE / 4096.0f) / 24.0f, 1.0e-9f,
		       "voltage scale");
	zassert_within(obj.current_sf, (1.0f / 4096.0f) / 10.0f, 1.0e-9f, "current scale");
	zassert_within(obj.Fdsmopos, 1.0f - 0.5f / 1.0e-3f * ts, TOL, "d plant %f",
		       (double)obj.Fdsmopos);
	zassert_within(obj.Fqsmopos, 1.0f - 0.5f / 1.25e-3f * ts, TOL, "q plant %f",
		       (double)obj.Fqsmopos);
	zassert_within(obj.Gdsmopos, (24.0f * ts / 10.0f) / 1.0e-3f, TOL, "d gain %f",
		       (double)obj.Gdsmopos);
	zassert_within(obj.Ke_PU, (0.01f * 200.0f * MATH_TWO_PI) / (24.0f * 4096.0f), 1.0e-7f,
		       "Ke");
	zassert_within(obj.pll_Umax, 1.0f, TOL, "pll limits");
}

/* dead_time_comp (prebuilt): disabled gives no output, enabled is limited */
ZTEST(realtimelib, test_dead_time_comp)
{
	DTC_Handle dtc = DTC_getInstance(DTC_MOTOR_1);
	DTC_InitParams_t p;
	DTC_Output_t out;

	zassert_not_null(dtc, "DTC_getInstance");
	DTC_setDefaultParams(&p);
	zassert_within(p.maxOutFactor, DTC_DEFAULT_MAX_OUT_FACTOR, TOL, "default limit");
	zassert_within(p.speedEnableLimit_Hz, DTC_DEFAULT_SPEED_ENABLE_LIMIT_HZ, TOL, "enable");
	zassert_within(p.speedDisableLimit_Hz, DTC_DEFAULT_SPEED_DISABLE_LIMIT_HZ, TOL, "disable");

	p.enableDTC = false;
	DTC_initParams(dtc, &p);
	zassert_false(DTC_isEnabled(dtc), "disabled");
	DTC_updateCoeffs(dtc, 50.0f, 50.0e-6f, 24.0f);
	DTC_run(dtc, 1.0f, 2.0f);
	DTC_getOutput(dtc, &out);
	zassert_within(out.Vd_V, 0.0f, TOL, "no output while disabled");
	zassert_within(out.Vq_V, 0.0f, TOL, "no output while disabled");

	DTC_enable(dtc);
	zassert_true(DTC_isEnabled(dtc), "enabled");
	DTC_updateGains(dtc, 1.0f, 1.0f, 0.5f, 0.5f);
	for (int i = 0; i < 3; i++) {
		DTC_updateCoeffs(dtc, 50.0f, 50.0e-6f, 24.0f);
		DTC_run(dtc, 1.0f, 2.0f);
	}
	/* The output saturates at maxOutFactor * Vdc and opposes the current */
	zassert_within(DTC_getVd(dtc), -p.maxOutFactor * 24.0f, 1.0e-3f, "Vd %f",
		       (double)DTC_getVd(dtc));
	zassert_within(DTC_getVq(dtc), -p.maxOutFactor * 24.0f, 1.0e-3f, "Vq %f",
		       (double)DTC_getVq(dtc));

	DTC_disable(dtc);
	zassert_false(DTC_isEnabled(dtc), "disabled again");
}

/* flux_obs (prebuilt): needs the tuning before the motor parameters */
ZTEST(realtimelib, test_flux_obs_parameter_checks_and_run)
{
	static uint8_t mem[256] __aligned(8);
	const size_t size = FLUX_OBS_getMemorySize();
	FLUX_OBS_Handle fo;
	const FLUX_OBS_TuningParams_t tune = {10.0f, 0.1f, 0.1f, 0.1f, 100.0f, false, false};
	const FLUX_OBS_TuningParams_t bad_tune = {0.1f, 10.0f, 0.1f, 0.1f, 100.0f, false, false};
	const FLUX_OBS_InitParams_t params = {50.0e-6f, 0.5f, 1.0e-3f, 0.01f};
	const FLUX_OBS_InitParams_t bad_params = {50.0e-6f, 0.0f, 1.0e-3f, 0.01f};
	FLUX_OBS_VoltageInput_t v = {1.0f, 0.0f};
	FLUX_OBS_CurrentInput_t c = {0.5f, 0.0f};
	FLUX_OBS_Output_t o = {0};

	zassert_true(size > 0 && size <= sizeof(mem), "size %u", (unsigned int)size);
	zassert_is_null(FLUX_OBS_init(mem, size - 1), "a too small buffer must be rejected");
	fo = FLUX_OBS_init(mem, size);
	zassert_not_null(fo, "FLUX_OBS_init");

	zassert_true(FLUX_OBS_setParams(fo, &params) < 0, "parameters before tuning");
	zassert_true(FLUX_OBS_setTuning(fo, &bad_tune) < 0, "max below min");
	zassert_equal(FLUX_OBS_setTuning(fo, &tune), 0, "tuning");
	zassert_true(FLUX_OBS_setParams(fo, &bad_params) < 0, "zero resistance");
	zassert_equal(FLUX_OBS_setParams(fo, &params), 0, "parameters");
	zassert_equal(FLUX_OBS_updateFilterParams(fo), 0, "filter");
	zassert_equal(FLUX_OBS_resetState(fo), 0, "reset");

	for (int i = 0; i < 4; i++) {
		zassert_equal(FLUX_OBS_run(fo, &v, &c, &o), 0, "run");
	}
	zassert_true(isfinite(o.angle_rad) && fabsf(o.angle_rad) <= MATH_PI, "angle %f",
		     (double)o.angle_rad);
	zassert_within(FLUX_OBS_getAngle_rad(fo), o.angle_rad, TOL, "getter matches output");
	zassert_within(FLUX_OBS_getSpeed_Hz(fo), o.speed_Hz, TOL, "speed getter");
}

/* FAST estimator (prebuilt): the object keeps the parameters it was given */
ZTEST(realtimelib, test_fast_estimator_set_params)
{
	EST_Handle e = EST_initEst(0);

	zassert_not_null(e, "EST_initEst");
	fill_user_params();
	EST_setParams(e, &up);
	zassert_within(EST_getRs_Ohm(e), 0.5f, TOL, "Rs %f", (double)EST_getRs_Ohm(e));
}

/* ti_tune (prebuilt) */
ZTEST(realtimelib, test_ti_tune_init)
{
	static uint8_t mem[512] __aligned(8);

	zassert_is_null(TiTune_Rs_init(mem, 1), "a too small buffer must be rejected");
	zassert_not_null(TiTune_Rs_init(mem, sizeof(mem)), "TiTune_Rs_init");
}
