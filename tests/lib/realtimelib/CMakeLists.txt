# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: LicenseRef-TI-TFL

cmake_minimum_required(VERSION 3.28.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(realtimelib)

target_sources(app PRIVATE
  src/main.c
  src/test_transforms_pi_filter.c
  src/test_utilities.c
  src/test_filters.c
  src/test_controllers.c
  src/test_datalog.c
)
