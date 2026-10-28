SCMI Clock Control
******************

.. important::

   The ``arm,scmi-clock`` driver does not touch any clock register. Every request is a System
   Control and Management Interface (SCMI) message to the platform firmware, which owns the clock
   tree. The messages use the SMC transport (``arm,scmi-smc``) and the IDs in ``clocks`` properties
   are the IDs the firmware expects.

Features Supported
==================

- Clock-control API (``<zephyr/drivers/clock_control.h>``) on the SCMI protocol. The ``clk``
  argument is the SCMI clock ID, cast to ``clock_control_subsys_t``:

  - :c:func:`clock_control_on` and :c:func:`clock_control_off` enable and disable a clock
    (CLOCK_CONFIG_SET)
  - :c:func:`clock_control_get_rate` reads the current rate in Hz (CLOCK_RATE_GET)
  - :c:func:`clock_control_set_rate` requests a new rate passed as a value (CLOCK_RATE_SET). The
    firmware picks the closest rate it can provide (``SCMI_CLK_RATE_SET_FLAGS_ROUNDS_AUTO``).
- SCMI clock protocol helpers (``<zephyr/drivers/firmware/scmi/clk.h>``): rate get and set, parent
  get and set, clock attributes and protocol attributes

Features NOT Supported
======================

- :c:func:`clock_control_get_status` always reports ``CLOCK_CONTROL_STATUS_UNKNOWN``.
  :c:func:`clock_control_configure` and :c:func:`clock_control_async_on` return ``-ENOSYS``.
- Rates are 32 bits wide. :c:func:`clock_control_get_rate` returns the low word of the 64-bit SCMI
  rate, and :c:func:`clock_control_set_rate` sends an upper word of zero.

Usage
=====

Enabling the Driver
-------------------

Set:

.. code-block:: kconfig

   CONFIG_ARM_SCMI=y
   CONFIG_CLOCK_CONTROL=y

``CONFIG_CLOCK_CONTROL_ARM_SCMI`` is enabled by default when an enabled ``arm,scmi-clock`` node
exists, and turns on ``CONFIG_ARM_SCMI_CLK_HELPERS``. ``CONFIG_ARM_SCMI_SMC_TRANSPORT`` and
``CONFIG_ARM_SCMI_SHMEM`` are also enabled by default when their nodes exist. A reply not received
within ``CONFIG_ARM_SCMI_CHAN_SEM_TIMEOUT_USEC`` (500 us by default) fails the request.

Devicetree Configuration
------------------------

The SoC file defines the SCMI agent, the clock protocol node and the transport shared memory:

.. code-block:: devicetree

   firmware {
           scmi {
                   compatible = "arm,scmi-smc";
                   arm,smc-id = <0x82004000>;
                   shmem = <&scmi_shmem>;

                   #address-cells = <1>;
                   #size-cells = <0>;

                   scmi_clk: protocol@14 {
                           compatible = "arm,scmi-clock";
                           reg = <0x14>;
                           #clock-cells = <1>;
                   };
           };
   };

   oc_sram: sram@70800000 {
           compatible = "mmio-sram";
           reg = <0x70800000 DT_SIZE_K(64)>;
           ranges = <0x0 0x70800000 0x10000>;
           #address-cells = <1>;
           #size-cells = <1>;

           scmi_shmem: sram@0 {
                   compatible = "arm,scmi-shmem";
                   reg = <0x0 0x1000>;
           };
   };

Peripherals reference the controller by clock ID:

.. code-block:: devicetree

   main_uart0: serial@2800000 {
           compatible = "ns16550";
           clocks = <&scmi_clk 358>;
   };

Using the Clock Control API
---------------------------

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/drivers/clock_control.h>
   #include <zephyr/sys/printk.h>

   #define MY_CLK_ID 358

   int main(void)
   {
           const struct device *clk = DEVICE_DT_GET(DT_NODELABEL(scmi_clk));
           clock_control_subsys_t id = (clock_control_subsys_t)MY_CLK_ID;
           uint32_t rate;
           int ret;

           if (!device_is_ready(clk)) {
                   return -ENODEV;
           }

           ret = clock_control_on(clk, id);
           if (ret < 0) {
                   return ret;
           }

           ret = clock_control_get_rate(clk, id, &rate);
           if (ret == 0) {
                   printk("clock %d runs at %u Hz\n", MY_CLK_ID, rate);
           }

           return 0;
   }

Devicetree Bindings
===================

- :ws-doc:`arm,scmi-clock <zephyr/build/dts/api/bindings/firmware/arm,scmi-clock.html>`
- :ws-doc:`arm,scmi-smc <zephyr/build/dts/api/bindings/firmware/arm,scmi-smc.html>`
- :ws-doc:`arm,scmi-shmem <zephyr/build/dts/api/bindings/firmware/arm,scmi-shmem.html>`
