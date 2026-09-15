/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ti/gui_composer/transport.h>
#include "guicomm_json.h"
#include "gui_json.h"

/* Sized for the longest line any GUIComm_send* below can produce:
 * `{"` + a command name + `":` + a signed 64-bit value + `}\n` + '\0'.
 */
static char txbuf[48];

int guicomm_send_uint64(char *cmd, uint8_t cmdLength, uint64_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%" PRIu64 "}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_int64(char *cmd, uint8_t cmdLength, int64_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%" PRId64 "}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_uint(char *cmd, uint8_t cmdLength, uint32_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%u}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_int(char *cmd, uint8_t cmdLength, uint32_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%d}\n", cmd, (int32_t)val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_uint8(char *cmd, uint8_t cmdLength, uint8_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%u}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_int8(char *cmd, uint8_t cmdLength, int8_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%d}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_int16(char *cmd, uint8_t cmdLength, int16_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%d}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_uint16(char *cmd, uint8_t cmdLength, uint16_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%u}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_uint32(char *cmd, uint8_t cmdLength, uint32_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%u}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_int32(char *cmd, uint8_t cmdLength, int32_t val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%d}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_bool(char *cmd, uint8_t cmdLength, bool val)
{
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":%s}\n", cmd, val ? "true" : "false");
	gui_transmit_string_blocking(txbuf);
	return 0;
}

int guicomm_send_string(char *cmd, uint8_t cmdLength, const char *val)
{
	/* Quoted, unlike the numeric/bool sends above, so the resulting
	 * line stays valid JSON.
	 */
	snprintf(txbuf, sizeof(txbuf), "{\"%s\":\"%s\"}\n", cmd, val);
	gui_transmit_string_blocking(txbuf);
	return 0;
}

uint8_t guicomm_read_uint8(char *string)
{
	return (uint8_t)strtoul(string, NULL, 10);
}

uint16_t guicomm_read_uint16(char *string)
{
	return (uint16_t)strtoul(string, NULL, 10);
}

uint32_t guicomm_read_uint32(char *string)
{
	return (uint32_t)strtoul(string, NULL, 10);
}

uint64_t guicomm_read_uint64(char *string)
{
	return (uint64_t)strtoul(string, NULL, 10);
}

int8_t guicomm_read_int8(char *string)
{
	return (int8_t)strtol(string, NULL, 10);
}

int16_t guicomm_read_int16(char *string)
{
	return (int16_t)strtol(string, NULL, 10);
}

int32_t guicomm_read_int32(char *string)
{
	return (int32_t)strtol(string, NULL, 10);
}

int64_t guicomm_read_int64(char *string)
{
	return (int64_t)strtol(string, NULL, 10);
}

bool guicomm_read_bool(char *string)
{
	if (strncmp(string, "true", 4) == 0) {
		return true;
	} else {
		return false;
	}
}

float guicomm_read_float(char *string)
{
	return strtof(string, NULL);
}

double guicomm_read_double(char *string)
{
	return strtod(string, NULL);
}
