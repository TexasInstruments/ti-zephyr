/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUICOMM_JSON_H_
#define ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUICOMM_JSON_H_

/**
 * @brief Sends a 32-bit integer.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_int(char *cmd, uint8_t cmdLength, uint32_t val);

/**
 * @brief Sends a 32-bit unsigned integer.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_uint(char *cmd, uint8_t cmdLength, uint32_t val);

/**
 * @brief Sends an uint8_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_uint8(char *cmd, uint8_t cmdLength, uint8_t val);

/**
 * @brief Sends an int8_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_int8(char *cmd, uint8_t cmdLength, int8_t val);

/**
 * @brief Sends an uint16_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_uint16(char *cmd, uint8_t cmdLength, uint16_t val);

/**
 * @brief Sends an int16_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_int16(char *cmd, uint8_t cmdLength, int16_t val);

/**
 * @brief Sends an uint32_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_uint32(char *cmd, uint8_t cmdLength, uint32_t val);

/**
 * @brief Sends an int32_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_int32(char *cmd, uint8_t cmdLength, int32_t val);

/**
 * @brief Sends an uint32_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_uint64(char *cmd, uint8_t cmdLength, uint64_t val);

/**
 * @brief Sends an int32_t value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_int64(char *cmd, uint8_t cmdLength, int64_t val);

/**
 * @brief Sends a bool value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_bool(char *cmd, uint8_t cmdLength, bool val);

/**
 * @brief Sends a null-terminated string value to the GUI.
 *
 * @return 0 if success, -1 if failure
 */
extern int guicomm_send_string(char *cmd, uint8_t cmdLength, const char *val);

/**
 * @brief Reads a uint8_t value received from the GUI
 *
 * @return The received uint8_t value
 */
extern uint8_t guicomm_read_uint8(char *string);

/**
 * @brief Reads a int8_t value received from the GUI
 *
 * @return The received int8_t value
 */
extern int8_t guicomm_read_int8(char *string);

/**
 * @brief Reads a uint16_t value received from the GUI
 *
 * @return The received uint16_t value
 */
extern uint16_t guicomm_read_uint16(char *string);

/**
 * @brief Reads a int16_t value received from the GUI
 *
 * @return The received int16_t value
 */
extern int16_t guicomm_read_int16(char *string);

/**
 * @brief Reads a uint32_t value received from the GUI
 *
 * @return The received uint32_t value
 */
extern uint32_t guicomm_read_uint32(char *string);

/**
 * @brief Reads a int32_t value received from the GUI
 *
 * @return The received int32_t value
 */
extern int32_t guicomm_read_int32(char *string);

/**
 * @brief Reads a uint64_t value received from the GUI
 *
 * @return The received uint64_t value
 */
extern uint64_t guicomm_read_uint64(char *string);

/**
 * @brief Reads a int64_t value received from the GUI
 *
 * @return The received int64_t value
 */
extern int64_t guicomm_read_int64(char *string);

/**
 * @brief Reads a bool value received from the GUI
 *
 * @return The received bool value
 */
extern bool guicomm_read_bool(char *string);

/**
 * @brief Reads a float value received from the GUI
 *
 * @return The received float value
 */
extern float guicomm_read_float(char *string);

/**
 * @brief Reads a double value received from the GUI
 *
 * @return The received double value
 */
extern double guicomm_read_double(char *string);

#endif /* ZEPHYR_INCLUDE_TI_GUI_COMPOSER_GUICOMM_JSON_H_ */
