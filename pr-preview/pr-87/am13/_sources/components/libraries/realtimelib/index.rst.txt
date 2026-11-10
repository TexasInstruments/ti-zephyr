.. ti-socs: am13

.. _ti-zephyr-component-realtimelib:

Real-Time Control Library
#########################

Overview
********

The real-time control library is a set of motor control building blocks:
coordinate transforms, filters, controllers and utilities. It is ported from
the ``realtimelib`` of the AM13 SDK. The modules are plain C with fixed
``float32_t`` interfaces and are used by including their headers.

Enable ``CONFIG_TI_REALTIMELIB`` (it depends on the AM13E SoC series and the
FPU, and selects ``CONFIG_TI_MATHLIB_FASTRTS``):

.. code-block:: c

   #include <park.h>
   #include <pi.h>

   PARK_Obj park_obj;
   PARK_Handle park = PARK_init(&park_obj, sizeof(park_obj));

   PARK_setup(park, rotor_angle_rad);

``datalog`` keeps its four capture buffers in the library only when
``CONFIG_TI_REALTIMELIB_DATALOG`` is enabled (about 6.4 KB of RAM).

``mtpa`` builds two inductance tables from ``CONFIG_TI_REALTIMELIB_MTPA_LS_D_UH``
and ``CONFIG_TI_REALTIMELIB_MTPA_LS_Q_UH`` (in uH, default 1442). They are only
used by ``MTPA_updateLs_d_withLUT()`` and ``MTPA_updateLs_q_withLUT()``.
``MTPA_computeParameters()`` takes its inductances as arguments.

Compared with the original, ``mtpa.h`` returns the q-axis table from
``MTPA_updateLs_q_withLUT()`` (it returned the d-axis table), defines
``MTPA_LUT_DELTA_CURRENT_LQ_A`` (it was defined twice as ``..._LD_A``), and
compares floats with ``0.0f`` instead of ``0.0``.

``motorvision`` exchanges application variables with the MotorVision PC tool
over a serial link. Enable ``CONFIG_TI_REALTIMELIB_MOTORVISION`` (it selects
``CONFIG_UART_INTERRUPT_DRIVEN``). The Zephyr UART driver replaces the register
level UART layer of the SDK solution: ``export.c`` sends with ``uart_poll_out()``
and collects received bytes in the UART interrupt. The application:

* defines the variable table of ``motorvision_hash.h`` (``MVHASH_hash_table``
  and the ``MVHASH_*_startIndex`` values),
* calls ``MOTORVISION_init()`` with the UART device,
* calls ``MOTORVISION_capturePlotData()`` from its control loop and
  ``MOTORVISION_sendPlotData()`` or ``MOTORVISION_captureAndSendPollingData()``
  from a background task.

Sending blocks until each byte is out, so do not call the send functions from the
control interrupt.

Each module has an ``_Obj`` structure that the caller owns, and an ``_init()``
function that takes a pointer to it and its size. ``_init()`` returns ``NULL``
if the buffer is too small.

Modules
*******

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Group
     - Modules
   * - Transforms
     - ``clarke``, ``park``, ``ipark``, ``svgen``, ``volts`` (voltage
       calculation and reconstruction)
   * - Filters
     - ``filter_fo`` (first order), ``filter_so`` (second order), ``notch``,
       ``offset``
   * - Control
     - ``pi``, ``pid``, ``pr`` (proportional resonant), ``fwc`` (field
       weakening), ``vs_freq`` (volts per frequency), ``vib_comp`` (vibration
       compensation), ``dclink_ss`` (DC link single shunt current reconstruction), ``mtpa``
       (maximum torque per ampere)
   * - Utilities
     - ``angle_gen``, ``rampgen``, ``traj`` (trajectory), ``cpu_time``,
       ``datalog`` (``datalog``, ``dlog_2ch_f``, ``dlog_4ch_f``,
       ``dlog_6ch_f``), ``motorvision`` (variable streaming over a UART),
       ``types``
   * - Math
     - ``math_types.h``: shared types, constants and helper macros

TMU acceleration
****************

``math_types.h`` includes ``<mathlib/fastrts_tmu.h>`` and maps ``sinf``,
``cosf``, ``asinf``, ``acosf``, ``atanf``, ``atan2f``, ``expf``, ``logf`` and
``sqrtf`` to the TMU functions, as the SDK does. This applies to code that
includes the library headers, so ``PARK_setup()`` and the other inline
functions run on the TMU.

The TMU is less accurate than the C library for ``expf``: the result is wrong
for large inputs (about 77 % high at 13.86). See :ref:`ti-zephyr-component-mathlib`.
No module of this library calls ``expf`` or ``logf``, but an application that
includes these headers and calls them gets the TMU versions.

Not included
************

The SDK ``realtimelib`` has 33 modules. These are not ported yet:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Modules
     - Reason
   * - ``dead_time_comp``, ``ipd``, ``ti_tune``, ``est_lib`` (FAST),
       ``flux_obs``
     - Delivered as prebuilt libraries only.
   * - ``esmo``, ``speedfr``, ``vsf``
     - Need ``USER_Params``, which is defined with the FAST estimator.
   * - ``vsf``
     - Needs motor constants from the application configuration.
   * - ``datalogIF``
     - Alternative to ``datalog`` selected by ``DATALOGF2_EN``, ``DATALOGF4_EN``
       or ``DATALOGI4_EN``, and it redefines the same types.

Samples and tests
*****************

* ``tests/lib/realtimelib`` is a ztest suite with 58 tests. It checks the
  transforms, the PI, PID and PR controllers, the filters, the volts per
  frequency profile, MTPA, the data logger and the utilities against exact expected values.
  The MotorVision tests run the protocol over an emulated UART: packets in, the
  acknowledges, polling, variable writes and the stream bytes out. For
  ``fwc``, ``vib_comp`` and ``dclink_ss`` it only checks initialisation and
  parameter storage. It runs on hardware because the sine, cosine and square
  root calls use the TMU.

There is no example for this library.
