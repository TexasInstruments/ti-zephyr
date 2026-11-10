# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: LicenseRef-TI-TFL

zephyr_library()

zephyr_include_directories(
  control/dclink_ss/include
  control/fwc/include
  control/pi/include
  control/pid/include
  control/pr/include
  control/vib_comp/include
  control/vs_freq/include
  filter/filter_fo/include
  filter/filter_so/include
  filter/notch/include
  filter/offset/include
  math/include
  transforms/clarke/include
  transforms/ipark/include
  transforms/park/include
  transforms/svgen/include
  transforms/volts/include
  utilities/angle_gen/include
  utilities/cpu_time/include
  utilities/rampgen/include
  utilities/traj/include
  utilities/types/include
)

zephyr_library_sources(
  control/dclink_ss/source/dclink_ss.c
  control/fwc/source/fwc.c
  control/pi/source/pi.c
  control/pid/source/pid.c
  control/pr/source/pr.c
  control/vib_comp/source/vib_comp.c
  control/vs_freq/source/vs_freq.c
  filter/filter_fo/source/filter_fo.c
  filter/filter_so/source/filter_so.c
  filter/notch/source/filter_notch.c
  filter/offset/source/offset.c
  transforms/ipark/source/ipark.c
  transforms/park/source/park.c
  transforms/svgen/source/svgen_current.c
  transforms/volts/source/volt_calc.c
  transforms/volts/source/volt_recons.c
  utilities/angle_gen/source/angle_gen.c
  utilities/cpu_time/source/cpu_time.c
  utilities/traj/source/traj.c
)
