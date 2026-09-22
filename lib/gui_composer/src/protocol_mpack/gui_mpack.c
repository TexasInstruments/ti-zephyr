/*
 * Copyright (c) 2019, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gui_mpack.h"
#include <ti/gui_composer/transport.h>
#include <ti/gui_composer/mpack/mpack.h>

static char txString[MAX_STR_LEN];
static char rxString[MAX_STR_LEN];

/*
 * Exact byte count gui_rx_char_callback() received for the message
 * currently in rxString. rxString holds raw mpack binary data, not a
 * C string -- mpack's positive-fixint encoding represents the integer
 * 0 as a literal 0x00 byte, so any message with a plain 0 value
 * anywhere before its end (a very common value: inversion=0, a
 * selectedIndex of 0, etc.) contains an embedded NUL. strlen(rxString)
 * would stop counting right there, silently truncating the buffer
 * mpack_reader_init_data() is given. This is tracked separately so
 * gui_parse_string() can use the real length instead.
 */
static uint16_t rxStringLen;

static const gui_rx_cmd_t *rx_cmds_array;
static uint16_t rx_cmds_array_size;

#define MPACK_WRITER_BUFFER_LEN 50

/*
 * Buffer the mpack writer accumulates an encoded message into before
 * it is flushed to the transport. Declared one byte larger than the
 * capacity given to mpack_writer_init() below, so mpack_flush_callback()
 * can append its trailing '\n' delimiter without writing past the end
 * of the array.
 */
static char mpack_tx_buffer[MPACK_WRITER_BUFFER_LEN + 1];

/* Shared with guicomm_mpack.c, which encodes each protocol message by
 * calling mpack_write_*() on this writer directly.
 */
mpack_writer_t writer;

static bool gui_parse_string(void);
static bool gui_rx_char_callback(char data);

void mpack_flush_callback(mpack_writer_t *w, const char *buffer, size_t count)
{
	(void)w;
	(void)buffer;

	/* Append the delimiter the GUI Composer wire protocol uses to mark
	 * the end of a message, then transmit the encoded message together
	 * with that delimiter.
	 */
	mpack_tx_buffer[count] = '\n';
	gui_transmit_string_blocking(mpack_tx_buffer, (count + 1));
}

void gui_init(void)
{
	gui_composer_transport_start(gui_rx_char_callback);

	/*
	 * Initialize the mpack writer and register the flush callback
	 * invoked whenever a fully encoded message is ready to transmit.
	 */
	mpack_writer_init(&writer, mpack_tx_buffer, MPACK_WRITER_BUFFER_LEN);
	mpack_writer_set_flush(&writer, mpack_flush_callback);
}

void gui_reset_writer(void)
{
	mpack_writer_init(&writer, mpack_tx_buffer, MPACK_WRITER_BUFFER_LEN);
	mpack_writer_set_flush(&writer, mpack_flush_callback);
}

void gui_init_rx_cmd(const gui_rx_cmd_t *command_array, uint16_t size)
{
	rx_cmds_array = command_array;
	rx_cmds_array_size = size;
}

void gui_transmit_string_blocking(char *str, size_t count)
{
	uint16_t i;

	for (i = 0; i < count; i++) {
		gui_composer_transport_send_char(str[i]);
	}
}

void gui_transmit_data(const gui_tx_cmd_t *tx_cmd_array, uint16_t size)
{
	uint16_t i;

	for (i = 0; i < size; i++) {
		snprintf(txString, sizeof(txString), tx_cmd_array[i].formatting_string_cmd,
			 *tx_cmd_array[i].param);
		gui_transmit_string_blocking(txString, strlen(txString));
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
			rxStringLen = charCnt;
			if (gui_parse_string() == true) {
				ret = true;
			}
		}
	}
	return ret;
}

/**
 * @brief Finds the RX command matching a parsed key and invokes its callback.
 *
 * @param reader mpack reader, positioned right after the key, ready to
 *               read the value if a matching command is found.
 * @param key Parsed key bytes (not null-terminated).
 * @param key_len Length of key, in bytes.
 */
static void dispatch_rx_cmd(mpack_reader_t *reader, const char *key, size_t key_len)
{
	for (int j = 0; j < rx_cmds_array_size; j++) {
		if (strncmp(key, rx_cmds_array[j].string_cmd, key_len) == 0) {
			if (rx_cmds_array[j].callback != NULL) {
				mpack_tag_t tag = mpack_read_tag(reader);

				rx_cmds_array[j].callback(&tag, reader);
			}
			break;
		}
	}
}

/**
 * @brief Parses a string looking for command.
 *
 * @return true to wake-up MCU when a command is received, false to stay in LPM
 */
static bool gui_parse_string(void)
{
	mpack_reader_t reader;
	mpack_tag_t tag;

	int i;
	int containerSize;

	mpack_reader_init_data(&reader, rxString, rxStringLen);
	tag = mpack_read_tag(&reader);

	if (mpack_tag_type(&tag) == mpack_type_map) {
		/* Map tag count specifies the number of key:value pairs, so to read
		 * each individual key and value, we must double the count
		 */
		containerSize = (mpack_tag_map_count(&tag) * 2);
		for (i = 0; i < containerSize; i++) {
			tag = mpack_read_tag(&reader);
			if (mpack_tag_type(&tag) == mpack_type_str) {
				/* Sized to MAX_STR_LEN. Upstream used char buffer[8], which
				 * overflows on any token name >= 8 bytes (e.g. "blinkEnable",
				 * 11 bytes) -- fixed by sizing to MAX_STR_LEN here. The tag's
				 * declared string length is untrusted wire data, though, and
				 * is not implicitly bounded by this buffer's size or by how
				 * many bytes actually remain in the message -- explicitly
				 * checked below before it is ever used as a copy length.
				 */
				char buffer[MAX_STR_LEN];
				size_t key_len = mpack_tag_str_length(&tag);

				if (key_len >= sizeof(buffer)) {
					mpack_reader_flag_error(&reader, mpack_error_too_big);
					break;
				}

				mpack_read_bytes(&reader, buffer, key_len);
				dispatch_rx_cmd(&reader, buffer, key_len);
			}
		}
	}

	if (mpack_reader_destroy(&reader) != mpack_ok) {
		return false;
	}
	return true;
}
