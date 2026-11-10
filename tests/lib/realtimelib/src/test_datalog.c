/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/ztest.h>

#include <datalog.h>
#include <dlog_2ch_f.h>
#include <dlog_4ch_f.h>
#include <dlog_6ch_f.h>

/* datalog: copies four inputs into the capture buffers, one sample per update */
ZTEST(realtimelib, test_datalog_init_rejects_small_buffer)
{
	DATALOG_Obj obj;
	DATALOG_Handle h = DATALOG_init(&obj, sizeof(obj));

	zassert_not_null(h, "DATALOG_init");
	zassert_equal(obj.size, DATA_LOG_BUFF_SIZE, "default size");
	zassert_equal(obj.cntr, 0, "counter cleared");
	zassert_is_null(DATALOG_init(&obj, 1), "a too small buffer must be rejected");
}

ZTEST(realtimelib, test_datalog_captures_and_wraps)
{
	DATALOG_Obj obj;
	DATALOG_Handle h = DATALOG_init(&obj, sizeof(obj));
	float in[DATA_LOG_BUFF_NUM];

	for (int i = 0; i < DATA_LOG_BUFF_NUM; i++) {
		obj.iptr[i] = &in[i];
	}
	obj.flag_enableLogData = true;
	obj.flag_enableLogOneShot = false;
	obj.size = 3;

	for (int n = 0; n < 4; n++) {
		in[0] = 10.0f + n;
		in[1] = 20.0f + n;
		in[2] = 30.0f + n;
		in[3] = 40.0f + n;
		DATALOG_update(h);
	}

	/* Samples 0..2 fill the buffers, sample 3 wraps to index 0 and overwrites it */
	zassert_equal(obj.cntr, 1, "counter after wrap");
	zassert_within(datalogBuff1[0], 13.0f, 1.0e-6f, "buffer 1 wrapped");
	zassert_within(datalogBuff1[1], 11.0f, 1.0e-6f, "buffer 1 [1]");
	zassert_within(datalogBuff1[2], 12.0f, 1.0e-6f, "buffer 1 [2]");
	zassert_within(datalogBuff2[0], 23.0f, 1.0e-6f, "buffer 2");
	zassert_within(datalogBuff3[1], 31.0f, 1.0e-6f, "buffer 3");
	zassert_within(datalogBuff4[2], 42.0f, 1.0e-6f, "buffer 4");
	zassert_true(obj.flag_enableLogData, "continuous logging keeps running");
}

ZTEST(realtimelib, test_datalog_one_shot_stops_after_one_pass)
{
	DATALOG_Obj obj;
	DATALOG_Handle h = DATALOG_init(&obj, sizeof(obj));
	float in[DATA_LOG_BUFF_NUM] = {1.0f, 2.0f, 3.0f, 4.0f};

	for (int i = 0; i < DATA_LOG_BUFF_NUM; i++) {
		obj.iptr[i] = &in[i];
	}
	obj.size = 2;
	obj.flag_enableLogData = false;
	obj.flag_enableLogOneShot = true;

	/* A one-shot request arms the logger and rewinds it; nothing is stored yet */
	DATALOG_update(h);
	zassert_true(obj.flag_enableLogData, "armed");
	zassert_equal(obj.cntr, 0, "rewound");

	in[0] = 5.0f;
	DATALOG_update(h);
	in[0] = 6.0f;
	DATALOG_update(h);
	zassert_equal(obj.cntr, 2, "two samples stored");
	zassert_within(datalogBuff1[0], 5.0f, 1.0e-6f, "first sample");
	zassert_within(datalogBuff1[1], 6.0f, 1.0e-6f, "second sample");

	/* The next update finds the buffer full, stops, and still stores that one sample at 0 */
	in[0] = 7.0f;
	DATALOG_update(h);
	zassert_false(obj.flag_enableLogData, "stopped after one pass");
	zassert_false(obj.flag_enableLogOneShot, "one-shot cleared");

	in[0] = 8.0f;
	DATALOG_update(h);
	zassert_true(obj.flag_enableLogData == false, "stays stopped");
}

ZTEST(realtimelib, test_datalog_disabled_stores_nothing)
{
	DATALOG_Obj obj;
	DATALOG_Handle h = DATALOG_init(&obj, sizeof(obj));
	float in[DATA_LOG_BUFF_NUM] = {1.0f, 2.0f, 3.0f, 4.0f};

	for (int i = 0; i < DATA_LOG_BUFF_NUM; i++) {
		obj.iptr[i] = &in[i];
	}
	obj.flag_enableLogData = false;
	obj.flag_enableLogOneShot = false;
	datalogBuff1[0] = -99.0f;

	DATALOG_update(h);
	zassert_equal(obj.cntr, 0, "counter unchanged");
	zassert_within(datalogBuff1[0], -99.0f, 1.0e-6f, "buffer untouched");
}

