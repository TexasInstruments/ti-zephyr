# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.28.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(comp_analog_wakeup)

target_sources(app PRIVATE src/main.c)
