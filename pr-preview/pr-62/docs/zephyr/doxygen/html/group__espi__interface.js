var group__espi__interface =
[
    [ "eSPI Emulation Interface", "group__espi__emul__interface.html", "group__espi__emul__interface" ],
    [ "eSPI SAF", "group__espi__saf__interface.html", "group__espi__saf__interface" ],
    [ "espi.h", "espi_8h.html", null ],
    [ "espi_evt_data_kbc", "structespi__evt__data__kbc.html", [
      [ "data", "structespi__evt__data__kbc.html#a5f56bd66943cc82825bb1725bf8a2966", null ],
      [ "evt", "structespi__evt__data__kbc.html#a55e94583f10481a2cdc34ad7ba388f2f", null ],
      [ "reserved", "structespi__evt__data__kbc.html#a0c17b320752b11dc04926df2965807b2", null ],
      [ "type", "structespi__evt__data__kbc.html#a75294f9d88f6dfad7bb56e5af5613c64", null ]
    ] ],
    [ "espi_evt_data_acpi", "structespi__evt__data__acpi.html", [
      [ "data", "structespi__evt__data__acpi.html#a8fb3633a97b1af7618b391539f576c6f", null ],
      [ "reserved", "structespi__evt__data__acpi.html#acb291c371bb201d0ceb739fca7e22d3b", null ],
      [ "type", "structespi__evt__data__acpi.html#aa5d1e0b548e9be6b65daf67316df8bfe", null ]
    ] ],
    [ "espi_evt_data_pvt", "structespi__evt__data__pvt.html", [
      [ "data", "structespi__evt__data__pvt.html#a67a1ac452110cfdb0c9c5e0f453c5c57", null ],
      [ "reserved", "structespi__evt__data__pvt.html#aff82e44d2a95e985d1a672169f55e4df", null ],
      [ "type", "structespi__evt__data__pvt.html#adf7edc5acdfb9affde2d1ff1d18cf23d", null ]
    ] ],
    [ "espi_event", "structespi__event.html", [
      [ "evt_data", "structespi__event.html#aa0491cbed5bec091721dfa14898b7277", null ],
      [ "evt_details", "structespi__event.html#aea36925bd3f599c1481c9cc867795c33", null ],
      [ "evt_type", "structespi__event.html#acac75eb1d2b384aa337a5240825b635f", null ]
    ] ],
    [ "espi_cfg", "structespi__cfg.html", [
      [ "channel_caps", "structespi__cfg.html#abb24c88ecc977d8116973d44a7e7cb94", null ],
      [ "io_caps", "structespi__cfg.html#a75175c60a31c3a88c83772b99c2a5c34", null ],
      [ "max_freq", "structespi__cfg.html#aed88dd6fc8028a7c281260e7fa29ce21", null ]
    ] ],
    [ "espi_request_packet", "structespi__request__packet.html", [
      [ "address", "structespi__request__packet.html#a4ac2d50786fdc4c4bd8da88bde28f77d", null ],
      [ "cycle_type", "structespi__request__packet.html#a9b474155dfa0c74d0bd55e4b831b383a", null ],
      [ "data", "structespi__request__packet.html#a6f808763faf949ff7ef83d68fb71e7ba", null ],
      [ "len", "structespi__request__packet.html#ae8b7dc12b04f469b5689ad27cc4f0643", null ],
      [ "tag", "structespi__request__packet.html#a22a0111e338827125eaf79ce8516b744", null ]
    ] ],
    [ "espi_oob_packet", "structespi__oob__packet.html", [
      [ "buf", "structespi__oob__packet.html#a0b19890b5e63ecbd1b95eef57fd9263f", null ],
      [ "len", "structespi__oob__packet.html#adf45a10c88a171df8e7138929958b346", null ]
    ] ],
    [ "espi_flash_packet", "structespi__flash__packet.html", [
      [ "buf", "structespi__flash__packet.html#a22244bc4063618707eea571f170bdfde", null ],
      [ "flash_addr", "structespi__flash__packet.html#ac638c0b5d0d98e15ab66850afc394cec", null ],
      [ "len", "structespi__flash__packet.html#adade51336ae37a519c06b12bf19fc86d", null ]
    ] ],
    [ "ESPI_VWIRE_SIGNAL_OCB_0", "group__espi__interface.html#gae4855b6cf8049de8aa6d67763f1be8c3", null ],
    [ "ESPI_VWIRE_SIGNAL_OCB_1", "group__espi__interface.html#ga2ed490939e0249ff8196f77a74967727", null ],
    [ "ESPI_VWIRE_SIGNAL_OCB_2", "group__espi__interface.html#gaf35d03bc041dc6baefe35b74c163d73b", null ],
    [ "ESPI_VWIRE_SIGNAL_OCB_3", "group__espi__interface.html#gaeae1765d801cd298c27404c173a94707", null ],
    [ "HOST_KBC_EVT_IBF", "group__espi__interface.html#gaa0807c908666cdcbb3a85f310bfbfccc", null ],
    [ "HOST_KBC_EVT_OBE", "group__espi__interface.html#ga807bb75027f24e040cd28eba4bc1002c", null ],
    [ "espi_callback_handler_t", "group__espi__interface.html#ga9d848c7e367cf277230f365f73c15c46", null ],
    [ "espi_bus_event", "group__espi__interface.html#ga36ac3fbf9813f78bad90f047e1eb1128", [
      [ "ESPI_BUS_RESET", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128ae6d4d43eded0d8a368c90ea653a60956", null ],
      [ "ESPI_BUS_EVENT_CHANNEL_READY", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128a996aee0a8f8ba0cead11cea0d39d2973", null ],
      [ "ESPI_BUS_EVENT_VWIRE_RECEIVED", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128aa09f7dcd4b2d3addfaddbad5b8f2e5a2", null ],
      [ "ESPI_BUS_EVENT_OOB_RECEIVED", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128a3b5e529d1d0ba11c59172ab01c30e203", null ],
      [ "ESPI_BUS_PERIPHERAL_NOTIFICATION", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128a9ac837699f302bd10332814c7014adea", null ],
      [ "ESPI_BUS_TAF_NOTIFICATION", "group__espi__interface.html#gga36ac3fbf9813f78bad90f047e1eb1128a9f2a9cfda3b803687a0992a9d41f0014", null ]
    ] ],
    [ "espi_channel", "group__espi__interface.html#gafaa3f7d54503c901ab23bd79a7f8a755", [
      [ "ESPI_CHANNEL_PERIPHERAL", "group__espi__interface.html#ggafaa3f7d54503c901ab23bd79a7f8a755a32f636bbad2618a6b1554656d3b53206", null ],
      [ "ESPI_CHANNEL_VWIRE", "group__espi__interface.html#ggafaa3f7d54503c901ab23bd79a7f8a755ac2e84a127870ff50359cf96528c9a7c6", null ],
      [ "ESPI_CHANNEL_OOB", "group__espi__interface.html#ggafaa3f7d54503c901ab23bd79a7f8a755a0ccb80544cbda501cc90ea0c3b868e54", null ],
      [ "ESPI_CHANNEL_FLASH", "group__espi__interface.html#ggafaa3f7d54503c901ab23bd79a7f8a755a5616bf2bf49a8c457fcc2f27d9fe0518", null ]
    ] ],
    [ "espi_cycle_type", "group__espi__interface.html#ga3e52615f244d7fa8ccda495ab8ec8a5b", [
      [ "ESPI_CYCLE_MEMORY_READ32", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba5b480e093a58849d4181bba22d5868a5", null ],
      [ "ESPI_CYCLE_MEMORY_READ64", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5bac8ec7673b4466b278dfb5e468f7a4d79", null ],
      [ "ESPI_CYCLE_MEMORY_WRITE32", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba2e5a736cf8a63491ee13d51ae4f8f073", null ],
      [ "ESPI_CYCLE_MEMORY_WRITE64", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba54c11e63fcfe3f859af57e65af3f31f7", null ],
      [ "ESPI_CYCLE_MESSAGE_NODATA", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba5f2ae0575ca4e9b61c0acd7d4f08ee82", null ],
      [ "ESPI_CYCLE_MESSAGE_DATA", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba380e3d4e6b5579c8bb94e792fcb7d71f", null ],
      [ "ESPI_CYCLE_OK_COMPLETION_NODATA", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba0335f2d34fab46960939e2d2c1c12e3f", null ],
      [ "ESPI_CYCLE_OKCOMPLETION_DATA", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5baf3dbc2bcc0ed883bc4d4160c21e6934f", null ],
      [ "ESPI_CYCLE_NOK_COMPLETION_NODATA", "group__espi__interface.html#gga3e52615f244d7fa8ccda495ab8ec8a5ba0edb306e5fabcbf8b009375cde6a8a2b", null ]
    ] ],
    [ "espi_io_mode", "group__espi__interface.html#ga0d7c61f1f4ec611d0c8a67ba73e2b4f0", [
      [ "ESPI_IO_MODE_SINGLE_LINE", "group__espi__interface.html#gga0d7c61f1f4ec611d0c8a67ba73e2b4f0a9f47188ebd9bb4cb6e426d0bc0b6595c", null ],
      [ "ESPI_IO_MODE_DUAL_LINES", "group__espi__interface.html#gga0d7c61f1f4ec611d0c8a67ba73e2b4f0af766582d28d16af7c5aff6372e4ee243", null ],
      [ "ESPI_IO_MODE_QUAD_LINES", "group__espi__interface.html#gga0d7c61f1f4ec611d0c8a67ba73e2b4f0a87e70bb0f5c4f6a3a21ca1bcfa4540fc", null ]
    ] ],
    [ "espi_pc_event", "group__espi__interface.html#gac55098842dd9d181144ac4b4122a610e", [
      [ "ESPI_PC_EVT_BUS_CHANNEL_READY", "group__espi__interface.html#ggac55098842dd9d181144ac4b4122a610ea68ecb52b04ffddcc93fbcc77d3643687", null ],
      [ "ESPI_PC_EVT_BUS_MASTER_ENABLE", "group__espi__interface.html#ggac55098842dd9d181144ac4b4122a610ea0e245cbeef479218d4e6c9bec57e40d1", null ]
    ] ],
    [ "espi_virtual_peripheral", "group__espi__interface.html#ga2629a5518a94f031419eeccd05f07373", [
      [ "ESPI_PERIPHERAL_UART", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373a24478711945fd5258d8ca86bbb79b867", null ],
      [ "ESPI_PERIPHERAL_8042_KBC", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373aaea75bb5ab8a5ff31dd6b8787f44df30", null ],
      [ "ESPI_PERIPHERAL_HOST_IO", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373a8948a6f18bfbe87859a529d5f6e669b8", null ],
      [ "ESPI_PERIPHERAL_DEBUG_PORT80", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373af51b7171cf6085fa7946333ff32fe2f0", null ],
      [ "ESPI_PERIPHERAL_HOST_IO_PVT", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373aa217b4786d7b8bacbaa4ec740550e71e", null ],
      [ "ESPI_PERIPHERAL_HOST_IO_PVT2", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373a0e6e9dc1a7975cd80991090a78ea6231", null ],
      [ "ESPI_PERIPHERAL_HOST_IO_PVT3", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373a8d8c64c49e8c7a75cb3a665e63a6f5ce", null ],
      [ "ESPI_PERIPHERAL_EC_HOST_CMD", "group__espi__interface.html#gga2629a5518a94f031419eeccd05f07373ace2e4ef0298e7c67a82dbfcab43769ad", null ]
    ] ],
    [ "espi_vwire_signal", "group__espi__interface.html#gab65a0951a8912d9de398cfec0aef7d35", [
      [ "ESPI_VWIRE_SIGNAL_SLP_S3", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ab880a7156e489d5f7b8f78550e72b166", null ],
      [ "ESPI_VWIRE_SIGNAL_SLP_S4", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a4ef1fe12bd09be0875864d6c32164f73", null ],
      [ "ESPI_VWIRE_SIGNAL_SLP_S5", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ac4b4e30c097a4ef49f21549a799e1e13", null ],
      [ "ESPI_VWIRE_SIGNAL_OOB_RST_WARN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a2f3f97b51b750050cd98e887e39431d1", null ],
      [ "ESPI_VWIRE_SIGNAL_PLTRST", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a0bafcd0bb7f592f0e308dcfce39bcd08", null ],
      [ "ESPI_VWIRE_SIGNAL_SUS_STAT", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a0e13f250ba0c5a37451c7f3b0e98e663", null ],
      [ "ESPI_VWIRE_SIGNAL_NMIOUT", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ad14b4b7a25141346bfefc507c588a238", null ],
      [ "ESPI_VWIRE_SIGNAL_SMIOUT", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a1d54a71c6d2e52cb66cfaa0b55b61152", null ],
      [ "ESPI_VWIRE_SIGNAL_HOST_RST_WARN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35af44250de6f60eabf9b0d85b8860ac56e", null ],
      [ "ESPI_VWIRE_SIGNAL_SLP_A", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aa4c8c69475b48d88302066823d2f3592", null ],
      [ "ESPI_VWIRE_SIGNAL_SUS_PWRDN_ACK", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ab4f0bedd5717660cc8cac2d85c6c3d18", null ],
      [ "ESPI_VWIRE_SIGNAL_SUS_WARN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a9e32eee077aaa1668c356fe1d4ab1cc5", null ],
      [ "ESPI_VWIRE_SIGNAL_SLP_WLAN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a3d95f095ae41bba9ef916fc68ddd4779", null ],
      [ "ESPI_VWIRE_SIGNAL_SLP_LAN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aba8b891a643ca567966d091963be4615", null ],
      [ "ESPI_VWIRE_SIGNAL_HOST_C10", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ab9deb0dd6d4e878eff6413ddc34bb36c", null ],
      [ "ESPI_VWIRE_SIGNAL_DNX_WARN", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a923f3e602d9f8ac821ece2a625183146", null ],
      [ "ESPI_VWIRE_SIGNAL_PME", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aa87af5b5de184238e5f371851aaee692", null ],
      [ "ESPI_VWIRE_SIGNAL_WAKE", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a736724f8229aebc356c2720a34a0143f", null ],
      [ "ESPI_VWIRE_SIGNAL_OOB_RST_ACK", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a8e15df6f8159b4c275dc4b20aaec352e", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_BOOT_STS", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a231caf3d46cde2eaf7305bf3f694838c", null ],
      [ "ESPI_VWIRE_SIGNAL_ERR_NON_FATAL", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a504ae332b89262f70a0bf1a9c8b6f527", null ],
      [ "ESPI_VWIRE_SIGNAL_ERR_FATAL", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aae0a3fd882c5765db33a4e7bd05e1ab1", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ad99ea62f91b40c8926cda944928e3631", null ],
      [ "ESPI_VWIRE_SIGNAL_HOST_RST_ACK", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a89420628ae26df4c45ef7fba04a3a85e", null ],
      [ "ESPI_VWIRE_SIGNAL_RST_CPU_INIT", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a19c40230e0bb4cee7b6ee42580c28a8d", null ],
      [ "ESPI_VWIRE_SIGNAL_SMI", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a2d9b2eef499d1bfd13bec1d2caa0e2f3", null ],
      [ "ESPI_VWIRE_SIGNAL_SCI", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35afa1f99a7e45b306c6736888e280f527c", null ],
      [ "ESPI_VWIRE_SIGNAL_DNX_ACK", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a58280c68c980476aabefa16e0509dd1c", null ],
      [ "ESPI_VWIRE_SIGNAL_SUS_ACK", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35abe28432b566619d8c6cce5e5034e5333", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_0", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aa075b359a98779f42bc9447adc1b27a2", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_1", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ab4e6f770d2a8f8b0f2d2be6c392f92de", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_2", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a5e0bb79530540c15e802c852b5ee944b", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_3", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ac8c015227c6d6c2441ea811faa543b5a", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_4", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ad9b6bb998bea0bc983ab8a6c452d52a1", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_5", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35af10e9e5b320301b2fe923e6793adbeaf", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_6", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35aee0a1576a6e07708e1cde59597b49662", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_7", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ae34962d6db346e02f175894843ce0e1e", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_8", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a8a7b50f0aa53a2c80bae7747d628d219", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_9", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a9410475f349afcf44ddb25ebbd454978", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_10", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a637b5c00aaca05e5daa6969a680cbbb1", null ],
      [ "ESPI_VWIRE_SIGNAL_TARGET_GPIO_11", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35a07a6b00a4b1cfad2137dc43d7232fc9f", null ],
      [ "ESPI_VWIRE_SIGNAL_COUNT", "group__espi__interface.html#ggab65a0951a8912d9de398cfec0aef7d35ad3319083735295454bc59b742988cfca", null ]
    ] ],
    [ "lpc_peripheral_opcode", "group__espi__interface.html#gad00b6a22843e5df9c6d6945d0d82310e", [
      [ "E8042_OBF_HAS_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eac172afb38cb38970ef6242a4f45132f9", null ],
      [ "E8042_IBF_HAS_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eab70684413d736c8583f3dc53f14b7c7a", null ],
      [ "E8042_WRITE_KB_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eaa9135a5ea9a5ebd9d25660c37983c5d6", null ],
      [ "E8042_WRITE_MB_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eaaef0649f867424ec0c7889acb7354911", null ],
      [ "E8042_RESUME_IRQ", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eac5b666b3319dc2ef0fc571684c2ea198", null ],
      [ "E8042_PAUSE_IRQ", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea5e7205b5f1371f1a0f2eaed299389475", null ],
      [ "E8042_CLEAR_OBF", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eacdeb74a9534310f5dab3f1bfb5def534", null ],
      [ "E8042_READ_KB_STS", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea8c62cd1266681c88aa0cb1af65ab551b", null ],
      [ "E8042_SET_FLAG", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea83266d88740588d45be6667a4034c447", null ],
      [ "E8042_CLEAR_FLAG", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea5c592b1cad381b296cde832c67264cd1", null ],
      [ "EACPI_OBF_HAS_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea3cce8bd0205691ecda53fc469606929d", null ],
      [ "EACPI_IBF_HAS_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea09f241a3f6ffcb5b7839c81c7fc4ea6e", null ],
      [ "EACPI_WRITE_CHAR", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea689643f6feb724a303d3b790ff2899f7", null ],
      [ "EACPI_READ_STS", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea02bf8538a66f633e3d2ec094f57c6173", null ],
      [ "EACPI_WRITE_STS", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea02c7b1c69d803a5c45304260786afb6f", null ],
      [ "EACPI_GET_SHARED_MEMORY", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea530fb2a8690538e9659d6eb2c234fb91", null ],
      [ "ECUSTOM_HOST_SUBS_INTERRUPT_EN", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea113a85c90924922dde04f3b5bf64046b", null ],
      [ "ECUSTOM_HOST_CMD_GET_PARAM_MEMORY", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310ea464db427cfd432b44874495ed7503db8", null ],
      [ "ECUSTOM_HOST_CMD_GET_PARAM_MEMORY_SIZE", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eae1e018f0ecde965905752204e557ba40", null ],
      [ "ECUSTOM_HOST_CMD_SEND_RESULT", "group__espi__interface.html#ggad00b6a22843e5df9c6d6945d0d82310eaf97f7d0d3562dba07b826101bb805243", null ]
    ] ],
    [ "espi_add_callback", "group__espi__interface.html#gabf5f0ea01ec8ed5345b2e119181c2313", null ],
    [ "espi_config", "group__espi__interface.html#ga7227c53d384eb0dc666361261f069f68", null ],
    [ "espi_flash_erase", "group__espi__interface.html#gab42be7c76c4523cea96365aa77fd18be", null ],
    [ "espi_get_channel_status", "group__espi__interface.html#ga869551e20fc2c3be4311c21c3c53999d", null ],
    [ "espi_init_callback", "group__espi__interface.html#ga8d88e4e3893d610713195e5352ec2565", null ],
    [ "espi_read_flash", "group__espi__interface.html#ga0a97d11167367342283bfe6b6d66726e", null ],
    [ "espi_read_lpc_request", "group__espi__interface.html#gaeaae20afa90d9d825d80997369f31465", null ],
    [ "espi_read_request", "group__espi__interface.html#ga112f7554ba614c8e5f239b1319b4a763", null ],
    [ "espi_receive_oob", "group__espi__interface.html#ga4b8f4fbf66a2b2ae394e00b16500f70d", null ],
    [ "espi_receive_vwire", "group__espi__interface.html#gaa8bb48b5592c4b49d27c9b8a42432410", null ],
    [ "espi_remove_callback", "group__espi__interface.html#ga7f04f98ea6a4671b821cf6ddf6bbf2a6", null ],
    [ "espi_send_oob", "group__espi__interface.html#ga2557cfc7a38f669d14e9826e3fb0fdee", null ],
    [ "espi_send_vwire", "group__espi__interface.html#gacab2b3bff7d940e71ee1c2a9fdedf782", null ],
    [ "espi_write_flash", "group__espi__interface.html#gab02d46fd690e33875cc1b2433c976891", null ],
    [ "espi_write_lpc_request", "group__espi__interface.html#ga880b6b04824f9f0deea10e8018573b88", null ],
    [ "espi_write_request", "group__espi__interface.html#ga143ed88b1f220f9582c809165fd983fd", null ]
];