#
# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gui_composer_demo)

target_sources(app PRIVATE src/main.c)

target_sources_ifdef(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_BLINK app PRIVATE src/blink_led.c)
target_sources_ifdef(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_THERMISTOR app PRIVATE src/thermistor.c)
target_sources_ifdef(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_LIGHTSENSOR app PRIVATE src/lightsensor.c)
target_sources_ifdef(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_FUNCTION_GENERATOR app PRIVATE src/function_generator.c)
target_sources_ifdef(CONFIG_SAMPLE_GUI_COMPOSER_DEMO_TEMPERATURE app PRIVATE src/temperature.c)
