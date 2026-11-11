/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/serial/uart_emul.h>
#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#include <motorvision.h>
#include <motorvision_sine_generator.h>

#define MV_START 115
#define MV_END   101
#define MV_PACKET_SIZE 16

/* Commands used below (enum MV_CMDS in motorvision.c) */
#define CMD_PACKET_ERROR 39
#define CMD_INIT         40
#define CMD_WRITE_VAR    43
#define CMD_INIT_STREAM  44
#define CMD_START_A      45
#define CMD_START_POLL_1 63
#define CMD_POLLING_DATA 77
#define CMD_CLEAR_PLOT   78
#define CMD_STREAM_1_VAR 53

static const struct device *const uart = DEVICE_DT_GET(DT_NODELABEL(mv_uart));

/* The application variable table that MotorVision reads and writes */
static volatile bool mv_bool = true;
static volatile float mv_float;

#define HASH_U16   0
#define HASH_U32   5
#define HASH_BOOL  6
#define HASH_FLOAT 13

volatile void *MVHASH_hash_table[MVHASH_NUMBER_OF_VARIABLES];
int16_t MVHASH_uint16_t_startIndex = HASH_U16;
int16_t MVHASH_uint32_t_startIndex = HASH_U32;
int16_t MVHASH_int16_t_startIndex = -1;
int16_t MVHASH_int32_t_startIndex = -1;
int16_t MVHASH_bool_startIndex = HASH_BOOL;
int16_t MVHASH_float_startIndex = HASH_FLOAT;

static void *setup(void)
{
	for (size_t i = 0; i < MVHASH_NUMBER_OF_VARIABLES; i++) {
		MVHASH_hash_table[i] = &mv_float;
	}
	MVHASH_hash_table[HASH_U16] = &rw_var_6;
	MVHASH_hash_table[HASH_U32] = &rw_var_3;
	MVHASH_hash_table[HASH_BOOL] = &mv_bool;
	MVHASH_hash_table[HASH_FLOAT] = &sine_wave1;

	return NULL;
}

static void before(void *unused)
{
	ARG_UNUSED(unused);

	zassert_true(device_is_ready(uart), "emulated UART not ready");
	uart_emul_flush_rx_data(uart);
	uart_emul_flush_tx_data(uart);
	MOTORVISION_init((void *)uart);
}

/* Feed one 16 byte packet: START, command, 12 payload bytes, END */
static void send_packet(uint8_t cmd, const uint8_t *payload, size_t len)
{
	uint8_t pkt[MV_PACKET_SIZE] = {0};

	pkt[0] = MV_START;
	pkt[1] = cmd;
	memcpy(&pkt[2], payload, len);
	pkt[MV_PACKET_SIZE - 1] = MV_END;

	zassert_equal(uart_emul_put_rx_data(uart, pkt, sizeof(pkt)), sizeof(pkt));
	/* The emulated UART delivers the bytes from a work queue */
	k_msleep(100);
}

static size_t read_tx(uint8_t *buf, size_t size)
{
	return uart_emul_get_tx_data(uart, buf, size);
}

/* The next call of the polling function sends the acknowledge of the last command */
static void expect_ack(uint8_t cmd)
{
	uint8_t tx[8];
	size_t n;

	MOTORVISION_captureAndSendPollingData();
	n = read_tx(tx, sizeof(tx));
	zassert_equal(n, 3, "ack length %u", (unsigned int)n);
	zassert_equal(tx[0], MV_START, "start byte");
	zassert_equal(tx[1], cmd, "ack of command %u, got %u", cmd, tx[1]);
	zassert_equal(tx[2], MV_END, "end byte");
}

ZTEST(motorvision, test_init_command_is_acknowledged)
{
	send_packet(CMD_INIT, NULL, 0);
	expect_ack(CMD_INIT);

	/* Nothing more to send once the acknowledge is out and nothing is polled */
	MOTORVISION_captureAndSendPollingData();
	zassert_equal(read_tx(&(uint8_t){0}, 1), 0, "no extra bytes");
}

ZTEST(motorvision, test_bad_packet_gives_error)
{
	uint8_t pkt[MV_PACKET_SIZE] = {MV_START, CMD_INIT};

	/* The end byte is missing */
	pkt[MV_PACKET_SIZE - 1] = 0x00;
	zassert_equal(uart_emul_put_rx_data(uart, pkt, sizeof(pkt)), sizeof(pkt));
	k_msleep(100);

	expect_ack(CMD_PACKET_ERROR);
}

ZTEST(motorvision, test_unknown_command_gives_error)
{
	send_packet(0xEE, NULL, 0);
	expect_ack(CMD_PACKET_ERROR);
}

