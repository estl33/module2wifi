	Computer_System u0 (
		.audio_ADCDAT                                    (<connected-to-audio_ADCDAT>),                                    //                               audio.ADCDAT
		.audio_ADCLRCK                                   (<connected-to-audio_ADCLRCK>),                                   //                                    .ADCLRCK
		.audio_BCLK                                      (<connected-to-audio_BCLK>),                                      //                                    .BCLK
		.audio_DACDAT                                    (<connected-to-audio_DACDAT>),                                    //                                    .DACDAT
		.audio_DACLRCK                                   (<connected-to-audio_DACLRCK>),                                   //                                    .DACLRCK
		.audio_clk_clk                                   (<connected-to-audio_clk_clk>),                                   //                           audio_clk.clk
		.audio_pll_ref_clk_clk                           (<connected-to-audio_pll_ref_clk_clk>),                           //                   audio_pll_ref_clk.clk
		.audio_pll_ref_reset_reset                       (<connected-to-audio_pll_ref_reset_reset>),                       //                 audio_pll_ref_reset.reset
		.av_config_SDAT                                  (<connected-to-av_config_SDAT>),                                  //                           av_config.SDAT
		.av_config_SCLK                                  (<connected-to-av_config_SCLK>),                                  //                                    .SCLK
		.bus_master_audio_external_interface_address     (<connected-to-bus_master_audio_external_interface_address>),     // bus_master_audio_external_interface.address
		.bus_master_audio_external_interface_byte_enable (<connected-to-bus_master_audio_external_interface_byte_enable>), //                                    .byte_enable
		.bus_master_audio_external_interface_read        (<connected-to-bus_master_audio_external_interface_read>),        //                                    .read
		.bus_master_audio_external_interface_write       (<connected-to-bus_master_audio_external_interface_write>),       //                                    .write
		.bus_master_audio_external_interface_write_data  (<connected-to-bus_master_audio_external_interface_write_data>),  //                                    .write_data
		.bus_master_audio_external_interface_acknowledge (<connected-to-bus_master_audio_external_interface_acknowledge>), //                                    .acknowledge
		.bus_master_audio_external_interface_read_data   (<connected-to-bus_master_audio_external_interface_read_data>),   //                                    .read_data
		.sdram_clk_clk                                   (<connected-to-sdram_clk_clk>),                                   //                           sdram_clk.clk
		.system_pll_ref_clk_clk                          (<connected-to-system_pll_ref_clk_clk>),                          //                  system_pll_ref_clk.clk
		.system_pll_ref_reset_reset                      (<connected-to-system_pll_ref_reset_reset>)                       //                system_pll_ref_reset.reset
	);

