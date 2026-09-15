/*
 * SPDX-FileCopyrightText: 2019 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */

#ifndef ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUI_JSON_H_
#define ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUI_JSON_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gui_composer/gui_rx.h>

/** @brief Maximum string for RX */
#ifndef MAX_STR_LEN
#define MAX_STR_LEN 64
#endif

/** @brief Enable GUI Reception of commands */
#define GUI_RXCMD_ENABLE (1)

#if (GUI_RXCMD_ENABLE)
/** @brief Maximum number of RX commands (limit of the Zephyr JSON parser). */
#define GUI_RX_MAX_CMDS 62

/** @brief Value received with an RX command, as passed to guicomm_read_*(). */
typedef char *gui_rx_value_t;

/** @brief Callback for RX Commands (called when command is received from GUI) */
typedef void (*gui_rx_callback_cmd_t)(gui_rx_value_t value);

/** @brief Structure for RX commands. */
typedef struct {
	char *string_cmd;             /**< RX Command (received from GUI) */
	enum gui_rx_type type;        /**< Type of the value carried by the command. */
	gui_rx_callback_cmd_t callback; /**< Function executed if command is detected. */
} gui_rx_cmd_t;
#endif

/** @brief Structure for TX commands. */
typedef struct {
	char *formatting_string_cmd; /**< TX Command including sprintf formatting. */
	void **param;              /**< Parameter sent with TX command. */
} gui_tx_cmd_t;

/** @brief Initializes the GUI communication and protocol. */
extern void gui_init(void);

#if (GUI_RXCMD_ENABLE)
/**
 * @brief Initializes structure to receive and process commands.
 *
 * @param rx_cmd_array Pointer to the array of RX Commands. These commands
 *                   will execute a callback function when received.
 * @param size Size of rx_cmd_array.
 */
extern void gui_init_rx_cmd(const gui_rx_cmd_t *rx_cmd_array, uint16_t size);
#endif

/**
 * @brief Transmits an array of TX commands.
 *
 * @param tx_cmd_array Pointer to the array of TX commands. These commands
 *                  will be sent with corresponding formatted data.
 * @param size Size of TxCmdArray.
 */
extern void gui_transmit_data(const gui_tx_cmd_t *tx_cmd_array, uint16_t size);

/**
 * @brief Transmits a string to GUI.
 *
 * @param str The string to be sent.
 */
extern void gui_transmit_string_blocking(char *str);

#endif /* ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUI_JSON_H_ */
