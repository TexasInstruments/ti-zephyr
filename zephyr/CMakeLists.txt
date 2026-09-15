# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory_ifdef(CONFIG_GUI_COMPOSER
  ${CMAKE_CURRENT_LIST_DIR}/../lib/gui_composer
  ${CMAKE_CURRENT_BINARY_DIR}/gui_composer
)