ZTEST(motorvision, test_polling_sends_variable_value)
{
	const uint8_t idx[] = {HASH_U16};
	uint8_t tx[16];

	rw_var_6 = 0x1234;
	send_packet(CMD_INIT, NULL, 0);
	expect_ack(CMD_INIT);

	send_packet(CMD_START_POLL_1, idx, sizeof(idx));
	expect_ack(CMD_START_POLL_1);
	zassert_equal(MV_pollingArraySize, 1, "one polling variable");

	/* START, POLLING_DATA, hash index, value low byte, value high byte, END */
	MOTORVISION_captureAndSendPollingData();
	zassert_equal(read_tx(tx, sizeof(tx)), 6, "polling packet length");
	zassert_equal(tx[0], MV_START);
	zassert_equal(tx[1], CMD_POLLING_DATA);
	zassert_equal(tx[2], HASH_U16);
	zassert_equal(tx[3], 0x34, "low byte");
	zassert_equal(tx[4], 0x12, "high byte");
	zassert_equal(tx[5], MV_END);
}

ZTEST(motorvision, test_write_variables)
{
	/* hash index, then the value most significant byte first */
	const uint8_t w16[] = {HASH_U16, 0xAB, 0xCD};
	const uint8_t w32[] = {HASH_U32, 0x12, 0x34, 0x56, 0x78};
	const uint8_t wbool[] = {HASH_BOOL, 0x00, 0x00};

	send_packet(CMD_WRITE_VAR, w16, sizeof(w16));
	zassert_equal(rw_var_6, 0xABCD, "uint16 write 0x%04x", rw_var_6);
	expect_ack(CMD_WRITE_VAR);

	send_packet(CMD_WRITE_VAR, w32, sizeof(w32));
	zassert_equal(rw_var_3, 0x12345678U, "uint32 write 0x%08x", (unsigned int)rw_var_3);
	expect_ack(CMD_WRITE_VAR);

	mv_bool = true;
	send_packet(CMD_WRITE_VAR, wbool, sizeof(wbool));
	zassert_false(mv_bool, "bool write");
	expect_ack(CMD_WRITE_VAR);
}

ZTEST(motorvision, test_stream_one_variable)
{
	const uint8_t vars[] = {HASH_FLOAT, HASH_FLOAT, HASH_FLOAT, HASH_FLOAT};
	static uint8_t tx[1024];
	size_t n;
	size_t packets = 0;

	send_packet(CMD_INIT, NULL, 0);
	expect_ack(CMD_INIT);
	send_packet(CMD_INIT_STREAM, vars, sizeof(vars));
	expect_ack(CMD_INIT_STREAM);
	send_packet(CMD_START_A, NULL, 0);
	expect_ack(CMD_START_A);

	/* 1.5f is 0x3FC00000, sent least significant byte first */
	sine_wave1 = 1.5f;

	/* The capture buffer holds 50 samples; the first send swaps buffers */
	for (int i = 0; i < 50; i++) {
		MOTORVISION_capturePlotData();
	}
	MOTORVISION_sendPlotData();
	zassert_equal(read_tx(tx, sizeof(tx)), 0, "nothing is sent until the swap");
	MOTORVISION_sendPlotData();

	n = read_tx(tx, sizeof(tx));
	/* A clear-plot command, then 50 packets of START, cmd, 4 value bytes, END */
	zassert_equal(n, 3 + 50 * 7, "stream length %u", (unsigned int)n);
	zassert_equal(tx[0], MV_START);
	zassert_equal(tx[1], CMD_CLEAR_PLOT, "clear plot first");
	zassert_equal(tx[2], MV_END);

	for (size_t off = 3; off < n; off += 7) {
		zassert_equal(tx[off], MV_START, "start at %u", (unsigned int)off);
		zassert_equal(tx[off + 1], CMD_STREAM_1_VAR);
		zassert_equal(tx[off + 2], 0x00);
		zassert_equal(tx[off + 3], 0x00);
		zassert_equal(tx[off + 4], 0xC0);
		zassert_equal(tx[off + 5], 0x3F);
		zassert_equal(tx[off + 6], MV_END);
		packets++;
	}
	zassert_equal(packets, 50);
}

ZTEST(motorvision, test_sine_generator)
{
	SineGen_init();
	zassert_within(amp1, 30.0f, 1.0e-6f, "amp");
	zassert_within(offset1, 50.0f, 1.0e-6f, "offset");

	/* At counter 1 the argument is freq1 = 0.01 rad */
	SineGen_update();
	zassert_within(sine_wave1, (50.0f - 60.0f * 0.0099998f) / 100.0f, 1.0e-3f, "wave 1 %f",
		       (double)sine_wave1);
	zassert_within(sine_wave2, (50.0f + 60.0f * 0.0099998f) / 100.0f, 1.0e-3f, "wave 2 %f",
		       (double)sine_wave2);
	zassert_equal(SM_isrCounter, 1, "counter");
}

ZTEST_SUITE(motorvision, NULL, setup, before, NULL, NULL);
