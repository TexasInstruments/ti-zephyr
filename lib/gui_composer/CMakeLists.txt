# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause

zephyr_library()

zephyr_include_directories(include)

zephyr_library_include_directories(
  include/ti/gui_composer
  include/ti/gui_composer/mpack
  include/ti/gui_composer/jsmn
)

zephyr_library_sources(src/backends/transport.c)

zephyr_library_sources_ifdef(CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK
  src/mpack/mpack.c
  src/protocol_mpack/gui_mpack.c
  src/protocol_mpack/guicomm_mpack.c
)

zephyr_library_sources_ifdef(CONFIG_GUI_COMPOSER_PROTOCOL_JSON
  src/jsmn/jsmn.c
  src/protocol_json/gui_json.c
  src/protocol_json/guicomm_json.c
)

zephyr_library_compile_definitions(MAX_STR_LEN=${CONFIG_GUI_COMPOSER_MAX_STR_LEN})
