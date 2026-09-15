/*
 * SPDX-FileCopyrightText: 2019 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#include "gui_json.h"
#include <gui_composer/transport.h>
#include <zephyr/data/json.h>
#include <zephyr/sys/util.h>

static char txString[MAX_STR_LEN];
#if (GUI_RXCMD_ENABLE)
static char rxString[MAX_STR_LEN];
static const gui_rx_cmd_t *rx_cmds_array;
static uint16_t rx_cmds_array_size;

/* Storage the Zephyr JSON parser decodes each RX command value into. */
static struct {
	bool boolean[GUI_RX_MAX_CMDS];
	struct json_obj_token number[GUI_RX_MAX_CMDS];
} rx_values;

static struct json_obj_descr rx_descr[GUI_RX_MAX_CMDS];
#endif

#if (GUI_RXCMD_ENABLE)
static bool gui_parse_string(void);
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
	if (size > GUI_RX_MAX_CMDS) {
		size = GUI_RX_MAX_CMDS;
	}

	for (uint16_t i = 0; i < size; i++) {
		struct json_obj_descr *d = &rx_descr[i];

		*d = (struct json_obj_descr){
			.field_name = command_array[i].string_cmd,
			.field_name_len = strlen(command_array[i].string_cmd),
		};

		if (command_array[i].type == GUI_RX_BOOL) {
			d->type = JSON_TOK_TRUE;
			d->offset = offsetof(__typeof__(rx_values), boolean) + i * sizeof(bool);
			d->field.size = sizeof(bool);
		} else {
			d->type = JSON_TOK_FLOAT;
			d->offset = offsetof(__typeof__(rx_values), number) +
				    i * sizeof(struct json_obj_token);
			d->field.size = sizeof(struct json_obj_token);
		}
	}

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
	static char true_str[] = "true";
	static char false_str[] = "false";
	bool ret = false;
	int64_t found;

	if (rx_cmds_array == NULL) {
		return false;
	}

	memset(&rx_values, 0, sizeof(rx_values));

	found = json_obj_parse(rxString, strlen(rxString), rx_descr, rx_cmds_array_size,
			       &rx_values);
	if (found < 0) {
		return false;
	}

	for (int i = 0; i < rx_cmds_array_size; i++) {
		if (!(found & BIT64(i)) || rx_cmds_array[i].callback == NULL) {
			continue;
		}

		if (rx_cmds_array[i].type == GUI_RX_BOOL) {
			rx_cmds_array[i].callback(rx_values.boolean[i] ? true_str : false_str);
		} else {
			rx_cmds_array[i].callback(rx_values.number[i].start);
		}
		ret = true;
	}

	return ret;
}
#endif
