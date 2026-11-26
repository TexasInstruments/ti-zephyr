var group__audio__dmic__interface =
[
    [ "pdm_io_cfg", "structpdm__io__cfg.html", [
      [ "max_pdm_clk_dc", "structpdm__io__cfg.html#aa03fbcc1f17fb2597bfa8c1898b74455", null ],
      [ "max_pdm_clk_freq", "structpdm__io__cfg.html#a4f07b2239435b0ac21e1049413d1a03a", null ],
      [ "min_pdm_clk_dc", "structpdm__io__cfg.html#a14d6d390746f7731c4ff203a4beec5fe", null ],
      [ "min_pdm_clk_freq", "structpdm__io__cfg.html#ae308635d30eaee617cb62c03c0255f37", null ],
      [ "pdm_clk_pol", "structpdm__io__cfg.html#a4a12d293f2c8edae3e7e5b71646ee55e", null ],
      [ "pdm_clk_skew", "structpdm__io__cfg.html#a692d7e838db86ca84558eb3e3170551a", null ],
      [ "pdm_data_pol", "structpdm__io__cfg.html#aa242b3383b20e0673ed47eea8450e9a7", null ]
    ] ],
    [ "pcm_stream_cfg", "structpcm__stream__cfg.html", [
      [ "block_size", "structpcm__stream__cfg.html#a3c224ba6f6c6fa7a128a739b6f1c36bf", null ],
      [ "mem_slab", "structpcm__stream__cfg.html#ade324dcfe8604fcb94400544ddb4517e", null ],
      [ "pcm_rate", "structpcm__stream__cfg.html#ae6e3790576f910943b0383f4731a5545", null ],
      [ "pcm_width", "structpcm__stream__cfg.html#a7d9fd0039bc25de292f35f60c99c6615", null ]
    ] ],
    [ "pdm_chan_cfg", "structpdm__chan__cfg.html", [
      [ "act_chan_map_hi", "structpdm__chan__cfg.html#a9a91e9341f4c033243effbebd5ffa194", null ],
      [ "act_chan_map_lo", "structpdm__chan__cfg.html#a34d2e2d03df48e26245835d4df88b60b", null ],
      [ "act_num_chan", "structpdm__chan__cfg.html#a3268169aaee14827cb15778fbc44ceb1", null ],
      [ "act_num_streams", "structpdm__chan__cfg.html#a7614fefd7976ba7b195afb4e81f7100e", null ],
      [ "req_chan_map_hi", "structpdm__chan__cfg.html#a11a258f6b21126e1d50c5a3f46b2cad7", null ],
      [ "req_chan_map_lo", "structpdm__chan__cfg.html#a028c8cd3dfcf0bfce618ef8937a7b211", null ],
      [ "req_num_chan", "structpdm__chan__cfg.html#aa2518c6f577c3fd92ec009fa134a6185", null ],
      [ "req_num_streams", "structpdm__chan__cfg.html#ae1398759382606cc60cf8944a1b9e5f6", null ]
    ] ],
    [ "dmic_cfg", "structdmic__cfg.html", [
      [ "channel", "structdmic__cfg.html#aaef2d2b475ce65d18c48d60cd1d393c4", null ],
      [ "io", "structdmic__cfg.html#abac7595e0a03cf976d8359a86259f54b", null ],
      [ "streams", "structdmic__cfg.html#a90f08070236a8e2c07c30edc54c687a5", null ]
    ] ],
    [ "PDM_DT_HAS_LEFT_CHANNEL", "group__audio__dmic__interface.html#ga0cd5bf8dad26b9315619e13c4c74f300", null ],
    [ "PDM_DT_HAS_RIGHT_CHANNEL", "group__audio__dmic__interface.html#ga0984dc3e571fd79f4700467e7305e5c6", null ],
    [ "PDM_DT_IO_CFG_GET", "group__audio__dmic__interface.html#ga61b191e55cffb65950e151cf1f67aed4", null ],
    [ "dmic_state", "group__audio__dmic__interface.html#gac99a4f579b6aa45bb6cce1f797247a23", [
      [ "DMIC_STATE_UNINIT", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23a45b37076808ed30994ed02ca0581b1be", null ],
      [ "DMIC_STATE_INITIALIZED", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23a733caa2b8a49d4b2f78698b810cbdbc1", null ],
      [ "DMIC_STATE_CONFIGURED", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23a70d47513a9c0ff07cdf4d814ac78a826", null ],
      [ "DMIC_STATE_ACTIVE", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23ae3f1d62885d522598ced88089f4ae8b4", null ],
      [ "DMIC_STATE_PAUSED", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23a14306a41b9a832343c8b6e1fcb971c81", null ],
      [ "DMIC_STATE_ERROR", "group__audio__dmic__interface.html#ggac99a4f579b6aa45bb6cce1f797247a23a8b6622df75d7c4205c11db2f26619eba", null ]
    ] ],
    [ "dmic_trigger", "group__audio__dmic__interface.html#ga24aae0a7871b3bd31b130061feeefc14", [
      [ "DMIC_TRIGGER_STOP", "group__audio__dmic__interface.html#gga24aae0a7871b3bd31b130061feeefc14ab3f5cdba100516563098e659adee2e23", null ],
      [ "DMIC_TRIGGER_START", "group__audio__dmic__interface.html#gga24aae0a7871b3bd31b130061feeefc14a031d6c7646642d4858a22c853d6d6eaa", null ],
      [ "DMIC_TRIGGER_PAUSE", "group__audio__dmic__interface.html#gga24aae0a7871b3bd31b130061feeefc14a6077bf984fd654947ff36e1bc66e98e5", null ],
      [ "DMIC_TRIGGER_RELEASE", "group__audio__dmic__interface.html#gga24aae0a7871b3bd31b130061feeefc14a69b5be548e54589e03645baf2ab80804", null ],
      [ "DMIC_TRIGGER_RESET", "group__audio__dmic__interface.html#gga24aae0a7871b3bd31b130061feeefc14aa6dca65941d65845383209c081173a79", null ]
    ] ],
    [ "pdm_lr", "group__audio__dmic__interface.html#ga7c2067bbb1e07fa8afe315f91a127991", [
      [ "PDM_CHAN_LEFT", "group__audio__dmic__interface.html#gga7c2067bbb1e07fa8afe315f91a127991acca80c04de225e35ae2bdfafcca6e345", null ],
      [ "PDM_CHAN_RIGHT", "group__audio__dmic__interface.html#gga7c2067bbb1e07fa8afe315f91a127991a0c746475a2b5873aa863058358d8e7d0", null ]
    ] ],
    [ "dmic_build_channel_map", "group__audio__dmic__interface.html#ga5d1fcefaf085962d779b8082b3480cbd", null ],
    [ "dmic_build_clk_skew_map", "group__audio__dmic__interface.html#gaf5abf489fe6ed18c2daea5d918aa2f80", null ],
    [ "dmic_configure", "group__audio__dmic__interface.html#ga09fc7074323c2f3115b06c5212d21fc7", null ],
    [ "dmic_parse_channel_map", "group__audio__dmic__interface.html#gaa1fded235751a71632965011a62ddead", null ],
    [ "dmic_read", "group__audio__dmic__interface.html#ga74b51bddaa175c518d45211f936d08e6", null ],
    [ "dmic_trigger", "group__audio__dmic__interface.html#ga051e82f30b76f0fca789ba0147054184", null ]
];