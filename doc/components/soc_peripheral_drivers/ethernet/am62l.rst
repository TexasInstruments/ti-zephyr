CPSW3G
******

.. important::

   This documentation is experimental.

Features Supported
==================

- The 3-port Gigabit Ethernet Switch (``ti,am62l-cpsw``) through the Zephyr Ethernet L2 and network
  interface APIs. The two external MAC ports each appear as a separate network interface, and the
  internal host port is not described in devicetree
- RGMII (``rgmii``, ``rgmii-id``, ``rgmii-rxid`` and ``rgmii-txid``) and RMII
  (``phy-connection-type``). RGMII ports report 10, 100 and 1000 Mbit/s through
  :c:func:`net_eth_get_hw_capabilities`; RMII ports report 10 and 100 Mbit/s
- PHY management through the CPSW MDIO controller (``ti,am62l-cpsw-mdio``) and the Zephyr MDIO API
  (``<zephyr/drivers/mdio.h>``): :c:func:`mdio_read` and :c:func:`mdio_write`. PHYs are bound with
  ``phy-handle``, and the link speed and duplex reported by the PHY are applied to the MAC port
- DMA based packet transfer through the AM62L PKTDMA driver, with scatter-gather receive and
  transmit of fragmented ``net_pkt`` buffers
- Separate transmit completion and receive threads, with a configurable descriptor count, budget,
  stack size and priority
- MAC addresses from devicetree (``local-mac-address``, ``zephyr,random-mac-address`` or
  ``nvmem-cells``), or from the device eFuse for MAC port 1 when devicetree gives none
- Run-time MAC address change with :c:func:`net_if_set_link_addr`
  (``ETHERNET_CONFIG_TYPE_MAC_ADDRESS``)
- MAC-only ports (``ti,mac-only``), which keep the two ports from switching traffic between each
  other

Features NOT Supported
======================

- Hardware features the driver does not expose:

  - L2 switching between the MAC ports (the driver programs the address lookup engine for MAC-only
    operation)
  - VLAN, quality of service and DSCP priority mapping
  - IEEE 1588 time stamping (CPTS) and time sensitive networking (802.1Qbv, 802.3br)
  - Energy Efficient Ethernet and flow control
  - Several transmit DMA channels; only ``tx0`` is used even though the SoC devicetree lists eight
  - Statistics counters and promiscuous mode

- MII, GMII and SGMII; the TRM lists them as not supported on AM62L
- Device power management.

Usage
=====

Enabling the Driver
-------------------

The application must enable networking and an IP stack. ``CONFIG_ETH_TI_AM62L_CPSW`` is enabled by
default once a ``ti,am62l-cpsw`` node has ``status = "okay"``, and selects ``CONFIG_DMA``.
``CONFIG_MDIO_TI_AM62L_CPSW`` is enabled by default for the MDIO node. The PHY driver for a node
such as ``ti,dp83867`` is enabled the same way.

.. code-block:: kconfig

   CONFIG_NETWORKING=y
   CONFIG_NET_L2_ETHERNET=y
   CONFIG_NET_IPV4=y
   CONFIG_NET_DHCPV4=y

``CONFIG_NET_BUF_DATA_SIZE`` must be at least 64, which the driver checks at build time. If the
application uses both ports with an address family, ``CONFIG_NET_IF_MAX_IPV4_COUNT`` and
``CONFIG_NET_IF_MAX_IPV6_COUNT`` need a slot for each interface.

The following options tune the driver:

.. list-table::
   :header-rows: 1
   :widths: 50 15 35

   * - Option
     - Default
     - Description
   * - ``CONFIG_ETH_TI_AM62L_CPSW_TX_DESC_NUM``
     - 128
     - Transmit descriptors
   * - ``CONFIG_ETH_TI_AM62L_CPSW_RX_DESC_NUM``
     - 128
     - Receive descriptors
   * - ``CONFIG_ETH_TI_AM62L_CPSW_TX_BUDGET``
     - 64
     - Transmit completions handled before the thread yields
   * - ``CONFIG_ETH_TI_AM62L_CPSW_RX_BUDGET``
     - 64
     - Packets received before the thread yields
   * - ``CONFIG_ETH_TI_AM62L_CPSW_TX_DESC_ACQUIRE_TIMEOUT``
     - 100
     - Milliseconds to wait for a free transmit descriptor
   * - ``CONFIG_ETH_TI_AM62L_CPSW_TX_THREAD_STACK_SIZE``
     - 2048
     - Stack of the transmit completion thread
   * - ``CONFIG_ETH_TI_AM62L_CPSW_RX_THREAD_STACK_SIZE``
     - 2048
     - Stack of the receive thread
   * - ``CONFIG_ETH_TI_AM62L_TX_THREAD_PRIO``
     - 2
     - Priority of the transmit completion thread
   * - ``CONFIG_ETH_TI_AM62L_RX_THREAD_PRIO``
     - 2
     - Priority of the receive thread

The SoC devicetree defines the switch, both MAC ports and the MDIO controller as follows:

