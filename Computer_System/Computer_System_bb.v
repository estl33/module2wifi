
module Computer_System (
	audio_ADCDAT,
	audio_ADCLRCK,
	audio_BCLK,
	audio_DACDAT,
	audio_DACLRCK,
	audio_clk_clk,
	audio_pll_ref_clk_clk,
	audio_pll_ref_reset_reset,
	av_config_SDAT,
	av_config_SCLK,
	bus_master_audio_external_interface_address,
	bus_master_audio_external_interface_byte_enable,
	bus_master_audio_external_interface_read,
	bus_master_audio_external_interface_write,
	bus_master_audio_external_interface_write_data,
	bus_master_audio_external_interface_acknowledge,
	bus_master_audio_external_interface_read_data,
	sdram_clk_clk,
	system_pll_ref_clk_clk,
	system_pll_ref_reset_reset);	

	input		audio_ADCDAT;
	input		audio_ADCLRCK;
	input		audio_BCLK;
	output		audio_DACDAT;
	input		audio_DACLRCK;
	output		audio_clk_clk;
	input		audio_pll_ref_clk_clk;
	input		audio_pll_ref_reset_reset;
	inout		av_config_SDAT;
	output		av_config_SCLK;
	input	[15:0]	bus_master_audio_external_interface_address;
	input	[3:0]	bus_master_audio_external_interface_byte_enable;
	input		bus_master_audio_external_interface_read;
	input		bus_master_audio_external_interface_write;
	input	[31:0]	bus_master_audio_external_interface_write_data;
	output		bus_master_audio_external_interface_acknowledge;
	output	[31:0]	bus_master_audio_external_interface_read_data;
	output		sdram_clk_clk;
	input		system_pll_ref_clk_clk;
	input		system_pll_ref_reset_reset;
endmodule
