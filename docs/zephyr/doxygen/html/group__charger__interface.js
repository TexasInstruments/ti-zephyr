var group__charger__interface =
[
    [ "Device-specific Charger API extensions", "group__charger__interface__ext.html", "group__charger__interface__ext" ],
    [ "Charger Driver Backend API", "group__charger__interface__backend.html", "group__charger__interface__backend" ],
    [ "charger.h", "charger_8h.html", null ],
    [ "charger_current_notifier", "structcharger__current__notifier.html", [
      [ "current_ua", "structcharger__current__notifier.html#acb54706bf47f55e197e799393b9eb87e", null ],
      [ "duration_us", "structcharger__current__notifier.html#a4998999ef893d18fa64b86ec4fd9ad94", null ],
      [ "severity", "structcharger__current__notifier.html#a22c26477263e5abedb200de36a29d9de", null ]
    ] ],
    [ "charger_propval", "unioncharger__propval.html", [
      [ "charge_term_current_ua", "unioncharger__propval.html#a89d0c93d68650cefe29e891f4db928f4", null ],
      [ "charge_type", "unioncharger__propval.html#ac1a594de2b1c10d28b6a81dd66f1a6ba", null ],
      [ "const_charge_current_ua", "unioncharger__propval.html#af4e383bb797432033050937740ce0ac3", null ],
      [ "const_charge_voltage_uv", "unioncharger__propval.html#a733bae9ce0bf7e902f8af307f94a356c", null ],
      [ "custom_bool", "unioncharger__propval.html#a1619752e64e8f99d1a488d1fa887ee60", null ],
      [ "custom_int", "unioncharger__propval.html#a0e9c59df061dbe7d221244cd48530a10", null ],
      [ "custom_uint", "unioncharger__propval.html#a7b7aa29e279028e502d8662c70fc1747", null ],
      [ "discharge_current_notification", "unioncharger__propval.html#af256e63ca94f7809da101ad1c33c8e7a", null ],
      [ "health", "unioncharger__propval.html#abd990f67423424da60856f4b6e8824a3", null ],
      [ "input_current_notification", "unioncharger__propval.html#a0ad956ab4d0b306f94cce2aa81283a35", null ],
      [ "input_current_regulation_current_ua", "unioncharger__propval.html#ae51670de8fa560f0c7610452db029bc2", null ],
      [ "input_voltage_regulation_voltage_uv", "unioncharger__propval.html#afbe7b5fe1e421fb97b37d71ce14dc51a", null ],
      [ "online", "unioncharger__propval.html#a8589124e6152b70b649788a8b94461ab", null ],
      [ "online_notification", "unioncharger__propval.html#ab9f40a6083ff2361ea4d161226cd77ad", null ],
      [ "precharge_current_ua", "unioncharger__propval.html#a24a97e16505fa336bd3a68743d05936b", null ],
      [ "present", "unioncharger__propval.html#ab0eec508b90d205a1662fd6c69638764", null ],
      [ "status", "unioncharger__propval.html#a7f4976d72d4a7d48385ceb9f18c510b5", null ],
      [ "status_notification", "unioncharger__propval.html#ad77547e88ec465d2e37526751008f634", null ],
      [ "system_voltage_notification", "unioncharger__propval.html#a541d1bf0b3e32d7aaeb6a2f4f443c5dc", null ]
    ] ],
    [ "charger_custom_value_bool_t", "group__charger__interface.html#ga3b448c73ef8c4844ccbe7adb1f2458fe", null ],
    [ "charger_custom_value_int_t", "group__charger__interface.html#ga9325615cdd8985cbb3407257afb387a2", null ],
    [ "charger_custom_value_uint_t", "group__charger__interface.html#gaa7a6c34b4871673151671eae1fac84da", null ],
    [ "charger_online_notifier_t", "group__charger__interface.html#gab29c2fafc53988555d72974199f25475", null ],
    [ "charger_prop_t", "group__charger__interface.html#ga4ce808890de2d01cdb75feddbf275635", null ],
    [ "charger_status_notifier_t", "group__charger__interface.html#ga7666697bde91b66829113efe151d1cb2", null ],
    [ "charger_charge_type", "group__charger__interface.html#gaee833a379a8674621d2fdf9b57d1f803", [
      [ "CHARGER_CHARGE_TYPE_UNKNOWN", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803aab6c7ea7d961c131bc91f91a9fa7cce4", null ],
      [ "CHARGER_CHARGE_TYPE_NONE", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803a71e7c6a73193c0ce1ba0467d93b72f17", null ],
      [ "CHARGER_CHARGE_TYPE_TRICKLE", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803acc5367f61f10e6574ec01d090598cbf8", null ],
      [ "CHARGER_CHARGE_TYPE_FAST", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803ae2b5fc2d034a037c9eee5afca2c5a711", null ],
      [ "CHARGER_CHARGE_TYPE_STANDARD", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803a004c6c71a3a9943e151ed4fa49746ee6", null ],
      [ "CHARGER_CHARGE_TYPE_ADAPTIVE", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803aa5a6d6cc9767fb0b19d13070c108ccc2", null ],
      [ "CHARGER_CHARGE_TYPE_LONGLIFE", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803acaa4a3c513188cd9dba71c98639bd31f", null ],
      [ "CHARGER_CHARGE_TYPE_BYPASS", "group__charger__interface.html#ggaee833a379a8674621d2fdf9b57d1f803abe42aac55841886e2a69b94018918ee8", null ]
    ] ],
    [ "charger_health", "group__charger__interface.html#gaab33241d3b85ab0770be9b1bd17e4412", [
      [ "CHARGER_HEALTH_UNKNOWN", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a21f2873586a920885d411e8dd02786ad", null ],
      [ "CHARGER_HEALTH_GOOD", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a7fe49d6d81fc76c70ab979eece42d538", null ],
      [ "CHARGER_HEALTH_OVERHEAT", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412affd0237aed52d704e51f18a4d57f3b3b", null ],
      [ "CHARGER_HEALTH_OVERVOLTAGE", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412affbc931d777af7a03233d0dbed364459", null ],
      [ "CHARGER_HEALTH_UNSPEC_FAILURE", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a7651b0b32b8472caa8545278bef4e8fa", null ],
      [ "CHARGER_HEALTH_COLD", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a24b2e80a03d4d06a267f3264edd64967", null ],
      [ "CHARGER_HEALTH_WATCHDOG_TIMER_EXPIRE", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412ab9c81803fd5a1dd6a843765304e592ce", null ],
      [ "CHARGER_HEALTH_SAFETY_TIMER_EXPIRE", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a17ccf9a3e7878c11e6a34bf5a4e38a82", null ],
      [ "CHARGER_HEALTH_CALIBRATION_REQUIRED", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a1936cf89fd8e7286831a91dc218c25f1", null ],
      [ "CHARGER_HEALTH_WARM", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a206b491bf7e7efc60b9a41d69a2d305f", null ],
      [ "CHARGER_HEALTH_COOL", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412aad346d9b4b43998367facd491bc0ecd3", null ],
      [ "CHARGER_HEALTH_HOT", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412aa3cd6b3e3e2ccfde03ba49912a2efcf1", null ],
      [ "CHARGER_HEALTH_NO_BATTERY", "group__charger__interface.html#ggaab33241d3b85ab0770be9b1bd17e4412a50dabeb87f23ca712b5c78f0e5a4c60b", null ]
    ] ],
    [ "charger_notification_severity", "group__charger__interface.html#ga2e7d7f15725db4d502bc3f46476a310d", [
      [ "CHARGER_SEVERITY_PEAK", "group__charger__interface.html#gga2e7d7f15725db4d502bc3f46476a310da667f21cc72028772d09228424f75b40b", null ],
      [ "CHARGER_SEVERITY_CRITICAL", "group__charger__interface.html#gga2e7d7f15725db4d502bc3f46476a310da6bb0e1059a26d31380094530da94e3e8", null ],
      [ "CHARGER_SEVERITY_WARNING", "group__charger__interface.html#gga2e7d7f15725db4d502bc3f46476a310da347972f338fe70920f72c85dcdd1f885", null ]
    ] ],
    [ "charger_online", "group__charger__interface.html#gad95d2b1aaf18ac3a1c536f2d40317c19", [
      [ "CHARGER_ONLINE_OFFLINE", "group__charger__interface.html#ggad95d2b1aaf18ac3a1c536f2d40317c19a50004df0b037325d33d21427a847a5b3", null ],
      [ "CHARGER_ONLINE_FIXED", "group__charger__interface.html#ggad95d2b1aaf18ac3a1c536f2d40317c19a119679acb9bcc173831003dfa309f5a6", null ],
      [ "CHARGER_ONLINE_PROGRAMMABLE", "group__charger__interface.html#ggad95d2b1aaf18ac3a1c536f2d40317c19a2b51eb24ac1047c99f8079beb261503e", null ]
    ] ],
    [ "charger_property", "group__charger__interface.html#ga6eb3b4cc76e4f1e34732b7853eb860b7", [
      [ "CHARGER_PROP_ONLINE", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a1c8906c0e4e278b84cab7d126cf95526", null ],
      [ "CHARGER_PROP_PRESENT", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a0dd5ee0d22cba6440c3f3583582d405b", null ],
      [ "CHARGER_PROP_STATUS", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a73b9f45dd43347016a4dd6e15cd78cf6", null ],
      [ "CHARGER_PROP_CHARGE_TYPE", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7aff40a23cd65dcab5b3118fd9da3aaf95", null ],
      [ "CHARGER_PROP_HEALTH", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a2cc8bc744a98284e6c96a6ffb30ec654", null ],
      [ "CHARGER_PROP_CONSTANT_CHARGE_CURRENT_UA", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a81a18d7286bfcf729402db6990ddc306", null ],
      [ "CHARGER_PROP_PRECHARGE_CURRENT_UA", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7aa8d97a1db907ccd40a7b5d917c0a9ab9", null ],
      [ "CHARGER_PROP_CHARGE_TERM_CURRENT_UA", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7ac60d2e9945e8f1eaf0e824b778a377fc", null ],
      [ "CHARGER_PROP_CONSTANT_CHARGE_VOLTAGE_UV", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a5b4a6c4f1f6abb7a568c7c42dca696bf", null ],
      [ "CHARGER_PROP_INPUT_REGULATION_CURRENT_UA", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a703a0e374053a410ec154eac117f9085", null ],
      [ "CHARGER_PROP_INPUT_REGULATION_VOLTAGE_UV", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a1e60d770a5d8e47118040ace3c6d8ec0", null ],
      [ "CHARGER_PROP_INPUT_CURRENT_NOTIFICATION", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a70f2c71cd9d4e0f88ee36886094540fe", null ],
      [ "CHARGER_PROP_DISCHARGE_CURRENT_NOTIFICATION", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a3d3e3190b016a9c31ed5c82e2ade0b72", null ],
      [ "CHARGER_PROP_SYSTEM_VOLTAGE_NOTIFICATION_UV", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7ace0a0e6cbc0ccaf166ff0df7e1b275c0", null ],
      [ "CHARGER_PROP_STATUS_NOTIFICATION", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a9340eb6fb5b80370148a43366b753d9b", null ],
      [ "CHARGER_PROP_ONLINE_NOTIFICATION", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a389920359b8866af70b164d68820d4b7", null ],
      [ "CHARGER_PROP_COMMON_COUNT", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a3c6d858ad83fa0aeca39ce33817f3732", null ],
      [ "CHARGER_PROP_CUSTOM_BEGIN", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a279bd89ce909be3ea95be2fee33b08b3", null ],
      [ "CHARGER_PROP_MAX", "group__charger__interface.html#gga6eb3b4cc76e4f1e34732b7853eb860b7a97342ebdadce27894e82dc57ed54724e", null ]
    ] ],
    [ "charger_status", "group__charger__interface.html#ga4a3c46bc0916082d15e665f7665c89d7", [
      [ "CHARGER_STATUS_UNKNOWN", "group__charger__interface.html#gga4a3c46bc0916082d15e665f7665c89d7ad474b06c524aea24b1ecb2e7d6621fa5", null ],
      [ "CHARGER_STATUS_CHARGING", "group__charger__interface.html#gga4a3c46bc0916082d15e665f7665c89d7ad95370e7cd6dc5d72e73d741b41cb8ad", null ],
      [ "CHARGER_STATUS_DISCHARGING", "group__charger__interface.html#gga4a3c46bc0916082d15e665f7665c89d7ad06083516280a41fec5f8cc649ff499e", null ],
      [ "CHARGER_STATUS_NOT_CHARGING", "group__charger__interface.html#gga4a3c46bc0916082d15e665f7665c89d7a445d1979c2541ead57ddaa89dc57c658", null ],
      [ "CHARGER_STATUS_FULL", "group__charger__interface.html#gga4a3c46bc0916082d15e665f7665c89d7a2f550ea27e63b2eab78cc673d3e692cf", null ]
    ] ],
    [ "charger_charge_enable", "group__charger__interface.html#gace1ea9841574d75d314db078df5a0b3a", null ],
    [ "charger_get_prop", "group__charger__interface.html#ga00e5b61d517c93d7d3c863b14b07b738", null ],
    [ "charger_set_prop", "group__charger__interface.html#ga0036e3f5924585457a99a2e444ef5aab", null ]
];