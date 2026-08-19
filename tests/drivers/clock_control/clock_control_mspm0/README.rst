.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: BSD-3-Clause

TI MSPM0 Clock Control Test
###########################

Exercises the MSPM0 clock control driver: configuring each clock (HFCLK, HSCLK
MCLK, SYSPLL, LFCLK, MFPCLK, CANCLK) onto every source it legally supports,
checking rejected sources return ``-ENOTSUP``, and verifying rates propagate
correctly through mux chains (e.g. MCLK<-HSCLK<-SYSPLL<-HFCLK).

Each test tracks the clocks it touches, and ``mspm0_clock_control_after()``
restores them to their devicetree-default source afterward, so tests don't
leak state into each other. LFCLK is the one exception - once switched off LFOSC
it can't be moved back.

Which sub-tests actually execute depends on what clocks and sources that board's
devicetree wires up.

Supported Boards
****************

- ``lp_mspm33c321a``
- ``lp_mspm0g3519``
- ``lp_mspm0l2228``
