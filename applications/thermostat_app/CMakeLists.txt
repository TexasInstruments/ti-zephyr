# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: LicenseRef-TI-TFL

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ti_thermostat)

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)

include(${UI_DIR}/file_list_gen.cmake)
include(${UI_DIR}/user_config.cmake)

target_sources(app PRIVATE
    src/main.c
    src/sensor_monitor.c
    ${LV_EDITOR_PROJECT_SOURCES}
)

target_include_directories(app PRIVATE ${UI_DIR})

target_compile_definitions(app PRIVATE LV_LVGL_H_INCLUDE_SIMPLE)
