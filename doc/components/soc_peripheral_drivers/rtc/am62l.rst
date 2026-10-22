Overview
********

The AM62L RTC is a two-layer stack. The K3 wakeup-domain hardware peripheral
(`ti,k3-rtc-counter <../../../../../zephyr/build/dts/api/bindings/counter/ti,k3-rtc-counter.html>`_)
exposes a 48-bit, 32 kHz free-running second counter through the Zephyr counter
API. A ``zephyr,rtc-counter`` child node wraps that counter and implements the
standard Zephyr RTC API by storing a Unix-epoch offset on top of the raw
counter value.

Features Supported
******************

- 32-bit and 64-bit time support
- Two independent alarms with callbacks (``CONFIG_RTC_ALARM``)

Features NOT Supported
***********************

- Update callback (``CONFIG_RTC_UPDATE``) — not implemented in the
  ``zephyr,rtc-counter`` driver
- Calibration (``CONFIG_RTC_CALIBRATION``) — not implemented in the
  ``zephyr,rtc-counter`` driver
- Sub-second interval or prescaler timers

Usage
*****

The driver implements Zephyr's standard RTC API (``<zephyr/drivers/rtc.h>``).

Enabling the Driver
===================

Both the underlying K3 counter node (``wkup_rtc0``) and the virtual RTC child
node (``counter_rtc0``) must be enabled. This is already done in the
``am62l_evm`` board DTS; no additional overlay is needed for the default
configuration.

.. code-block:: devicetree

   &wkup_rtc0 {
           status = "okay";
   };

   &counter_rtc0 {
           status = "okay";
   };

.. code-block:: kconfig

   CONFIG_RTC=y
   CONFIG_COUNTER=y

Devicetree Bindings
===================

The node properties are described in the devicetree bindings:

- `ti,k3-rtc-counter <../../../../../zephyr/build/dts/api/bindings/counter/ti,k3-rtc-counter.html>`_
  (hardware counter node)
- `zephyr,rtc-counter <../../../../../zephyr/build/dts/api/bindings/rtc/zephyr,rtc-counter.html>`_
  (virtual RTC child node)

Samples and Tests
*****************

Samples
=======

- `RTC Sample <../../../../../zephyr/samples/drivers/rtc/README.html>`_

  - Standard Zephyr RTC sample; works unchanged on ``am62l_evm``.
  - Uses the ``zephyr,rtc-counter`` wrapper.

- `Counter Alarm Sample <../../../../../zephyr/samples/drivers/counter/alarm/README.html>`_

  - Sets counter alarms on the underlying ``ti,k3-rtc-counter`` device.

Tests
=====

- ``rtc_api`` — Validates the standard Zephyr RTC API (time and alarms).
