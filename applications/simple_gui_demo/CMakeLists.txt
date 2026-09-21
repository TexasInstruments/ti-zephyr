#
# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(simple_gui_demo)

target_sources(app PRIVATE src/main.c)
target_sources_ifdef(CONFIG_SAMPLE_SIMPLE_GUI_DEMO_COUNTERS app PRIVATE src/counters.c)
