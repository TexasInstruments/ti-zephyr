# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: LicenseRef-TI-TFL

zephyr_library()

zephyr_include_directories(
  config
  control/dclink_ss/include
  control/fwc/include
  control/mtpa/include
  control/pi/include
  control/pid/include
  control/pr/include
  control/vib_comp/include
  control/vsf/include
  control/vs_freq/include
  filter/filter_fo/include
  filter/filter_so/include
  filter/notch/include
  filter/offset/include
  math/include
  observers/est_lib/include
  observers/esmo/include
  observers/speedfr/include
  transforms/clarke/include
  transforms/ipark/include
  transforms/park/include
  transforms/svgen/include
  transforms/volts/include
  utilities/angle_gen/include
  utilities/cpu_time/include
  utilities/datalog/include
  utilities/rampgen/include
  utilities/traj/include
  utilities/types/include
)

zephyr_library_sources(
  control/dclink_ss/source/dclink_ss.c
  control/fwc/source/fwc.c
  control/mtpa/source/mtpa.c
  control/pi/source/pi.c
  control/pid/source/pid.c
  control/pr/source/pr.c
  control/vib_comp/source/vib_comp.c
  control/vsf/source/vsf.c
  control/vs_freq/source/vs_freq.c
  filter/filter_fo/source/filter_fo.c
  filter/filter_so/source/filter_so.c
  filter/notch/source/filter_notch.c
  filter/offset/source/offset.c
  observers/esmo/source/esmo.c
  observers/speedfr/source/speedfr.c
  transforms/ipark/source/ipark.c
  transforms/park/source/park.c
  transforms/svgen/source/svgen_current.c
  transforms/volts/source/volt_calc.c
  transforms/volts/source/volt_recons.c
  utilities/angle_gen/source/angle_gen.c
  utilities/cpu_time/source/cpu_time.c
  utilities/datalog/source/datalog.c
  utilities/datalog/source/dlog_2ch_f.c
  utilities/datalog/source/dlog_4ch_f.c
  utilities/datalog/source/dlog_6ch_f.c
  utilities/traj/source/traj.c
)

zephyr_library_compile_definitions_ifdef(CONFIG_TI_REALTIMELIB_DATALOG DATALOG_ENABLE)

# datalog.c keeps the TI "#pragma DATA_SECTION" lines, which GCC does not know
set_source_files_properties(utilities/datalog/source/datalog.c
  PROPERTIES COMPILE_OPTIONS -Wno-unknown-pragmas)

if(CONFIG_TI_REALTIMELIB_MOTORVISION)
  zephyr_include_directories(utilities/motorvision/include)
  zephyr_library_sources(
    utilities/motorvision/source/export.c
    utilities/motorvision/source/export_package.c
    utilities/motorvision/source/motorvision.c
    utilities/motorvision/source/motorvision_sine_generator.c
  )
  zephyr_library_include_directories(utilities/motorvision/source)
endif()

# Prebuilt libraries (GCC, Cortex-M33). They are linked only when enabled.
if(CONFIG_TI_REALTIMELIB_FAST)
  # est.h selects the library variant with this macro
  zephyr_compile_definitions(_FULL_FAST_LIB)
  zephyr_library_import(fast_full_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/observers/est_lib/lib/fast_full_lib_am13e230x_m33_gcc_arm.a)
endif()

if(CONFIG_TI_REALTIMELIB_FLUX_OBS)
  zephyr_include_directories(observers/flux_obs/include)
  zephyr_library_import(flux_obs_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/observers/flux_obs/lib/flux_obs_lib_am13e230x_m33_gcc_arm.a)
endif()

if(CONFIG_TI_REALTIMELIB_TI_TUNE)
  zephyr_include_directories(control/ti_tune/include)
  zephyr_library_import(titune_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/control/ti_tune/lib/titune_lib_am13e230x_m33_gcc_arm.a)
endif()

if(CONFIG_TI_REALTIMELIB_DEAD_TIME_COMP)
  zephyr_include_directories(control/dead_time_comp/include)
  zephyr_library_import(dead_time_comp_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/control/dead_time_comp/lib/dead_time_comp_lib_am13e230x_m33_gcc_arm.a)
endif()
