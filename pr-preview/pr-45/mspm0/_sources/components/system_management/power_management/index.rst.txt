.. ti-socs: mspm0

Power Management
================

Zephyr provides two mechanisms for reducing power consumption:

* **System Power Management:** Places the MCU in a lower-power operating mode
  when the CPU is idle.
* **Device Runtime Power Management:** Suspends individual peripherals when
  they are not in use and resumes them when needed.

Texas Instruments (TI) peripheral drivers manage supported hardware power
transitions. Applications and dependent drivers can configure additional power
management requirements.

Features Supported
------------------

* Device Runtime Power Management for supported MSPM0 peripheral drivers.
* System power policy locks to prevent entry into operating modes incompatible
  with an active peripheral.
* The ``sleep`` pinctrl state for configuring peripheral pins during suspension.
* Peripheral operation in MSPM0 RUN and SLEEP modes, as shown in the
  `Supported Operating Modes`_ table.

Features NOT Supported
----------------------

* Peripheral-initiated wake-up from STOP or STANDBY through the listed drivers.
  For example, incoming UART, I2C, or SPI activity cannot wake the MCU from
  these modes through these drivers.
* Active peripheral operation in STOP or STANDBY through the listed drivers.
* Automatic recalculation of peripheral timing when clock frequencies change.

Usage
-----

System Power States
~~~~~~~~~~~~~~~~~~~

MSPM0 microcontrollers support operating modes with different performance and
power consumption characteristics. Zephyr maps these modes to system power
states, which its power management policy can select when the CPU is idle.
Multiple MSPM0 operating modes can map to the same Zephyr power state.

.. list-table:: MSPM0 Operating Modes and Zephyr Power States
   :header-rows: 1
   :widths: 35 65

   * - Zephyr Power State
     - MSPM0 Operating Modes
   * - ``PM_STATE_RUNTIME_IDLE``
     - RUN0/SLEEP0, RUN1/SLEEP1, RUN2/SLEEP2
   * - ``PM_STATE_SUSPEND_TO_IDLE``
     - STOP0, STOP1, STOP2
   * - ``PM_STATE_STANDBY``
     - STANDBY0, STANDBY1

.. note::

   Available operating modes vary by MSPM0 device. Refer to the device
   datasheet for details.

Tickless Kernel
~~~~~~~~~~~~~~~

Enabling ``CONFIG_TICKLESS_KERNEL`` can reduce power consumption by avoiding
unnecessary periodic kernel timer interrupts.

Without tickless operation, the MCU may wake on every kernel tick, even when no
application work is pending, and then return to a low-power state. These
repeated transitions increase power consumption.

With tickless operation, Zephyr schedules timer interrupts around upcoming
events, allowing the MCU to remain in a low-power state for longer periods.
Applications should enable this option when supported. Wake-up timing remains
subject to hardware timer resolution, clock accuracy, and interrupt latency.
Other enabled interrupts can still wake the MCU.

Device Runtime Power Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Device Runtime PM suspends idle peripherals and resumes them when needed. When
enabled, supported TI drivers automatically manage these transitions during
normal peripheral operations.

Asynchronous Communication
^^^^^^^^^^^^^^^^^^^^^^^^^^

Some communication peripherals can receive data asynchronously, even when the
CPU is idle. To prevent data loss, supported TI drivers keep a peripheral
active while asynchronous reception is enabled. For interfaces with APIs to
start and stop reception, the peripheral can suspend after reception is
disabled.

Applications should disable reception when incoming data is no longer needed
so the peripheral can suspend and conserve power.

Minimizing Suspend and Resume Overhead
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Suspending and resuming a peripheral requires CPU processing time and may add
latency. Applications or dependent drivers performing frequent transactions
can keep a peripheral active across multiple operations using:

* ``pm_device_runtime_get()``: Requests the peripheral and resumes it if needed.
* ``pm_device_runtime_put()``: Releases the request, allowing the peripheral
  to suspend when no longer in use.

