var group__gnss__interface =
[
    [ "GNSS Driver Backend API", "group__gnss__interface__backend.html", "group__gnss__interface__backend" ],
    [ "gnss.h", "gnss_8h.html", null ],
    [ "gnss_info", "structgnss__info.html", [
      [ "fix_quality", "structgnss__info.html#a54396dd23fd05185bcb1104ee529984e", null ],
      [ "fix_status", "structgnss__info.html#a718e95833c4d07ad530669035ca21f8c", null ],
      [ "geoid_separation", "structgnss__info.html#a5f87d86da1ea290fb3951fab1f95459d", null ],
      [ "hdop", "structgnss__info.html#a5de15dd2a9843c05b8805d4519b49a73", null ],
      [ "satellites_cnt", "structgnss__info.html#a81160955c4b9d3a746280d88a74f07eb", null ]
    ] ],
    [ "gnss_time", "structgnss__time.html", [
      [ "century_year", "structgnss__time.html#a4daa514f38a7b7d88bf49cb5d664b2ac", null ],
      [ "hour", "structgnss__time.html#a5c8a280736741d8d07dacba30d48ed6f", null ],
      [ "millisecond", "structgnss__time.html#a2ab6b41452491ec34966fda0ddbdd9da", null ],
      [ "minute", "structgnss__time.html#a3e8b96861e45cc2a6222961e92ab62f5", null ],
      [ "month", "structgnss__time.html#a1c6730dc08cd3dfb48aee08154f5f2de", null ],
      [ "month_day", "structgnss__time.html#a766744149009588f83cca930ef53ff10", null ]
    ] ],
    [ "gnss_data", "structgnss__data.html", [
      [ "info", "structgnss__data.html#a78155e6d2c981b9121b2ee42c1700988", null ],
      [ "nav_data", "structgnss__data.html#a2470031921179cb30172441163d80052", null ],
      [ "utc", "structgnss__data.html#ae6bfc0c6a53901963a1d20bad025fb99", null ]
    ] ],
    [ "gnss_data_callback", "structgnss__data__callback.html", [
      [ "callback", "structgnss__data__callback.html#a07e9c7fd6192fd5b58ff37aabdbff11e", null ],
      [ "dev", "structgnss__data__callback.html#afdf273ceed2d2038b6415840f9ecdd7b", null ]
    ] ],
    [ "gnss_satellite", "structgnss__satellite.html", [
      [ "azimuth", "structgnss__satellite.html#a27a864422f5207b5d38efbd8b50893a3", null ],
      [ "elevation", "structgnss__satellite.html#a3d99c5ad18242a42f95b5de496ca501d", null ],
      [ "is_corrected", "structgnss__satellite.html#a4296fad8444a7ebcd938f7550332ec52", null ],
      [ "is_tracked", "structgnss__satellite.html#ae4c596c298ed158a188ea4a5fbd6181c", null ],
      [ "prn", "structgnss__satellite.html#a03543a7425f27c78e743b0ec180ccea7", null ],
      [ "snr", "structgnss__satellite.html#a6818083aad878aa8b06d2cbeca53b1da", null ],
      [ "system", "structgnss__satellite.html#ad9b31c85dd0e162979d384b049142549", null ]
    ] ],
    [ "gnss_satellites_callback", "structgnss__satellites__callback.html", [
      [ "callback", "structgnss__satellites__callback.html#af12fc64ff9c696b5862b5adfef31269b", null ],
      [ "dev", "structgnss__satellites__callback.html#a19e64ff2450c00080ae39528f7d07b2b", null ]
    ] ],
    [ "GNSS_DATA_CALLBACK_DEFINE", "group__gnss__interface.html#gae9c3fd5804bd6f8e6790cb1b7e47e4a6", null ],
    [ "GNSS_DT_DATA_CALLBACK_DEFINE", "group__gnss__interface.html#ga00d744003c1c8f71f06a685ecb8d07b1", null ],
    [ "GNSS_DT_SATELLITES_CALLBACK_DEFINE", "group__gnss__interface.html#ga794cc5cadc7a7f470b6f57395bf333f1", null ],
    [ "GNSS_SATELLITES_CALLBACK_DEFINE", "group__gnss__interface.html#ga414a414635e1cd00ef800edaf70bc234", null ],
    [ "gnss_data_callback_t", "group__gnss__interface.html#ga025c965369275ad8e5ab5ad44c14a23b", null ],
    [ "gnss_satellites_callback_t", "group__gnss__interface.html#ga80cf700468d1c942cfa064368e212e6f", null ],
    [ "gnss_systems_t", "group__gnss__interface.html#ga731907f9ae501909bf26ecae5441a5ce", null ],
    [ "gnss_fix_quality", "group__gnss__interface.html#gacf5a3464c487ae7609d526c73ccc758a", [
      [ "GNSS_FIX_QUALITY_INVALID", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aa67256fb63beab67ba6e7cbb63916af30", null ],
      [ "GNSS_FIX_QUALITY_GNSS_SPS", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aa316ea7e84abca8f04bbf95d3d73cea3f", null ],
      [ "GNSS_FIX_QUALITY_DGNSS", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aa65df312eef5cd8b24384440629f534f8", null ],
      [ "GNSS_FIX_QUALITY_GNSS_PPS", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aab4de07a3891e7c89d5bd23fec7add20f", null ],
      [ "GNSS_FIX_QUALITY_RTK", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aa5256b22bb56e93c788d52a641c2a15a6", null ],
      [ "GNSS_FIX_QUALITY_FLOAT_RTK", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aaedd76bb21200df49fd82243d4a394ac6", null ],
      [ "GNSS_FIX_QUALITY_ESTIMATED", "group__gnss__interface.html#ggacf5a3464c487ae7609d526c73ccc758aa851d88a47c1314eed8bc25e5cc7ba35a", null ]
    ] ],
    [ "gnss_fix_status", "group__gnss__interface.html#ga15536308e2229a45493d06e8b61e63d9", [
      [ "GNSS_FIX_STATUS_NO_FIX", "group__gnss__interface.html#gga15536308e2229a45493d06e8b61e63d9a508bbeea1550d5020e579e6a311fbdbe", null ],
      [ "GNSS_FIX_STATUS_GNSS_FIX", "group__gnss__interface.html#gga15536308e2229a45493d06e8b61e63d9a8ad1fb7bf9f4d2607cb86f40a901e343", null ],
      [ "GNSS_FIX_STATUS_DGNSS_FIX", "group__gnss__interface.html#gga15536308e2229a45493d06e8b61e63d9a213553f5fa30cfa99317fcc28d0af1cc", null ],
      [ "GNSS_FIX_STATUS_ESTIMATED_FIX", "group__gnss__interface.html#gga15536308e2229a45493d06e8b61e63d9a19c4e932246f410ee702122d80fbfbeb", null ]
    ] ],
    [ "gnss_navigation_mode", "group__gnss__interface.html#gadb734bb12433276d3014e08a1d21bb18", [
      [ "GNSS_NAVIGATION_MODE_ZERO_DYNAMICS", "group__gnss__interface.html#ggadb734bb12433276d3014e08a1d21bb18a96f01438a205c0d557eb5ea4f80425ff", null ],
      [ "GNSS_NAVIGATION_MODE_LOW_DYNAMICS", "group__gnss__interface.html#ggadb734bb12433276d3014e08a1d21bb18ac5f4b57f2b0f732f9ae8051cb8ac9453", null ],
      [ "GNSS_NAVIGATION_MODE_BALANCED_DYNAMICS", "group__gnss__interface.html#ggadb734bb12433276d3014e08a1d21bb18ab7fd6073a1e7d28ebf6ae9bc51ded88d", null ],
      [ "GNSS_NAVIGATION_MODE_HIGH_DYNAMICS", "group__gnss__interface.html#ggadb734bb12433276d3014e08a1d21bb18a5f57c5f13e22a94c003f582e8c41a27f", null ]
    ] ],
    [ "gnss_pps_mode", "group__gnss__interface.html#ga2b43ac2fec33053a769b7070c4ed3263", [
      [ "GNSS_PPS_MODE_DISABLED", "group__gnss__interface.html#gga2b43ac2fec33053a769b7070c4ed3263a5e9af4b08bd41a1aa1bf01c0e290d9fc", null ],
      [ "GNSS_PPS_MODE_ENABLED", "group__gnss__interface.html#gga2b43ac2fec33053a769b7070c4ed3263a9cdbb144191c83f4a7a2cc27268eacfa", null ],
      [ "GNSS_PPS_MODE_ENABLED_AFTER_LOCK", "group__gnss__interface.html#gga2b43ac2fec33053a769b7070c4ed3263a2df416ad912a84135eff1e30977b4507", null ],
      [ "GNSS_PPS_MODE_ENABLED_WHILE_LOCKED", "group__gnss__interface.html#gga2b43ac2fec33053a769b7070c4ed3263afc82bd9ebcfd9d3b6111c9e82fc79ecc", null ]
    ] ],
    [ "gnss_system", "group__gnss__interface.html#ga928a05b4e820a9fcc8bc2db81f5f8c79", [
      [ "GNSS_SYSTEM_GPS", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a84d3fc7ebefad4b20c58335bb7ca3e33", null ],
      [ "GNSS_SYSTEM_GLONASS", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a6f5a6de0d8196df55ade83d1f09de7dd", null ],
      [ "GNSS_SYSTEM_GALILEO", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a1456923f0dd999b26e167fccc2461d5c", null ],
      [ "GNSS_SYSTEM_BEIDOU", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a25a0162daa071267b63d2c7331a46c55", null ],
      [ "GNSS_SYSTEM_QZSS", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79aac9ccafec388c071468ef16981679dff", null ],
      [ "GNSS_SYSTEM_IRNSS", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a833e14eb77d70b3c5eb2843026b63242", null ],
      [ "GNSS_SYSTEM_SBAS", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a3e759b71b7e531d6c2c3068108e263dd", null ],
      [ "GNSS_SYSTEM_IMES", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a6a216ab95d09806435af4257b4d189e9", null ],
      [ "GNSS_SYSTEM_GPS_L5", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a785caccbf75d3e8973e22b6f8c42836c", null ],
      [ "GNSS_SYSTEM_GALILEO_L5", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a9b0680e18087d1625753c14fbf09f7aa", null ],
      [ "GNSS_SYSTEM_QZSS_L5", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a7992be6af9405d17991351b4796329c2", null ],
      [ "GNSS_SYSTEM_BEIDOU_B1C", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a781047ec07475745b083770535d574a2", null ],
      [ "GNSS_SYSTEM_BEIDOU_B2A", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79ada9d2a2bb2133d924a452da2eff8a58c", null ],
      [ "GNSS_SYSTEM_QZSS_L1S", "group__gnss__interface.html#gga928a05b4e820a9fcc8bc2db81f5f8c79a806e153d2fe7888c56f73df2ddd9a9f2", null ]
    ] ],
    [ "gnss_get_enabled_systems", "group__gnss__interface.html#ga155126e113195c08158ef49e2a6b4f6a", null ],
    [ "gnss_get_fix_rate", "group__gnss__interface.html#ga8e625fb3f8758eab78c115d12bef6a72", null ],
    [ "gnss_get_latest_timepulse", "group__gnss__interface.html#ga8eb37446d71c020e0517a406ea240fc2", null ],
    [ "gnss_get_navigation_mode", "group__gnss__interface.html#gae5614f8125dad02a3ebd30400b575e6d", null ],
    [ "gnss_get_supported_systems", "group__gnss__interface.html#ga598609846ff196ff300fcad8237b2d51", null ],
    [ "gnss_set_enabled_systems", "group__gnss__interface.html#ga818e42c7a979623679eba26887662823", null ],
    [ "gnss_set_fix_rate", "group__gnss__interface.html#ga16e8326737f114bd6017983812c7f28d", null ],
    [ "gnss_set_navigation_mode", "group__gnss__interface.html#ga9d0347640e68d5702b1cb43ddf8380df", null ]
];