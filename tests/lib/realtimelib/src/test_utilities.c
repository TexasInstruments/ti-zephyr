/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include <zephyr/ztest.h>

#include <angle_gen.h>
#include <cpu_time.h>
#include <rampgen.h>
#include <traj.h>

#define TOL 1.0e-5f

/* traj: the output moves toward the target by at most maxDelta per step */
ZTEST(realtimelib, test_traj_rate_limit_and_clamp)
{
	TRAJ_Obj obj;
	TRAJ_Handle traj = TRAJ_init(&obj, sizeof(obj));

	zassert_not_null(traj, "TRAJ_init");
	TRAJ_setIntValue(traj, 0.0f);
	TRAJ_setMaxDelta(traj, 0.25f);
	TRAJ_setMinValue(traj, -1.0f);
	TRAJ_setMaxValue(traj, 0.8f);
	TRAJ_setTargetValue(traj, 1.0f);

	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.25f, TOL, "step 1 %f",
		       (double)TRAJ_getIntValue(traj));
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.50f, TOL, "step 2");
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.75f, TOL, "step 3");

	/* The next step would reach 1.0, but the output is clamped to maxValue */
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.8f, TOL, "clamped to max %f",
		       (double)TRAJ_getIntValue(traj));

	/* Going back down is also rate limited, and clamped at minValue */
	TRAJ_setTargetValue(traj, -5.0f);
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.55f, TOL, "step down %f",
		       (double)TRAJ_getIntValue(traj));
	for (int i = 0; i < 20; i++) {
		TRAJ_run(traj);
	}
	zassert_within(TRAJ_getIntValue(traj), -1.0f, TOL, "clamped to min");
}

ZTEST(realtimelib, test_traj_reaches_target_exactly)
{
	TRAJ_Obj obj;
	TRAJ_Handle traj = TRAJ_init(&obj, sizeof(obj));

	TRAJ_setIntValue(traj, 0.0f);
	TRAJ_setMaxDelta(traj, 1.0f);
	TRAJ_setMinValue(traj, -10.0f);
	TRAJ_setMaxValue(traj, 10.0f);
	TRAJ_setTargetValue(traj, 0.4f);

	/* A target closer than maxDelta is reached in one step and held */
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.4f, TOL, "one step");
	TRAJ_run(traj);
	zassert_within(TRAJ_getIntValue(traj), 0.4f, TOL, "held");
}

ZTEST(realtimelib, test_traj_init_rejects_small_buffer)
{
	TRAJ_Obj obj;

	zassert_is_null(TRAJ_init(&obj, 1), "a too small buffer must be rejected");
}

/* angle_gen: angle += freq * period * 2 * pi, wrapped to -pi..pi */
ZTEST(realtimelib, test_angle_gen_step_and_wrap)
{
	ANGLE_GEN_Obj obj;
	ANGLE_GEN_Handle gen = ANGLE_GEN_init(&obj, sizeof(obj));
	const float period = 1.0e-3f;
	const float freq = 100.0f;
	const float step = freq * period * MATH_TWO_PI;
	float expected = 0.0f;

	zassert_not_null(gen, "ANGLE_GEN_init");
	ANGLE_GEN_setParams(gen, period);
	zassert_within(ANGLE_GEN_getAngle(gen), 0.0f, TOL, "starts at zero");

	ANGLE_GEN_run(gen, freq);
	expected += step;
	zassert_within(ANGLE_GEN_getAngle(gen), expected, 1.0e-4f, "first step %f",
		       (double)ANGLE_GEN_getAngle(gen));

	/* 100 Hz at 1 kHz is 10 steps per revolution, so it wraps within 6 steps */
	for (int i = 0; i < 100; i++) {
		ANGLE_GEN_run(gen, freq);
		zassert_true(ANGLE_GEN_getAngle(gen) <= MATH_PI + 1.0e-4f &&
				     ANGLE_GEN_getAngle(gen) >= -MATH_PI - 1.0e-4f,
			     "angle %f left the -pi..pi range", (double)ANGLE_GEN_getAngle(gen));
	}

	/* 101 steps of 36 degrees is 10 revolutions plus one step */
	zassert_within(ANGLE_GEN_getAngle(gen), step, 1.0e-3f, "angle after 101 steps %f",
		       (double)ANGLE_GEN_getAngle(gen));
}