For example, an application can call ``pm_device_runtime_get()`` before a
series of SPI transactions and ``pm_device_runtime_put()`` afterward to avoid
unnecessary power transitions. These additional requests are optional for
normal peripheral usage and must be balanced.

Sleep Pin Control
^^^^^^^^^^^^^^^^^

Supported TI drivers provide a ``sleep`` pinctrl state for configuring
peripheral pins while the peripheral is suspended. The driver applies the
configured ``sleep`` state on suspension and restores the ``default`` state
on resume.

Applications can define a ``sleep`` state to place pins in a known condition,
reduce noise from floating signals, and minimize power consumption.

The following Devicetree overlay configures UART0 TX and RX as GPIO inputs
with pull-down and pull-up resistors while the peripheral is suspended:

.. code-block:: dts

   &pinctrl {
       uart0_tx_pa10_sleep: uart0_tx_pa10_sleep {
           pinmux = <MSP_PINMUX(21, MSPM0_PIN_FUNCTION_GPIO)>;
           input-enable;
           bias-pull-down;
       };

       uart0_rx_pa11_sleep: uart0_rx_pa11_sleep {
           pinmux = <MSP_PINMUX(22, MSPM0_PIN_FUNCTION_GPIO)>;
           input-enable;
           bias-pull-up;
       };
   };

   &uart0 {
       status = "okay";

       pinctrl-0 = <&uart0_tx_pa10 &uart0_rx_pa11>;
       pinctrl-1 = <&uart0_tx_pa10_sleep &uart0_rx_pa11_sleep>;
       pinctrl-names = "default", "sleep";
   };

* ``default``: Pin configuration during normal peripheral operation.
* ``sleep``: Pin configuration while the peripheral is suspended.

.. note::

   Choose pin settings based on the external circuit to avoid signal conflicts
   or unwanted current consumption. A peripheral's ``sleep`` pinctrl state
   refers to peripheral suspension; it does not necessarily mean that the MCU
   has entered its SLEEP operating mode.

Peripheral Clock Requirements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Peripheral clock availability and supported frequencies can vary between MCU
operating modes. TI peripheral drivers do not currently recalculate peripheral
timing when clock frequencies change.

Applications that require a peripheral to operate in a low-power mode must
ensure its source clock remains available at the frequency used during
configuration. Refer to the device datasheet for clock details.

System Power-State Restrictions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some system power states disable clocks or resources required by an active
peripheral, potentially interrupting its operation. Supported TI drivers use
system power policy locks and the ``zephyr,disabling-power-states`` Devicetree
property to prevent entry into incompatible states while the peripheral
requires those resources.

Applications can extend these restrictions through a Devicetree overlay.
Existing restrictions that are still needed must be preserved when overriding
the property.

Enabling Power Management
-------------------------

Power management features are configured through Kconfig and Devicetree.

Kconfig Options
~~~~~~~~~~~~~~~

The following options can be configured in the application's ``prj.conf``:

.. list-table:: Kconfig Options
   :header-rows: 1
   :widths: 50 50

   * - Kconfig Option
     - Description
   * - ``CONFIG_PM``
     - Enables system power management.
   * - ``CONFIG_TICKLESS_KERNEL``
     - Avoids unnecessary periodic kernel timer interrupts.
   * - ``CONFIG_PM_DEVICE``
     - Enables device power management support.
   * - ``CONFIG_PM_DEVICE_RUNTIME``
     - Enables Device Runtime PM.
   * - ``CONFIG_PM_DEVICE_RUNTIME_DEFAULT_ENABLE``
     - Enables runtime PM by default for supported devices.
   * - ``CONFIG_PM_POLICY_DEVICE_CONSTRAINTS``
     - Enables device-specific system power-state restrictions.

For example:

.. code-block:: ini

   CONFIG_PM=y
   CONFIG_TICKLESS_KERNEL=y
   CONFIG_PM_DEVICE=y
   CONFIG_PM_DEVICE_RUNTIME=y
   CONFIG_PM_POLICY_DEVICE_CONSTRAINTS=y

