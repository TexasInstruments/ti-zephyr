# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: LicenseRef-TI-TFL

cmake_minimum_required(VERSION 3.20.0)

# Default the shield so "west build -b lp_mspm33c321a ." works out of the box;
# an explicit --shield still overrides.
if(NOT DEFINED SHIELD)
  set(SHIELD waveshare_3_5_rpi_lcd_f)
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(washing_machine_app)

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)

include(${UI_DIR}/file_list_gen.cmake)
include(${UI_DIR}/user_config.cmake)

target_sources(app PRIVATE
    src/main.c
    ${LV_EDITOR_PROJECT_SOURCES}
)

target_include_directories(app PRIVATE ${UI_DIR})

# The vendored UI headers include "lvgl.h" (flat form); this guard selects it
# over "lvgl/lvgl.h" so it matches what Zephyr's LVGL module puts on the path.
target_compile_definitions(app PRIVATE LV_LVGL_H_INCLUDE_SIMPLE)
