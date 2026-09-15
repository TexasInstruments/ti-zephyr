# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: LicenseRef-TI-TFL

zephyr_library()

zephyr_library_include_directories(
  ${ZEPHYR_TI_ZEPHYR_MODULE_DIR}/include/gui_composer
  ${ZEPHYR_TI_ZEPHYR_MODULE_DIR}/include/gui_composer/mpack
)

zephyr_library_sources(src/backends/transport.c)

zephyr_library_sources_ifdef(CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK
  src/mpack/mpack.c
  src/protocol_mpack/gui_mpack.c
  src/protocol_mpack/guicomm_mpack.c
)

zephyr_library_sources_ifdef(CONFIG_GUI_COMPOSER_PROTOCOL_JSON
  src/protocol_json/gui_json.c
  src/protocol_json/guicomm_json.c
)

zephyr_library_compile_definitions(MAX_STR_LEN=${CONFIG_GUI_COMPOSER_MAX_STR_LEN})