To enable runtime PM globally for supported devices, also set:

.. code-block:: ini

   CONFIG_PM_DEVICE_RUNTIME_DEFAULT_ENABLE=y

Alternatively, runtime PM can be enabled for individual peripherals through
Devicetree or programmatically.

Devicetree Properties
~~~~~~~~~~~~~~~~~~~~~

.. list-table:: Devicetree Properties
   :header-rows: 1
   :widths: 48 52

   * - Property
     - Description
   * - ``zephyr,pm-device-runtime-auto``
     - Automatically enables runtime PM for the peripheral after initialization.
   * - ``zephyr,disabling-power-states``
     - Identifies system power states that must be avoided while the peripheral
       requires resources unavailable in those states.

To enable runtime PM for UART0 in a Devicetree overlay:

.. code-block:: dts

   &uart0 {
       status = "okay";
       zephyr,pm-device-runtime-auto;
   };

Supported TI drivers handle the corresponding power management operations.

Supported Hardware and Operating Modes
--------------------------------------

MSPM0 Product Families
~~~~~~~~~~~~~~~~~~~~~~

The TI drivers in the table below support Device Runtime PM, system power
policy locks, and the ``sleep`` pinctrl state across these product families:

**MSPM0G:** MSPM0G110x, MSPM0G150x, MSPM0G310x, MSPM0G350x, MSPM0Gx51x

**MSPM0L:** MSPM0L110x, MSPM0L111x, MSPM0L122x, MSPM0L222x, MSPM0L130x

**Primary validation platforms:** MSPM0G3507 LaunchPad, MSPM0G3519 LaunchPad,
and MSPM0L2228 LaunchPad.

Validation is primarily performed on these three development boards. Other
devices in the listed families may differ in clock configurations, operating
modes, and peripheral capabilities.

Supported Operating Modes
~~~~~~~~~~~~~~~~~~~~~~~~~

The following table shows the MSPM0 operating modes supported by each driver
when using Device Runtime PM and system power policy locks.

.. list-table:: Supported Operating Modes
   :header-rows: 1
   :widths: 48 13 13 13 13

   * - Peripheral Driver
     - RUN
     - SLEEP
     - STOP
     - STANDBY
   * - UART (``uart_mspm0``)
     - X
     - X
     -
     -
   * - I2C (``i2c_mspm0``)
     - X
     - X
     -
     -
   * - SPI (``spi_mspm0``)
     - X
     - X
     -
     -
   * - ADC (``adc_mspm0_adc12``)
     - X
     - X
     -
     -

**X** indicates supported peripheral operation in the specified mode with
Device Runtime PM and system power policy locks. Blank cells indicate modes
not currently supported for peripheral operation under these features. The
MCU may still enter those modes when the peripheral is inactive or suspended.

.. note::

   Peripheral-initiated wake-up is not currently supported by these drivers.
   Incoming UART, I2C, or SPI traffic cannot wake the MCU from STOP or
   STANDBY through these drivers. Other supported wake-up sources may still
   be used.

Zephyr Documentation
--------------------

* `Power Management Overview <../../../../../zephyr/services/pm/index.html>`_
* `System Power Management <../../../../../zephyr/services/pm/system.html>`_
* `Device Power Management <../../../../../zephyr/services/pm/device.html>`_
* `Device Runtime Power Management <../../../../../zephyr/services/pm/device_runtime.html>`_
* `System Timer Drivers <../../../../../zephyr/kernel/services/timing/clocks.html#timer-drivers>`_
* `Pin Control <../../../../../zephyr/hardware/pinctrl/index.html>`_
* `Device Runtime PM API <../../../../../zephyr/doxygen/html/group__subsys__pm__device__runtime.html>`_
* `System Power Policy API <../../../../../zephyr/doxygen/html/group__subsys__pm__sys__policy.html>`_
* `Kconfig Reference <../../../../../zephyr/kconfig.html>`_
