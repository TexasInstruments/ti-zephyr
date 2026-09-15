/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "guicomm_mpack.h"
#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/transport.h>
#include "gui_mpack.h"

extern mpack_writer_t writer;

/**
 * @brief Flushes the current message and reports mpack's error state.
 *
 * Every GUIComm_send* function below calls this last, after writing its
 * one key/value pair into the shared writer.
 */
static int send_helper_flush(void)
{
	mpack_writer_flush_message(&writer);

	if (writer.error != mpack_ok) {
		/* mpack latches its writer into a permanent error state on
		 * failure; recover it so the next send has a fresh chance
		 * instead of silently no-op'ing forever.
		 */
		gui_reset_writer();
		return -1;
	}

	return 0;
}

int guicomm_send_bool(char *cmd, uint8_t cmdLength, bool val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_bool(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_string(char *cmd, uint8_t cmdLength, const char *val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_cstr(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_uint(char *cmd, uint8_t cmdLength, uint32_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_uint(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_int(char *cmd, uint8_t cmdLength, uint32_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_int(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_uint8(char *cmd, uint8_t cmdLength, uint8_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_u8(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_int8(char *cmd, uint8_t cmdLength, int8_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_i8(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_uint16(char *cmd, uint8_t cmdLength, uint16_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_u16(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_int16(char *cmd, uint8_t cmdLength, int16_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_i16(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_uint32(char *cmd, uint8_t cmdLength, uint32_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_u32(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_int32(char *cmd, uint8_t cmdLength, int32_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_i32(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_uint64(char *cmd, uint8_t cmdLength, uint64_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_u64(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

int guicomm_send_int64(char *cmd, uint8_t cmdLength, int64_t val)
{
	mpack_start_map(&writer, 1);
	mpack_write_str(&writer, cmd, cmdLength);
	mpack_write_i64(&writer, val);
	mpack_finish_map(&writer);
	return send_helper_flush();
}

uint8_t guicomm_read_uint8(mpack_tag_t *tag)
{
	return (uint8_t)mpack_tag_uint_value(tag);
}

uint16_t guicomm_read_uint16(mpack_tag_t *tag)
{
	return (uint16_t)mpack_tag_uint_value(tag);
}

uint32_t guicomm_read_uint32(mpack_tag_t *tag)
{
	return (uint32_t)mpack_tag_uint_value(tag);
}

uint64_t guicomm_read_uint64(mpack_tag_t *tag)
{
	return (uint64_t)mpack_tag_uint_value(tag);
}

int8_t guicomm_read_int8(mpack_tag_t *tag)
{
	return (int8_t)mpack_tag_int_value(tag);
}

int16_t guicomm_read_int16(mpack_tag_t *tag)
{
	return (int16_t)mpack_tag_int_value(tag);
}

int32_t guicomm_read_int32(mpack_tag_t *tag)
{
	return (int32_t)mpack_tag_int_value(tag);
}

int64_t guicomm_read_int64(mpack_tag_t *tag)
{
	return (int64_t)mpack_tag_int_value(tag);
}

bool guicomm_read_bool(mpack_tag_t *tag)
{
	return mpack_tag_bool_value(tag);
}

float guicomm_read_float(mpack_tag_t *tag)
{
	return mpack_tag_float_value(tag);
}

double guicomm_read_double(mpack_tag_t *tag)
{
	return mpack_tag_double_value(tag);
}
