.. SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: LicenseRef-TI-TFL

TMU Performance Comparison
##########################

Overview
********

Measures the cost of each TMU function against the C library equivalent with
the DWT cycle counter and prints a table of cycles per call. The cycle counts
include the loop and an indirect call, which both sides share (about 48 cycles),
so the speedup shown is lower than the raw instruction speedup.

The test fails if a TMU function that replaces a software routine (``sinf``,
``cosf``, ``atanf``, ``atan2f``, ``expf``, ``logf``) is not faster than libm.
``sqrtf`` and ``divf`` are printed but not checked, because the FPU has its own
instructions for them.

Building and Running
********************

.. code-block:: console

   west twister -T tests/lib/mathlib/fastrts_tmu_perf -p lp_am13e230 --device-testing \
       --device-serial /dev/serial/by-id/<XDS110>-if00

Sample output
*************

.. code-block:: none

   function       libm        TMU  speedup
   sinf         213 cy      57 cy    3.74x
   cosf         234 cy      57 cy    4.11x
   atanf        258 cy     102 cy    2.53x
   atan2f       411 cy     134 cy    3.07x
   expf         253 cy      76 cy    3.33x
   logf         306 cy      57 cy    5.37x
   sqrtf         48 cy      51 cy    0.94x
   divf          51 cy      48 cy    1.06x