.. code-block:: devicetree

   cpsw3g: ethernet@8000000 {
           compatible = "ti,am62l-cpsw";
           reg = <0x08000000 0x200000>,
                 <0x091be000 0x8>,
                 <0x43002000 0x8>;
           reg-names = "cpsw_nuss", "gmii_sel", "mac_efuse";
           clocks = <&scmi_clk 9>;
           clock-names = "fck";
           power-domains = <&cpsw3g_pd>;
           dmas = <&pktdma 64 15>, <&pktdma 66 15>, <&pktdma 68 15>, <&pktdma 70 15>,
                  <&pktdma 72 15>, <&pktdma 74 15>, <&pktdma 76 15>, <&pktdma 78 15>,
                  <&pktdma 96 15>;
           dma-names = "tx0", "tx1", "tx2", "tx3", "tx4", "tx5", "tx6", "tx7", "rx";
           status = "disabled";

           ethernet-ports {
                   #address-cells = <1>;
                   #size-cells = <0>;

                   cpsw_port1: port@1 {
                           reg = <1>;
                           phy-connection-type = "rgmii-id";
                           ti,mac-only;
                   };

                   cpsw_port2: port@2 {
                           reg = <2>;
                           zephyr,random-mac-address;
                           phy-connection-type = "rgmii-id";
                           ti,mac-only;
                   };
           };

           cpsw_mdio: mdio {
                   compatible = "ti,am62l-cpsw-mdio";
                   clock-frequency = <1000000>;
                   #address-cells = <1>;
                   #size-cells = <0>;
                   status = "disabled";
           };
   };

The third cell of each ``dmas`` entry is the address select (ASEL) value. ``15`` keeps device
writes from allocating in the L2 cache; ``14`` allocates for cache warming.

Enable the PKTDMA, the switch and the MDIO controller, then bind one PHY to each port. This is
already done in the ``am62l_evm`` board devicetree for the two DP83867 PHYs:

.. code-block:: devicetree

   &pktdma {
           status = "okay";
   };

   &cpsw3g {
           status = "okay";
   };

   &cpsw_port1 {
           phy-handle = <&cpsw_phy1>;
   };

   &cpsw_port2 {
           phy-handle = <&cpsw_phy2>;
   };

   &cpsw_mdio {
           status = "okay";

           cpsw_phy1: ethernet-phy@0 {
                   compatible = "ti,dp83867";
                   reg = <0>;
                   ti,interface-type = "rgmii-id";
                   ti,rx-internal-delay = <7>; /* 2 ns delay */
                   ti,tx-internal-delay = <0>; /* Delay added by MAC */
           };

           cpsw_phy2: ethernet-phy@1 {
                   compatible = "ti,dp83867";
                   reg = <1>;
                   ti,interface-type = "rgmii-id";
                   ti,rx-internal-delay = <7>; /* 2 ns delay */
                   ti,tx-internal-delay = <0>; /* Delay added by MAC */
           };
   };

For ``rgmii-id`` and ``rgmii-txid`` the MAC port adds the transmit delay, so the PHY node is
configured without one.

MAC Address
-----------

For each port the driver first asks devicetree for an address. If MAC port 1 has none, it reads
the address from the eFuse register named ``mac_efuse``. A port without an address logs a warning
and stays without one; use ``zephyr,random-mac-address`` or ``local-mac-address`` on that port.

Sending and Receiving Data
--------------------------

Applications normally use sockets. Each MAC port is a separate network interface, so bind to the
interface to choose a port:

.. code-block:: c

   #include <zephyr/kernel.h>
   #include <zephyr/net/net_if.h>
   #include <zephyr/net/net_ip.h>
   #include <zephyr/net/socket.h>

   int send_hello(struct net_if *iface)
   {
           struct sockaddr_in dst = {
                   .sin_family = AF_INET,
                   .sin_port = htons(4242),
           };
           struct ifreq ifreq = {0};
           int sock, ret;

           net_addr_pton(AF_INET, "192.0.2.2", &dst.sin_addr);

           sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
           if (sock < 0) {
                   return -errno;
           }

           net_if_get_name(iface, ifreq.ifr_name, sizeof(ifreq.ifr_name));
           zsock_setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &ifreq, sizeof(ifreq));

           ret = zsock_sendto(sock, "hello", 5, 0, (struct sockaddr *)&dst, sizeof(dst));

           zsock_close(sock);

           return ret < 0 ? -errno : 0;
   }

Samples
-------

- :ws-doc:`Zperf sample <zephyr/samples/net/zperf/README.html>` (``samples/net/zperf``). The
  ``am62l_evm`` configuration of the sample sizes the network stack and the interface counts for
  both ports

Devicetree Bindings
===================

- :ws-doc:`ti,am62l-cpsw <zephyr/build/dts/api/bindings/ethernet/ti,am62l-cpsw.html>`
- :ws-doc:`ti,am62l-cpsw-mdio <zephyr/build/dts/api/bindings/ethernet/mdio/ti,am62l-cpsw-mdio.html>`
- :ws-doc:`ti,dp83867 <zephyr/build/dts/api/bindings/ethernet/phy/ti,dp83867.html>`
