# Copyright (c) 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(clock_control_mspm0)

target_sources(app PRIVATE src/main.c)
