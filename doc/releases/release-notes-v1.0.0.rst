.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-release-notes-v1.0.0:

TI Zephyr SDK v1.0.0 Release Notes
####################################

Summary
*******

First release of the TI Zephyr SDK. Establishes the SDK infrastructure,
out-of-box (OOB) sample library, and downstream board staging area for
TI device families.

This release pins to Zephyr RTOS mainline (post-4.4 development branch,
SHA ``40c6582``).

New Features
************

SDK Infrastructure
==================

* ``install.py`` — single-script GitHub install. Performs west workspace
  setup, Python venv creation, optional Zephyr SDK NG toolchain install,
  and optional TI custom OpenOCD build.
* ``scripts/setup_toolchain.py`` — standalone Zephyr SDK NG toolchain
  installer; symlinks existing local install or downloads from GitHub.
* ``scripts/setup_openocd.py`` — builds TI custom OpenOCD (branch
  ``vivian_m0g3519``) required for ``west flash`` on MSPM0 boards.
* ``scripts/env_setup.py`` — environment activation script. Sets
  ``ZEPHYR_BASE``, ``TI_ZEPHYR_BASE``, ``ZEPHYR_SDK_INSTALL_DIR``,
  ``OPENOCD_TCL``, and ``PATH``.
* ``zephyr/module.yml`` — registers ti-zephyr as a Zephyr module
  (``board_root``, ``dts_root``, ``module_ext_root``, ``snippet_root``).
* ``west.yml`` — west manifest with TI downstream fork placeholders for
  Zephyr, HAL/TI, MCUboot, and tools. Easy single-line replacement when
  TI provides official fork URLs.

Boards
======

* ``lp_mspm0g3519`` — MSPM0G3519 LaunchPad (downstream copy for active
  development; upstream board at ``zephyr/boards/ti/lp_mspm0g3519/``).
* ``lp_mspm33c321a`` — MSPM33C321A LaunchPad (in development; SoC support
  pending upstream Zephyr acceptance).

Samples
=======

* ``samples/basic/blinky`` — GPIO LED blink (1 Hz) for TI LaunchPads.
* ``samples/drivers/uart/echo_bot`` — interrupt-driven UART echo.
* ``samples/drivers/adc/adc_read`` — polling ADC channel 0 read.

Tests
=====

* ``tests/run.py`` — multi-board Twister test dispatcher
  (``--board lp_mspm0g3519``).
* ``tests/boards/lp_mspm0g3519/`` — Twister hardware map and runner for
  LP-MSPM0G3519 using custom OpenOCD.

Known Issues and Limitations
*****************************

* MSPM33C321A SoC support is not yet complete. The board definition
  (DTS, Kconfig, defconfig) is a stub; drivers are pending.
* ``west.yml`` uses a development fork (``vvnpais/zephyr``) as an
  INTERIM placeholder until TI provides official downstream fork URLs.
  See ``PLACEHOLDER_TI_ZEPHYR_FORK`` in ``west.yml``.
* The ``lp_mspm0g3519/map.yml`` hardware map contains machine-specific
  paths. Edit before use on a different machine.

Migration Guide
***************

This is the first release; no migration is required.

Zephyr Version
==============

This SDK pins to Zephyr mainline SHA ``40c6582`` (post-v4.4, development
branch heading toward v4.5). Minimum Zephyr SDK NG version: ``1.0.1``.
