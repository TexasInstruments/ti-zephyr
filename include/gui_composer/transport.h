/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#ifndef ZEPHYR_INCLUDE_TI_GUI_COMPOSER_TRANSPORT_H_
#define ZEPHYR_INCLUDE_TI_GUI_COMPOSER_TRANSPORT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef bool (*guicomm_rx_char_callback_t)(char c);

extern void gui_composer_transport_init(void);

extern void gui_composer_transport_start(guicomm_rx_char_callback_t rx_char_cb);

extern void gui_composer_transport_send_char(char character);

#endif /* ZEPHYR_INCLUDE_TI_GUI_COMPOSER_TRANSPORT_H_ */
