.. important::

   The Peripheral will only work for AM62L devices (``am62l_evm``).

Overview
********

The AM62L watchdog is the Real Time Interrupt (RTI) module of the K3 family
(:ws-doc:`ti,j7-rti-wdt <zephyr/build/dts/api/bindings/watchdog/ti,j7-rti-wdt.html>`).
It is a digital windowed watchdog and is exposed through Zephyr's standard
watchdog API (``<zephyr/drivers/watchdog.h>``) by the ``CONFIG_WDT_TI_RTI``
driver.

The AM62L devicetree describes two instances, ``main_rti0`` and ``main_rti1``,
both clocked at 25 MHz. ``am62l_evm`` enables ``main_rti0`` and points the
``watchdog0`` alias at it.

Features Supported
******************

- Standard API: :c:func:`wdt_install_timeout`, :c:func:`wdt_setup`,
  :c:func:`wdt_feed` and :c:func:`wdt_disable`
- Reset mode: a timeout resets the SoC (``WDT_FLAG_RESET_SOC``)
- Windowed operation: ``window.min`` can be non-zero, and feeding before the
  window opens is a violation just like a timeout (see
  `Timeout and Window`_)
- Pausing the watchdog while the CPU is halted by a debugger
  (``WDT_OPT_PAUSE_HALTED_BY_DBG``)
- Disabling the watchdog at runtime with :c:func:`wdt_disable`

Features NOT Supported
***********************

- NMI mode and the pre-reset callback. The driver only supports NMI when the
  node has an ``interrupts`` property, which the AM62L RTI nodes do not.
  Installing a timeout without ``WDT_FLAG_RESET_SOC`` returns ``-ENOTSUP``, and
  a ``callback`` is stored but never called.
- More than one timeout channel. Only channel 0 exists, and a second
  :c:func:`wdt_install_timeout` call replaces the first one.

Usage
*****

Enabling the Driver
===================

``am62l_evm`` already enables ``main_rti0``, and ``CONFIG_WDT_TI_RTI`` is
selected automatically once an enabled ``ti,j7-rti-wdt`` node is present. For
another board or for the second instance, enable the node and make it the
``watchdog0`` alias in the board devicetree (or an application overlay), and
turn the watchdog subsystem on in ``prj.conf``.

.. code-block:: devicetree

   / {
           aliases {
                   watchdog0 = &main_rti1;
           };
   };

   &main_rti1 {
           status = "okay";
   };

.. code-block:: kconfig

   CONFIG_WATCHDOG=y

Devicetree Bindings
===================

The node properties are described in
:ws-doc:`ti,j7-rti-wdt <zephyr/build/dts/api/bindings/watchdog/ti,j7-rti-wdt.html>`
devicetree binding.

Timeout and Window
==================

The timeout is the ``window.max`` field of :c:struct:`wdt_timeout_cfg`, in
milliseconds. The driver converts it to the RTI preload value, which limits the
longest timeout at the 25 MHz clock to about **1342 ms**; a larger value makes
:c:func:`wdt_install_timeout` return ``-EINVAL``.

The window is fixed to a fraction of the timeout, so ``window.min`` cannot be
chosen freely. Feeding is only accepted during the last part of the timeout:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Open window (share of ``window.max``)
     - ``window.min``
   * - 100%
     - ``0``
   * - 50%
     - ``window.max / 2``
   * - 25%
     - ``window.max * 3 / 4``
   * - 12.5%
     - ``window.max * 7 / 8``
   * - 6.25%
     - ``window.max * 15 / 16``
   * - 3.125%
     - ``window.max * 31 / 32``

``window.min`` must match one of these values exactly, otherwise
:c:func:`wdt_install_timeout` returns ``-EINVAL``. ``window.min = 0``, as used
by the upstream sample, gives the plain (non-windowed) behavior.

Using the Watchdog
==================

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/watchdog.h>

   static const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

   int main(void)
   {
           struct wdt_timeout_cfg cfg = {
                   .window.min = 0,
                   .window.max = 1000,
                   .flags = WDT_FLAG_RESET_SOC,
           };
           int channel;

           if (!device_is_ready(wdt)) {
                   return -ENODEV;
           }

           channel = wdt_install_timeout(wdt, &cfg);
           if (channel < 0) {
                   return channel;
           }

           wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG);

           while (1) {
                   /* do work, then feed before 1000 ms elapse */
                   wdt_feed(wdt, channel);
                   k_msleep(500);
           }
   }

Samples and Tests
*****************

Samples
=======

- :ws-doc:`Watchdog Driver Sample <zephyr/samples/drivers/watchdog/README.html>`

  - Standard Zephyr watchdog sample; it builds for ``am62l_evm`` through the
    ``watchdog0`` alias.
  - Feeds the watchdog a few times, then stops feeding and waits for the reset.

Tests
=====

- ``drivers.watchdog`` (``tests/drivers/watchdog/wdt_basic_api``) - Validates
  the standard Zephyr watchdog API (timeout installation, setup, feed, and
  reset on expiry).
