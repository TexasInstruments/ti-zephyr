#
# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: LicenseRef-TI-TFL
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(rtc_temp_lcd)
target_sources(app PRIVATE src/main.c)

# Seed for the RTC: Unix time (UTC) of the last CMake configure
string(TIMESTAMP BUILD_UNIX_TIME "%s" UTC)
target_compile_definitions(app PRIVATE BUILD_UNIX_TIME=${BUILD_UNIX_TIME}LL)
