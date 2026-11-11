.. _fastrts_tmu_sample:

TMU Fast Math
#############

Overview
********

This sample calls the AM13 Trigonometric Math Unit (TMU) functions from
``<mathlib/fastrts_tmu.h>`` and compares each result with the C library. It
covers sine, cosine, arctangent (including inputs outside ``[-1, 1]``),
arcsine, arccosine, square root, exponential, logarithm, ``atan2`` and divide.

A case passes when the relative error is at most ``1e-3``. The sample prints one
line per case, the TMU status flags, and a final ``Overall Result``.

Building and Running
********************

.. code-block:: console

   west build -p always -b lp_am13e230 ti-zephyr/samples/mathlib/fastrts_tmu
   west flash

Sample Output
*************

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   PASS sinf(0.5f, 0.0f)   tmu=0.479426 ref=0.479426 rel_err=2.980232e-08
   PASS atanf(-3.0f, 0.0f) tmu=-1.249046 ref=-1.249046 rel_err=9.544029e-08
   PASS divf(7.0f, 2.0f)   tmu=3.500000 ref=3.500000 rel_err=0.000000e+00
   ...
   TMU flags: 0x0
   Passed 23 of 23
   Overall Result: PASS
