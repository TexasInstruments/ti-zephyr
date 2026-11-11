.. ti-socs: am13

.. _ti-zephyr-component-mathlib:

Math Library (TMU)
##################

Overview
********

The AM13 Trigonometric Math Unit (TMU) is a coprocessor on the Cortex-M33,
reached through Arm Custom Datapath Extension (CDE) instructions on
coprocessor 0. It computes single-precision ``sin``, ``cos``, ``atan``,
``sqrt``, ``exp``, ``log`` and divide in hardware. The math library exposes it
through ``<mathlib/fastrts_tmu.h>``, a header-only set of inline functions.

Functions
*********

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Function
     - Notes
   * - ``sinf_tmu()``, ``cosf_tmu()``
     - Input in radians.
   * - ``atanf_tmu()``
     - Result in radians. Valid for any input: the TMU instruction only
       accepts ``|x| <= 1``, so larger inputs are folded in with
       ``atan(x) = +/-pi/2 - atan(1/x)`` at the cost of one divide.
   * - ``atan2f_tmu()``, ``asinf_tmu()``, ``acosf_tmu()``
     - Result in radians, built on the TMU atan and sqrt.
   * - ``sqrtf_tmu()``, ``expf_tmu()``, ``logf_tmu()``, ``divf_tmu()``
     - Direct TMU operations.
   * - ``fastrts_tmu_get_flags()``, ``fastrts_tmu_set_flags()``,
       ``fastrts_tmu_clear_flags()``
     - Read and modify the TMU underflow (``FASTRTS_TMU_FLAGS_LUF``) and
       overflow (``FASTRTS_TMU_FLAGS_LVF``) status flags. ``set`` and ``clear``
       return the flag value *before* the change.

The functions are not remapped over the C library names. Call the ``*_tmu``
variants explicitly.

Accuracy
********

The TMU trades some accuracy for speed. The ztest suite checks every function
against the C library, and the values below were measured on ``lp_am13e230``
with the inputs of the AM13 SDK ``mathlib/fastrts`` example.

* ``logf_tmu()`` matches the C library to within ``1e-5`` absolute error over
  0.0023 to 6.6e8.
* ``expf_tmu()`` is accurate to about ``1e-7`` relative error for inputs up to
  about 2.5 and degrades as the result grows. The relative error against the C
  library is about ``2e-6`` at 3.2, ``1e-5`` at 6.4 and ``1.2e-5`` at 9.2.
* ``expf_tmu()`` is not usable for large inputs. At 13.86 the result is about
  77 % too high (1.85e6 instead of 1.05e6). Keep the input below about 10, or
  use ``expf()`` from the C library.

The SDK example reports the same four ``expf`` inputs (3.2, 6.4, 9.2 and 13.9)
as outside its ``1e-5`` absolute tolerance. This is a limit of the TMU
hardware, not of this library.

Enabling
********

Enable ``CONFIG_TI_MATHLIB_FASTRTS`` (it depends on the AM13E SoC
series and the FPU), and include the header:

.. code-block:: c

   #include <mathlib/fastrts_tmu.h>

   float s = sinf_tmu(0.5f);

The option enables access to coprocessor 0 at boot. The compiler ``cdecp0``
feature comes from ``CONFIG_ARMV8_M_CDE_CP0``, which the AM13E SoC selects.
The CDE intrinsics need a compiler that supports it: the Zephyr SDK GCC does.

Interrupts
**********

The device has vector slots for ``TMU_LUF`` (IRQ 46) and ``TMU_LVF`` (IRQ 47),
but the library does not use them. Check the flags by polling with the flag
helpers, as the SDK example does. An application that wants the interrupts
can connect them itself with ``IRQ_CONNECT()``.

Samples and tests
*****************

* :ref:`fastrts_tmu_sample` compares each function against the C library and
  prints the results.
* ``tests/lib/mathlib/fastrts_tmu`` is a ztest suite that checks each function,
  including negative inputs, all ``atan2f`` quadrants and the flag helpers.
