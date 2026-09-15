/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: LicenseRef-TI-TFL
 */
#ifndef ZEPHYR_INCLUDE_GUI_COMPOSER_GUI_COMPOSER_H_
#define ZEPHYR_INCLUDE_GUI_COMPOSER_GUI_COMPOSER_H_

#if defined(CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK)
#include <gui_composer/gui_mpack.h>
#include <gui_composer/guicomm_mpack.h>
#elif defined(CONFIG_GUI_COMPOSER_PROTOCOL_JSON)
#include <gui_composer/gui_json.h>
#include <gui_composer/guicomm_json.h>
#endif

#endif /* ZEPHYR_INCLUDE_GUI_COMPOSER_GUI_COMPOSER_H_ */
