USB Device
**********

Features Supported
==================

- Device mode of the Synopsys DWC3 USB 2.0 controller (``snps,dwc3``) with the TI AM62 wrapper
  (``ti,dwc3-am62``), through the Zephyr USB device controller (UDC) driver API and the new USB
  device stack (``CONFIG_USB_DEVICE_STACK_NEXT``)
- High-speed (480 Mbit/s) and full-speed (12 Mbit/s) operation, selected by ``maximum-speed``; the
  link speed that was negotiated is reported to the stack
- Control, bulk, interrupt and isochronous endpoints, up to the number of endpoints named by
  ``num-in-endpoints`` and ``num-out-endpoints``
- Bidirectional control endpoint 0 with a 64 byte maximum packet size
- Wrapper setup of the PHY: PLL reference clock programming from the ``ref`` clock rate,
  through ``ti,syscon-phy-pll-refclk``, and the VBUS sensing divider (``ti,vbus-divider``)
- Transfers over a ring of transfer request blocks, sized with ``CONFIG_UDC_DWC3_TRB_NUM``
- Optional debug shell (``CONFIG_UDC_DWC3_SHELL``)

Features NOT Supported
======================

- Host mode
- Low-speed operation, which the controller does not support in device mode
- Device power management.

Usage
=====

Enabling the Driver
-------------------

``CONFIG_UDC_DWC3`` is enabled by default once an enabled ``snps,dwc3`` node is present, and
``CONFIG_UDC_DWC3_TI_AM62`` is enabled for the ``ti,dwc3-am62`` wrapper. The application selects
the USB device stack and the classes it needs.

.. code-block:: kconfig

   CONFIG_USB_DEVICE_STACK_NEXT=y
   CONFIG_USBD_CDC_ACM_CLASS=y

The following options tune the driver:

.. list-table::
   :header-rows: 1
   :widths: 40 10 50

   * - Option
     - Default
     - Description
   * - ``CONFIG_UDC_DWC3_EVENTS_NUM``
     - 16
     - Entries in the event ring buffer
   * - ``CONFIG_UDC_DWC3_TRB_NUM``
     - 4
     - Transfer request blocks of each endpoint other than the control endpoints, 3 to 32
   * - ``CONFIG_UDC_DWC3_SHELL``
     - n
     - Shell commands to inspect the controller; needs ``CONFIG_SHELL``

The USB samples size the buffer pool and the stack thread for this controller, for example:

.. code-block:: kconfig

   CONFIG_USBD_THREAD_STACK_SIZE=2048
   CONFIG_UDC_BUF_POOL_SIZE=16384

.. code-block:: devicetree

   main_usb0: usb@31000000 {
           compatible = "ti,dwc3-am62", "snps,dwc3";
           reg = <0x31000000 0x50000>,
                 <0x0f900000 0x800>,
                 <0x0f908000 0x400>;
           reg-names = "base", "usbss", "phy";
           interrupts = <GIC_SPI 215 IRQ_TYPE_LEVEL IRQ_DEFAULT_PRIORITY>;
           interrupt-parent = <&gic>;
           clocks = <&scmi_clk 331>;
           clock-names = "ref";
           power-domains = <&main_usb0_pd>;
           maximum-speed = "high-speed";
           num-in-endpoints = <16>;
           num-out-endpoints = <16>;
           num-bidir-endpoints = <1>;
           status = "disabled";
   };

Enable the controller and point it to the PHY register that holds the reference clock rate. The
``zephyr_udc0`` label is what the samples use to find the controller.

.. code-block:: devicetree

   zephyr_udc0: &main_usb0 {
           status = "okay";
           ti,syscon-phy-pll-refclk = <&usb0_phy_ctrl>;
           ti,vbus-divider;
   };

``ti,vbus-divider`` tells the wrapper that VBUS reaches the SoC through a 1/3 divider. Remove it
if the board connects VBUS directly.

Creating a Device
-----------------

The controller is used through the USB device stack. This initializes a device with a CDC ACM
class and enables it:

.. code-block:: c

   #include <zephyr/device.h>
   #include <zephyr/usb/usbd.h>

   USBD_DEVICE_DEFINE(usbd, DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)), 0x2fe3, 0x0001);

   USBD_DESC_LANG_DEFINE(lang);
   USBD_DESC_MANUFACTURER_DEFINE(mfr, "Zephyr Project");
   USBD_DESC_PRODUCT_DEFINE(product, "AM62L USB device");

   USBD_DESC_CONFIG_DEFINE(fs_cfg_desc, "FS Configuration");
   USBD_DESC_CONFIG_DEFINE(hs_cfg_desc, "HS Configuration");

   USBD_CONFIGURATION_DEFINE(fs_config, 0, 100, &fs_cfg_desc);
   USBD_CONFIGURATION_DEFINE(hs_config, 0, 100, &hs_cfg_desc);

   int start_usb(void)
   {
           int ret;

           usbd_add_descriptor(&usbd, &lang);
           usbd_add_descriptor(&usbd, &mfr);
           usbd_add_descriptor(&usbd, &product);

           usbd_add_configuration(&usbd, USBD_SPEED_FS, &fs_config);
           usbd_add_configuration(&usbd, USBD_SPEED_HS, &hs_config);

           usbd_register_all_classes(&usbd, USBD_SPEED_FS, 1, NULL);
           usbd_register_all_classes(&usbd, USBD_SPEED_HS, 1, NULL);

           ret = usbd_init(&usbd);
           if (ret != 0) {
                   return ret;
           }

           return usbd_enable(&usbd);
   }

The samples contain complete versions of this code, including the string descriptors and the
message callback.

Samples
-------

- :ws-doc:`USB CDC ACM sample <zephyr/samples/subsys/usb/cdc_acm/README.html>`
  (``samples/subsys/usb/cdc_acm``)
- :ws-doc:`USB DFU sample <zephyr/samples/subsys/usb/dfu/README.html>`
  (``samples/subsys/usb/dfu``).

Devicetree Bindings
===================

- :ws-doc:`ti,dwc3-am62 <zephyr/build/dts/api/bindings/usb/ti,dwc3-am62.html>`
- :ws-doc:`snps,dwc3 <zephyr/build/dts/api/bindings/usb/snps,dwc3.html>`
