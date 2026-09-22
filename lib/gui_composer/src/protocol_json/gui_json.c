/*
 * Copyright (c) 2019, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gui_json.h"
#include <ti/gui_composer/transport.h>
#include <ti/gui_composer/jsmn/jsmn.h>
#include <zephyr/sys/util.h>

static char txString[MAX_STR_LEN];
#if (GUI_RXCMD_ENABLE)
static char rxString[MAX_STR_LEN];
static jsmn_parser p;
static jsmntok_t t[64]; /* We expect no more than 64 tokens */
static const gui_rx_cmd_t *rx_cmds_array;
static uint16_t rx_cmds_array_size;
#endif

#if (GUI_RXCMD_ENABLE)
static bool gui_parse_string(void);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
#endif
static bool gui_rx_char_callback(char data);

void gui_init(void)
{
	gui_composer_transport_start(gui_rx_char_callback);
	rx_cmds_array_size = 0;
	rx_cmds_array = NULL;
}

#if (GUI_RXCMD_ENABLE)
void gui_init_rx_cmd(const gui_rx_cmd_t *command_array, uint16_t size)
{
	rx_cmds_array = command_array;
	rx_cmds_array_size = size;
}
#endif

void gui_transmit_string_blocking(char *str)
{
	uint16_t i;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] != 0) {
			gui_composer_transport_send_char(str[i]);
		}
	}
}

void gui_transmit_data(const gui_tx_cmd_t *tx_cmd_array, uint16_t size)
{
	uint16_t i;

	for (i = 0; i < size; i++) {
		snprintf(txString, sizeof(txString), tx_cmd_array[i].formatting_string_cmd,
			 *tx_cmd_array[i].param);
		gui_transmit_string_blocking(txString);
	}
}

/**
 * @brief Callback function for the GUI transport. Called when byte is received.
 *
 * @param data The byte received from GUI Comm interface.
 * @return true to wake-up MCU, false to stay in LPM
 */
static bool gui_rx_char_callback(char data)
{
#if (GUI_RXCMD_ENABLE)
	static bool rxInProgress;
	static uint16_t charCnt;
	bool ret = false;

	if (!rxInProgress) {
		if ((data != '\n')) {
			rxInProgress = true;
			charCnt = 0;
			rxString[charCnt] = data;
		}
	} else {
		charCnt++;
		if (charCnt >= MAX_STR_LEN) {
			/* Message too long for rxString (including the byte
			 * needed for the null terminator) -- abort it and
			 * wait for the next one, whether this byte is a
			 * regular character or the terminator itself.
			 */
			rxInProgress = false;
		} else if (data != '\n') {
			rxString[charCnt] = data;
		} else {
			rxInProgress = false;
			rxString[charCnt] = '\0';
			if (gui_parse_string() == true) {
				ret = true;
			}
		}
	}
	return ret;
#else
	return true;
#endif
}

#if (GUI_RXCMD_ENABLE)
/**
 * @brief Parses a string looking for JSON objects.
 *
 * @return true to wake-up MCU when a command is received, false to stay in LPM
 */
static bool gui_parse_string(void)
{
	int i;
	int r;
	int j;
	bool ret = false;

	jsmn_init(&p);

	r = jsmn_parse(&p, rxString, strlen(rxString), t, ARRAY_SIZE(t));

	if (rx_cmds_array != NULL) {
		for (i = 1; i < r; i++) {
			/* Every key must be followed by a value token; a key with
			 * nothing after it (i.e. the last token in a full token
			 * array) has no value to read and is skipped.
			 */
			if (i + 1 >= r) {
				break;
			}
			for (j = 0; j < rx_cmds_array_size; j++) {
				if (jsoneq(rxString, &t[i], rx_cmds_array[j].string_cmd) == 0) {
					if (rx_cmds_array[j].callback != NULL) {
						rx_cmds_array[j].callback(rxString + t[i + 1].start);
					}
					i++;
					ret = true;
				}
			}
		}
	}

	return ret;
}

/**
 * @brief Compare JSON keys.
 *
 * @param json The JSON string.
 * @param tok The JSON token.
 * @param s The string to check for equality.
 * @return 0 if string found, -1 if not.
 */
static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
	    strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
#endif
