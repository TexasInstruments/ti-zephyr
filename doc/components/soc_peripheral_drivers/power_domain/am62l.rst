SCMI Power Domain
*****************

.. important::

   The ``arm,scmi-power-domain`` driver does not touch any power register. Each power domain is a
   device whose power state is changed by sending System Control and Management Interface (SCMI)
   power domain protocol messages to the platform firmware, which owns the power controller.

Features Supported
==================

- One power domain device per ``arm,scmi-power-domain`` node. ``reg`` is the SCMI power domain ID,
  and the device is the ``power-domains`` provider of the peripherals that use it.
- Zephyr device power management (``<zephyr/pm/device.h>``) actions:

  - resume: the domain is switched on, then its child devices receive the turn-on action
  - suspend: the child devices receive the turn-off action, then the domain is switched off.
  - turn on and turn off: accepted, with no further action

- Powering a domain on or off follows the use count of its consumers when they use device runtime
  power management (:c:func:`pm_device_runtime_get` and :c:func:`pm_device_runtime_put`)
- Power domain protocol helpers (``<zephyr/drivers/firmware/scmi/power.h>``) for code that needs
  direct control: :c:func:`scmi_power_state_set` and :c:func:`scmi_power_state_get`, with the
  ``SCMI_POWER_STATE_GENERIC_ON`` and ``SCMI_POWER_STATE_GENERIC_OFF`` states. The protocol version
  is 3.0.1.

Features NOT Supported
======================

- Protocol features the driver does not expose:

  - power states other than the generic ON and OFF states

- PM device actions other than resume, suspend, turn on and turn off return ``-ENOTSUP``.
- The driver does not read the state back; use :c:func:`scmi_power_state_get` for that.

Usage
=====

Enabling the Driver
-------------------

Power management and power domains have to be enabled by the application:

.. code-block:: kconfig

   CONFIG_PM_DEVICE=y
   CONFIG_POWER_DOMAIN=y
   CONFIG_ARM_SCMI=y

Add ``CONFIG_PM_DEVICE_RUNTIME=y`` to switch domains on and off from the use count of the consumers.
``CONFIG_POWER_DOMAIN_ARM_SCMI`` defaults to ``y`` when an enabled ``arm,scmi-power-domain`` node
exists and the SCMI power protocol (``arm,scmi-power``) is described.

Devicetree Configuration
========================

The power domain protocol node is a child of the SCMI node (see :ti-ref:`SCMI clock control
<ti-zephyr-component-clock-control>`). Each domain is a node with the SCMI domain ID as ``reg``:

.. code-block:: devicetree

   scmi_pds: protocol@11 {
           compatible = "arm,scmi-power";
           reg = <0x11>;
           #power-domain-cells = <1>;
   };

   power-domains {
           #address-cells = <1>;
           #size-cells = <0>;

           main_uart0_pd: power-domain@59 {
                   compatible = "arm,scmi-power-domain";
                   reg = <89>;
                   #power-domain-cells = <0>;
           };
   };

A peripheral names its domain in ``power-domains``:

.. code-block:: devicetree

   main_uart0: serial@2800000 {
           compatible = "ns16550";
           power-domains = <&main_uart0_pd>;
   };

Using a Power Domain
--------------------

A driver that supports device runtime power management gets the domain switched on when it takes a
reference on its device. Application code does the same:

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/pm/device_runtime.h>

   static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(main_uart0));

   void use_uart(void)
   {
           pm_device_runtime_get(dev);   /* domain on, then the device resumes */
           /* ... use the device ... */
           pm_device_runtime_put(dev);   /* device suspends, then the domain can go off */
   }

To control a domain directly, call the protocol helpers:

.. code-block:: c

   #include <zephyr/drivers/firmware/scmi/power.h>

   void domain_on(void)
   {
           struct scmi_power_state_config cfg = {
                   .domain_id = 89,
                   .power_state = SCMI_POWER_STATE_GENERIC_ON,
           };
           uint32_t state;

           scmi_power_state_set(&cfg);
           scmi_power_state_get(89, &state);
   }

Devicetree Bindings
===================

- :ws-doc:`arm,scmi-power-domain
  <zephyr/build/dts/api/bindings/power-domain/arm,scmi-power-domain.html>`
- :ws-doc:`arm,scmi-power <zephyr/build/dts/api/bindings/firmware/arm,scmi-power.html>`
