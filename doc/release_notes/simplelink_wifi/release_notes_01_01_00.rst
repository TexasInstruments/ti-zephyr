.. _ti-zephyr-cc23-release-01.01.00:

SimpleLink Zephyr v4.4.0-ti-01.01.00 Release
#################################################

Texas Instruments Zephyr GitHub repository is the downstream fork of
`Zephyr <https://github.com/zephyrproject-rtos/zephyr>`__ and is the starting
point for developers wanting to start developing Zephyr applications.

This SimpleLink™ downstream Zephyr release is production grade and the team is
also working on upstreaming this content to mainline Zephyr.

To quickly get started, see the
`Zephyr Trainings on SimpleLink Academy <https://dev.ti.com/tirex/explore/node?node=A__AEIJm0rwIeU.2P1OBWwlaA__SIMPLELINK-ACADEMY-CC23XX__gsUPh5j__LATEST>`__.

Supported Devices
=================

.. list-table::
   :header-rows: 1
   :widths: 35 40 25
   :class: force-wrap

   * - Device
     - Board
     - Quality Level
   * - CC35XXE
     - LP_EM_CC35X1
     - EA

What's New
==========

**hal_ti**


Known Issues and Limitations
=============================

**Zephyr**


.. list-table::
   :header-rows: 1
   :widths: 15 85
   :class: force-wrap

  
Test Coverage
=============

Twister on-device testing (limited) was enabled for:

- ``lp_em_cc35x1``

Test Report
===========

`Zephyr Test Report <https://software-dl.ti.com/simplelink/esd/simplelink_lowpower_zephyr_sdk/4.4.0-ti-01.01.00/exports/Zephyr_test_report.html>`__

Versioning
==========

See the `README <https://github.com/TexasInstruments/simplelink-zephyr?tab=readme-ov-file#versioning>`__
in this repository.

**Operating System Support**

- Windows® 11
- Ubuntu 24.04.4 LTS (64-bit)
- macOS 13.x

Dependencies
============

Zephyr v4.4.0 STS

.. note::
   Flashing a Zephyr 4.4 ``.elf``/``.out`` file containing a TLS header
   segment will fail when using UniFlash or CCS. For CCS (version 21.0) and
   UniFlash (version 9.6.0) or earlier, use a ``.hex`` file instead.

Technical Support and Product Updates
======================================

- `TI SimpleLink Solutions <https://www.ti.com/wireless-connectivity/overview.html>`__
- `TI E2E Community <https://e2e.ti.com/>`__
- `Zephyr Trainings on SimpleLink Academy <https://dev.ti.com/tirex/explore/node?node=A__AEIJm0rwIeU.2P1OBWwlaA__SIMPLELINK-ACADEMY-CC23XX__gsUPh5j__LATEST>`__
- `TI Zephyr RTOS overview <https://www.ti.com/design-development/software-design/Zephyr-RTOS.html>`__
