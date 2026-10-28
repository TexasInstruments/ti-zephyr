.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

.. _ti-zephyr-drivers:

drivers/
########

Out-of-tree peripheral drivers for TI silicon.  Each sub-directory targets a
specific subsystem and follows the standard Zephyr driver layout with a ``CMakeLists.txt``, 
``Kconfig`` and C source files. Drivers are registered with the build system via 
``zephyr/module.yml``.