/* dlog_2ch_f: wait for a rising edge through the trigger, then log every pre_scalar-th sample */
ZTEST(realtimelib, test_dlog_2ch_triggers_and_logs)
{
	DLOG_2CH_F d;
	float in1 = 0.0f, in2 = 0.0f, out1[3] = {0}, out2[3] = {0};

	DLOG_2CH_F_init(&d);
	zassert_equal(d.status, 0, "init clears the state");

	d.input_ptr1 = &in1;
	d.input_ptr2 = &in2;
	d.output_ptr1 = out1;
	d.output_ptr2 = out2;
	d.trig_value = 0.5f;
	d.pre_scalar = 2;
	d.size = 3;
	d.status = 1;

	/* Below the trigger, nothing is logged */
	in1 = 0.2f;
	DLOG_2CH_F_FUNC(&d);
	zassert_equal(d.status, 1, "still waiting");

	/* Rising through 0.5 starts the capture (this sample is not stored) */
	in1 = 0.8f;
	DLOG_2CH_F_FUNC(&d);
	zassert_equal(d.status, 2, "triggered");
	zassert_equal(d.count, 0, "nothing stored on the trigger sample");

	/* pre_scalar = 2: every second sample is stored */
	for (int n = 0; n < 6; n++) {
		in1 = 1.0f + n;
		in2 = -1.0f - n;
		DLOG_2CH_F_FUNC(&d);
	}
	zassert_within(out1[0], 2.0f, 1.0e-6f, "first stored sample");
	zassert_within(out1[1], 4.0f, 1.0e-6f, "second stored sample");
	zassert_within(out1[2], 6.0f, 1.0e-6f, "third stored sample");
	zassert_within(out2[0], -2.0f, 1.0e-6f, "channel 2 follows");
	zassert_within(out2[2], -6.0f, 1.0e-6f, "channel 2 last");

	/* A full buffer rearms the trigger */
	zassert_equal(d.status, 1, "waiting again");
	zassert_equal(d.count, 0, "counter rewound");
}

ZTEST(realtimelib, test_dlog_2ch_needs_a_rising_edge)
{
	DLOG_2CH_F d;
	float in1 = 0.9f, in2 = 0.0f, out1[2] = {0}, out2[2] = {0};

	DLOG_2CH_F_init(&d);
	d.input_ptr1 = &in1;
	d.input_ptr2 = &in2;
	d.output_ptr1 = out1;
	d.output_ptr2 = out2;
	d.trig_value = 0.5f;
	d.pre_scalar = 1;
	d.size = 2;
	d.status = 1;
	d.prev_value = 0.9f;

	/* Falling through the trigger must not start the capture */
	in1 = 0.1f;
	DLOG_2CH_F_FUNC(&d);
	zassert_equal(d.status, 1, "falling edge ignored");
}

/* dlog_4ch_f / dlog_6ch_f: the macro and the function behave the same way */
ZTEST(realtimelib, test_dlog_4ch_macro_and_function_agree)
{
	DLOG_4CH_F a, b;
	float in[4] = {0}, oa[4][2] = {{0}}, ob[4][2] = {{0}};

	DLOG_4CH_F_init(&a);
	DLOG_4CH_F_init(&b);
	for (int i = 0; i < 4; i++) {
		(&a.input_ptr1)[i] = &in[i];
		(&b.input_ptr1)[i] = &in[i];
		(&a.output_ptr1)[i] = oa[i];
		(&b.output_ptr1)[i] = ob[i];
	}
	a.trig_value = b.trig_value = 0.5f;
	a.pre_scalar = b.pre_scalar = 1;
	a.size = b.size = 2;
	a.status = b.status = 1;

	for (int n = 0; n < 5; n++) {
		in[0] = (n == 0) ? 0.0f : 1.0f + n;
		in[1] = 10.0f + n;
		in[2] = 20.0f + n;
		in[3] = 30.0f + n;
		DLOG_4CH_F_MACRO(a);
		DLOG_4CH_F_FUNC(&b);
		zassert_equal(a.status, b.status, "status step %d", n);
		zassert_equal(a.count, b.count, "count step %d", n);
	}
	for (int c = 0; c < 4; c++) {
		for (int k = 0; k < 2; k++) {
			zassert_within(oa[c][k], ob[c][k], 1.0e-6f, "ch %d [%d]", c, k);
		}
	}
	zassert_true(oa[3][0] > 0.0f, "data was captured");
}

/* dlog_6ch_f: unlike the other two, it triggers on any change of channel 0 */
ZTEST(realtimelib, test_dlog_6ch_triggers_on_change_and_logs)
{
	DLOG_6CH_F d;
	float in[DLOG_CH_NUM] = {0};
	static float out[DLOG_CH_NUM][2];

	DLOG_6CH_F_init(&d);
	zassert_equal(d.preScalar, 10, "default pre-scaler");
	zassert_equal(d.size, DLOG_BUF_SIZE, "default size");

	for (int c = 0; c < DLOG_CH_NUM; c++) {
		d.input_ptr[c] = &in[c];
		d.output_ptr[c] = out[c];
	}
	d.preScalar = 1;
	d.size = 2;

	/* Status 0 only arms the logger */
	DLOG_6CH_F_FUNC(&d);
	zassert_equal(d.status, 1, "armed");

	/* An unchanged channel 0 does not trigger */
	DLOG_6CH_F_FUNC(&d);
	zassert_equal(d.status, 1, "no change, no trigger");

	/* A change triggers; that sample is not stored */
	in[0] = 1.0f;
	DLOG_6CH_F_FUNC(&d);
	zassert_equal(d.status, 2, "triggered");
	zassert_equal(d.count, 0, "nothing stored yet");

	for (int n = 0; n < 2; n++) {
		for (int c = 0; c < DLOG_CH_NUM; c++) {
			in[c] = 100.0f * c + 10.0f * n + 1.0f;
		}
		DLOG_6CH_F_FUNC(&d);
	}

	for (int c = 0; c < DLOG_CH_NUM; c++) {
		zassert_within(out[c][0], 100.0f * c + 1.0f, 1.0e-6f, "ch %d first", c);
		zassert_within(out[c][1], 100.0f * c + 11.0f, 1.0e-6f, "ch %d second", c);
	}
	zassert_equal(d.status, 1, "waiting again after a full buffer");
}