ZTEST(realtimelib, test_angle_gen_negative_frequency)
{
	ANGLE_GEN_Obj obj;
	ANGLE_GEN_Handle gen = ANGLE_GEN_init(&obj, sizeof(obj));

	ANGLE_GEN_setParams(gen, 1.0e-3f);
	ANGLE_GEN_run(gen, -100.0f);
	zassert_within(ANGLE_GEN_getAngle(gen), -100.0f * 1.0e-3f * MATH_TWO_PI, 1.0e-4f,
		       "reverse rotation %f", (double)ANGLE_GEN_getAngle(gen));
}

/* rampgen: out += step * freq, wrapping at 1.0 (per unit) */
ZTEST(realtimelib, test_rampgen_wraps_at_one)
{
	RAMPGEN ramp;

	RAMPGEN_config(&ramp, 1000.0f, 100.0f);
	RAMPGEN_reset(&ramp);
	zassert_within(ramp.out, 0.0f, TOL, "reset");

	/* 100 Hz at a 1 kHz rate: 0.1 per step */
	RAMPGEN_run(&ramp);
	zassert_within(ramp.out, 0.1f, TOL, "step 1 %f", (double)ramp.out);
	for (int i = 0; i < 8; i++) {
		RAMPGEN_run(&ramp);
	}
	zassert_within(ramp.out, 0.9f, 1.0e-4f, "step 9 %f", (double)ramp.out);

	RAMPGEN_run(&ramp);
	zassert_true(ramp.out >= 0.0f && ramp.out < 1.0e-3f, "wrapped to ~0, got %f",
		     (double)ramp.out);
}

/* cpu_time: tracks the time spent between two timer counts, with min and max */
ZTEST(realtimelib, test_cpu_time_tracks_min_max_and_reset)
{
	CPU_TIME_Obj obj;
	CPU_TIME_Handle cpu = CPU_TIME_init(&obj, sizeof(obj));

	zassert_not_null(cpu, "CPU_TIME_init");
	CPU_TIME_reset(cpu);
	zassert_equal(obj.deltaMax[0], 0, "max cleared by reset");
	zassert_equal(obj.deltaMin[0], 0x7FFF, "min set high by reset");
	zassert_true(obj.flag_resetStatus, "reset flag set");

	/* The timer counts down: elapsed = start - now (less a fixed overhead) */
	CPU_TIME_updateCnts(cpu, 1000, 0);
	CPU_TIME_run(cpu, 900, 0);
	zassert_true(obj.deltaNow[0] > 0 && obj.deltaNow[0] <= 100, "delta %u", obj.deltaNow[0]);
	uint16_t first = obj.deltaNow[0];

	CPU_TIME_updateCnts(cpu, 1000, 0);
	CPU_TIME_run(cpu, 700, 0);
	zassert_true(obj.deltaNow[0] > first, "a longer interval gives a larger delta");
	zassert_equal(obj.deltaMax[0], obj.deltaNow[0], "max follows the largest delta");

	CPU_TIME_updateCnts(cpu, 1000, 0);
	CPU_TIME_run(cpu, 990, 0);
	zassert_true(obj.deltaNow[0] < first, "a shorter interval gives a smaller delta");
	zassert_true(obj.deltaMin[0] <= obj.deltaNow[0], "min follows the smallest delta");
	zassert_true(obj.deltaMax[0] > obj.deltaMin[0], "max above min");
}

ZTEST(realtimelib, test_cpu_time_ignores_bad_module_index)
{
	CPU_TIME_Obj obj;
	CPU_TIME_Handle cpu = CPU_TIME_init(&obj, sizeof(obj));

	CPU_TIME_reset(cpu);

	/* An index past the end of the tables must not write out of bounds */
	CPU_TIME_run(cpu, 500, MOD_INDEX_MAX);
	CPU_TIME_run(cpu, 500, 0xFFFF);
	zassert_equal(obj.deltaMax[0], 0, "valid slots untouched");
}

ZTEST(realtimelib, test_cpu_time_width_ratio)
{
	CPU_TIME_Obj obj;
	CPU_TIME_Handle cpu = CPU_TIME_init(&obj, sizeof(obj));

	CPU_TIME_reset(cpu);

	/* The period is given in PWM counts and stored doubled (up-down counter) */
	CPU_TIME_setCtrlPeriod(cpu, 1000);
	zassert_equal(obj.ctrlPeriodCnt, 2000, "period count");

	obj.deltaMax[0] = 250;
	CPU_TIME_calcCPUWidthRatio(cpu);
	zassert_within(obj.cpuWidthRatio, 250.0f / 2000.0f, TOL, "ratio %f",
		       (double)obj.cpuWidthRatio);
}
