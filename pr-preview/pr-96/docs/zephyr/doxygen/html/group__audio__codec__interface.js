var group__audio__codec__interface =
[
    [ "pcm_config", "structpcm__config.html", [
      [ "block_size", "structpcm__config.html#aacad2be27111d146ad046ad048672f72", null ],
      [ "channels", "structpcm__config.html#ad0ad9c1e8c8af0016415f98a7fa0d69e", null ],
      [ "dir", "structpcm__config.html#a2e6b978dd1795d400076bd7ba0812f02", null ],
      [ "pcm_width", "structpcm__config.html#a9c9d58e429f9f9198d22a8437c8373e3", null ],
      [ "samplerate", "structpcm__config.html#ab1f1dd56e25317c3caaeb6206f07a211", null ]
    ] ],
    [ "audio_dai_cfg_t", "unionaudio__dai__cfg__t.html", [
      [ "i2s", "unionaudio__dai__cfg__t.html#a18514b2f3b9622ddb24a58d3701d090d", null ],
      [ "pcm", "unionaudio__dai__cfg__t.html#a859ece50501a1c060e20e80f3dc05b95", null ]
    ] ],
    [ "audio_codec_cfg", "structaudio__codec__cfg.html", [
      [ "dai_cfg", "structaudio__codec__cfg.html#aa42aa9a75adec5929c54910abdc5311c", null ],
      [ "dai_route", "structaudio__codec__cfg.html#ab1527545f55b33f9e2ce97f21c79e3b4", null ],
      [ "dai_type", "structaudio__codec__cfg.html#a121ca221241b0e2e90d9657a2462cd3c", null ],
      [ "mclk_freq", "structaudio__codec__cfg.html#a896521cdcacafeb3c5b42a439415c5bb", null ]
    ] ],
    [ "audio_property_value_t", "unionaudio__property__value__t.html", [
      [ "mute", "unionaudio__property__value__t.html#a23e0d9031006d64dfb31d87e4f061dc8", null ],
      [ "vol", "unionaudio__property__value__t.html#aafcfb98016ab4306b4fd6cb54f0c22e0", null ]
    ] ],
    [ "AUDIO_DAI_DIR_RX", "group__audio__codec__interface.html#gabd90b781156540cb1b6922a57a3ff265", null ],
    [ "AUDIO_DAI_DIR_TX", "group__audio__codec__interface.html#ga52ea7c9b5c0358f68223709291c60328", null ],
    [ "AUDIO_DAI_DIR_TXRX", "group__audio__codec__interface.html#ga810df470210d28a0eb0d7a12e8fde72f", null ],
    [ "audio_codec_error_callback_t", "group__audio__codec__interface.html#ga13ecf277fdf4cfaaa424402587a1a7ad", null ],
    [ "audio_codec_rx_done_callback_t", "group__audio__codec__interface.html#ga56e51f3a24b7fef4ce4fcb212b5807d9", null ],
    [ "audio_codec_tx_done_callback_t", "group__audio__codec__interface.html#ga5365e64b28a7fd33d359eb020cd7f366", null ],
    [ "audio_dai_dir_t", "group__audio__codec__interface.html#gad3a2ec4622f9a7b0eb1b4d0a463e20d5", null ],
    [ "audio_channel_t", "group__audio__codec__interface.html#ga966cf980ce24855830e24b5de4d884d6", [
      [ "AUDIO_CHANNEL_FRONT_LEFT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a0419483310bfc5abe46a0c586070ed18", null ],
      [ "AUDIO_CHANNEL_FRONT_RIGHT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a05525a25c5912eda05e9a8786a743a75", null ],
      [ "AUDIO_CHANNEL_LFE", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a6dfca2cc579d6619f87f38c90d0633e6", null ],
      [ "AUDIO_CHANNEL_FRONT_CENTER", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a9fc77d8958944d029fc2f056774b1be8", null ],
      [ "AUDIO_CHANNEL_REAR_LEFT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a4ab25a6492626c6634c41b39cf9462bc", null ],
      [ "AUDIO_CHANNEL_REAR_RIGHT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a370fed729325e3f3f79cc920f8fe37a9", null ],
      [ "AUDIO_CHANNEL_REAR_CENTER", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6af354197a57c321b4f966a33adf499b04", null ],
      [ "AUDIO_CHANNEL_SIDE_LEFT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6ada91a4234b3d7b115768ba08f39482ba", null ],
      [ "AUDIO_CHANNEL_SIDE_RIGHT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a7eac9f339583f2ce9f9a1a30c02555ec", null ],
      [ "AUDIO_CHANNEL_HEADPHONE_LEFT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6affc38ea2d76af83e6b06a1471dd68cfa", null ],
      [ "AUDIO_CHANNEL_HEADPHONE_RIGHT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a7b8d80478453df358cbdb7064f7a656d", null ],
      [ "AUDIO_CHANNEL_ALL", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a16fbc4bf16306517d21264188f447878", null ],
      [ "AUDIO_CHANNEL_COMMON_COUNT", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a1a35205e5a757748b3cf2f2048956cfe", null ],
      [ "AUDIO_CHANNEL_PRIV_START", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a3ad5430c80ae5a55ecbaf9246bd3ae93", null ],
      [ "AUDIO_CHANNEL_MAX", "group__audio__codec__interface.html#gga966cf980ce24855830e24b5de4d884d6a5370ff516a32a36a9f643d5906461817", null ]
    ] ],
    [ "audio_codec_error_type", "group__audio__codec__interface.html#ga2923c9aca9b8656ba3e3741d5860279a", [
      [ "AUDIO_CODEC_ERROR_OVERCURRENT", "group__audio__codec__interface.html#gga2923c9aca9b8656ba3e3741d5860279aacf3fcfdf3c6d471c5e402214cb8dc1f1", null ],
      [ "AUDIO_CODEC_ERROR_OVERTEMPERATURE", "group__audio__codec__interface.html#gga2923c9aca9b8656ba3e3741d5860279aab3b16fdb6f4a0d46801593922de82e7f", null ],
      [ "AUDIO_CODEC_ERROR_UNDERVOLTAGE", "group__audio__codec__interface.html#gga2923c9aca9b8656ba3e3741d5860279aa5fc4610a3cbc9588d02e0aaa7c8300ba", null ],
      [ "AUDIO_CODEC_ERROR_OVERVOLTAGE", "group__audio__codec__interface.html#gga2923c9aca9b8656ba3e3741d5860279aa84b9f2c5a8a930e0ecab603513fef9f6", null ],
      [ "AUDIO_CODEC_ERROR_DC", "group__audio__codec__interface.html#gga2923c9aca9b8656ba3e3741d5860279aab950af7e7fb02c49f5515389589c5e35", null ]
    ] ],
    [ "audio_dai_type_t", "group__audio__codec__interface.html#ga147094be4c7c5d62df695673d293f12d", [
      [ "AUDIO_DAI_TYPE_I2S", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12da13b981200501719efb95e15effacafac", null ],
      [ "AUDIO_DAI_TYPE_LEFT_JUSTIFIED", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12da3cac1ab007667ecc477f493cb74ca504", null ],
      [ "AUDIO_DAI_TYPE_RIGHT_JUSTIFIED", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12da94fdc805aec7c34da2de990d1b1bef6e", null ],
      [ "AUDIO_DAI_TYPE_PCMA", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12da0376329aa4700f6b6aed798a13cb1efe", null ],
      [ "AUDIO_DAI_TYPE_PCMB", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12daa578a69e94a460428e0d09f29cbf9ade", null ],
      [ "AUDIO_DAI_TYPE_PCM", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12dad32b311e083c62d10b20a4028295d192", null ],
      [ "AUDIO_DAI_TYPE_INVALID", "group__audio__codec__interface.html#gga147094be4c7c5d62df695673d293f12da7e794ea660c9953b621f56d14e25103c", null ]
    ] ],
    [ "audio_pcm_rate_t", "group__audio__codec__interface.html#ga8fd2079e6128c8a6da59c2dee58101ca", [
      [ "AUDIO_PCM_RATE_8K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caaaf78ccdda2de8de5e8978f5cc42318b8", null ],
      [ "AUDIO_PCM_RATE_11P025K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caa534b2e8dfa910c18b0617c236fa18c4d", null ],
      [ "AUDIO_PCM_RATE_16K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caae86fe5a3fc91eb2a7d93aaec8fb1f2d3", null ],
      [ "AUDIO_PCM_RATE_22P05K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caa77fab8d698408449f11d0974bfebaa0b", null ],
      [ "AUDIO_PCM_RATE_24K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caafaf9fdd89c698681685ee371ea834945", null ],
      [ "AUDIO_PCM_RATE_32K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caa29ebb75bd5be524a7e60470b20fdee90", null ],
      [ "AUDIO_PCM_RATE_44P1K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caae25d14c7f06f75b59407c1f881f1afad", null ],
      [ "AUDIO_PCM_RATE_48K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caad9d6a01be714fc4b72ef03266b7cc913", null ],
      [ "AUDIO_PCM_RATE_96K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caae682830b296e838f90dcff0689ae1811", null ],
      [ "AUDIO_PCM_RATE_192K", "group__audio__codec__interface.html#gga8fd2079e6128c8a6da59c2dee58101caa526109e4c13be490c6977b5ec56624e9", null ]
    ] ],
    [ "audio_pcm_width_t", "group__audio__codec__interface.html#ga1898dae0fac2e2bf34596453037bff7e", [
      [ "AUDIO_PCM_WIDTH_16_BITS", "group__audio__codec__interface.html#gga1898dae0fac2e2bf34596453037bff7ea222459c644bc02cb5edb07628de4c78e", null ],
      [ "AUDIO_PCM_WIDTH_20_BITS", "group__audio__codec__interface.html#gga1898dae0fac2e2bf34596453037bff7eae27896441250bb17791c1f7bb90d5638", null ],
      [ "AUDIO_PCM_WIDTH_24_BITS", "group__audio__codec__interface.html#gga1898dae0fac2e2bf34596453037bff7eaf26a8f2a5e72586771f6e0fc84cf5daa", null ],
      [ "AUDIO_PCM_WIDTH_32_BITS", "group__audio__codec__interface.html#gga1898dae0fac2e2bf34596453037bff7eab81e15d64e83e7e2371186fcd0e7fa0f", null ]
    ] ],
    [ "audio_property_t", "group__audio__codec__interface.html#ga5411b79fa217b0f9c63dc6890323804c", [
      [ "AUDIO_PROPERTY_OUTPUT_VOLUME", "group__audio__codec__interface.html#gga5411b79fa217b0f9c63dc6890323804ca3a386a4b7bfe9ae97a9ffdd7740d73d8", null ],
      [ "AUDIO_PROPERTY_OUTPUT_MUTE", "group__audio__codec__interface.html#gga5411b79fa217b0f9c63dc6890323804cafc7d548769874638f28a782ea4b10bdb", null ],
      [ "AUDIO_PROPERTY_INPUT_VOLUME", "group__audio__codec__interface.html#gga5411b79fa217b0f9c63dc6890323804ca0608dca9fec8160429d17d1d7dac0026", null ],
      [ "AUDIO_PROPERTY_INPUT_MUTE", "group__audio__codec__interface.html#gga5411b79fa217b0f9c63dc6890323804ca9b4548cfe663072a98db20ea0da377c1", null ]
    ] ],
    [ "audio_route_t", "group__audio__codec__interface.html#ga3b21ded95eb16d2c5f9f8da3c580a411", [
      [ "AUDIO_ROUTE_BYPASS", "group__audio__codec__interface.html#gga3b21ded95eb16d2c5f9f8da3c580a411a68af44d32f8c5e46a962fe0fc0fe0323", null ],
      [ "AUDIO_ROUTE_PLAYBACK", "group__audio__codec__interface.html#gga3b21ded95eb16d2c5f9f8da3c580a411a726898c5bfe3cbb217e3bca2fd1cdf7e", null ],
      [ "AUDIO_ROUTE_PLAYBACK_CAPTURE", "group__audio__codec__interface.html#gga3b21ded95eb16d2c5f9f8da3c580a411aa105c5914186aa9fbbd81af4e754ff0d", null ],
      [ "AUDIO_ROUTE_CAPTURE", "group__audio__codec__interface.html#gga3b21ded95eb16d2c5f9f8da3c580a411a31ebeb6337049a1ccea29ede230ad28a", null ]
    ] ],
    [ "audio_codec_apply_properties", "group__audio__codec__interface.html#gadd16956b1160388a17c23d6cc84218b3", null ],
    [ "audio_codec_clear_errors", "group__audio__codec__interface.html#ga550f86c43f7d96a1d826330d9da2d1af", null ],
    [ "audio_codec_configure", "group__audio__codec__interface.html#ga0c3344ea62fe994203315a0a7c20616c", null ],
    [ "audio_codec_register_done_callback", "group__audio__codec__interface.html#ga5e64f3537a4b0345f82e3e73992ee46a", null ],
    [ "audio_codec_register_error_callback", "group__audio__codec__interface.html#ga90f86f013dd46bad33bc95b2a7c8e1c9", null ],
    [ "audio_codec_route_input", "group__audio__codec__interface.html#ga92dc9ce48ff0785dbf002f52afcef254", null ],
    [ "audio_codec_route_output", "group__audio__codec__interface.html#gadaa36aabb932903c725007c2005a7304", null ],
    [ "audio_codec_set_property", "group__audio__codec__interface.html#ga6d28af0279eb8e4b693ea35f5235f189", null ],
    [ "audio_codec_start", "group__audio__codec__interface.html#ga445f95326d5bcf51c355dfc128f1e862", null ],
    [ "audio_codec_start_output", "group__audio__codec__interface.html#ga186e9c63c792072280558b953ced1cf7", null ],
    [ "audio_codec_stop", "group__audio__codec__interface.html#ga3fa574a02bc12c578ea306202a222d32", null ],
    [ "audio_codec_stop_output", "group__audio__codec__interface.html#ga6faba1d52c8d93e5ab5ec9954bf52f4a", null ],
    [ "audio_codec_write", "group__audio__codec__interface.html#gab6b430783bba55d30a2b99b4bf25a040", null ]
];