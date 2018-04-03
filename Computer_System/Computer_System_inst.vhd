	component Computer_System is
		port (
			audio_ADCDAT                                    : in    std_logic                     := 'X';             -- ADCDAT
			audio_ADCLRCK                                   : in    std_logic                     := 'X';             -- ADCLRCK
			audio_BCLK                                      : in    std_logic                     := 'X';             -- BCLK
			audio_DACDAT                                    : out   std_logic;                                        -- DACDAT
			audio_DACLRCK                                   : in    std_logic                     := 'X';             -- DACLRCK
			audio_clk_clk                                   : out   std_logic;                                        -- clk
			audio_pll_ref_clk_clk                           : in    std_logic                     := 'X';             -- clk
			audio_pll_ref_reset_reset                       : in    std_logic                     := 'X';             -- reset
			av_config_SDAT                                  : inout std_logic                     := 'X';             -- SDAT
			av_config_SCLK                                  : out   std_logic;                                        -- SCLK
			bus_master_audio_external_interface_address     : in    std_logic_vector(15 downto 0) := (others => 'X'); -- address
			bus_master_audio_external_interface_byte_enable : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- byte_enable
			bus_master_audio_external_interface_read        : in    std_logic                     := 'X';             -- read
			bus_master_audio_external_interface_write       : in    std_logic                     := 'X';             -- write
			bus_master_audio_external_interface_write_data  : in    std_logic_vector(31 downto 0) := (others => 'X'); -- write_data
			bus_master_audio_external_interface_acknowledge : out   std_logic;                                        -- acknowledge
			bus_master_audio_external_interface_read_data   : out   std_logic_vector(31 downto 0);                    -- read_data
			sdram_clk_clk                                   : out   std_logic;                                        -- clk
			system_pll_ref_clk_clk                          : in    std_logic                     := 'X';             -- clk
			system_pll_ref_reset_reset                      : in    std_logic                     := 'X'              -- reset
		);
	end component Computer_System;

	u0 : component Computer_System
		port map (
			audio_ADCDAT                                    => CONNECTED_TO_audio_ADCDAT,                                    --                               audio.ADCDAT
			audio_ADCLRCK                                   => CONNECTED_TO_audio_ADCLRCK,                                   --                                    .ADCLRCK
			audio_BCLK                                      => CONNECTED_TO_audio_BCLK,                                      --                                    .BCLK
			audio_DACDAT                                    => CONNECTED_TO_audio_DACDAT,                                    --                                    .DACDAT
			audio_DACLRCK                                   => CONNECTED_TO_audio_DACLRCK,                                   --                                    .DACLRCK
			audio_clk_clk                                   => CONNECTED_TO_audio_clk_clk,                                   --                           audio_clk.clk
			audio_pll_ref_clk_clk                           => CONNECTED_TO_audio_pll_ref_clk_clk,                           --                   audio_pll_ref_clk.clk
			audio_pll_ref_reset_reset                       => CONNECTED_TO_audio_pll_ref_reset_reset,                       --                 audio_pll_ref_reset.reset
			av_config_SDAT                                  => CONNECTED_TO_av_config_SDAT,                                  --                           av_config.SDAT
			av_config_SCLK                                  => CONNECTED_TO_av_config_SCLK,                                  --                                    .SCLK
			bus_master_audio_external_interface_address     => CONNECTED_TO_bus_master_audio_external_interface_address,     -- bus_master_audio_external_interface.address
			bus_master_audio_external_interface_byte_enable => CONNECTED_TO_bus_master_audio_external_interface_byte_enable, --                                    .byte_enable
			bus_master_audio_external_interface_read        => CONNECTED_TO_bus_master_audio_external_interface_read,        --                                    .read
			bus_master_audio_external_interface_write       => CONNECTED_TO_bus_master_audio_external_interface_write,       --                                    .write
			bus_master_audio_external_interface_write_data  => CONNECTED_TO_bus_master_audio_external_interface_write_data,  --                                    .write_data
			bus_master_audio_external_interface_acknowledge => CONNECTED_TO_bus_master_audio_external_interface_acknowledge, --                                    .acknowledge
			bus_master_audio_external_interface_read_data   => CONNECTED_TO_bus_master_audio_external_interface_read_data,   --                                    .read_data
			sdram_clk_clk                                   => CONNECTED_TO_sdram_clk_clk,                                   --                           sdram_clk.clk
			system_pll_ref_clk_clk                          => CONNECTED_TO_system_pll_ref_clk_clk,                          --                  system_pll_ref_clk.clk
			system_pll_ref_reset_reset                      => CONNECTED_TO_system_pll_ref_reset_reset                       --                system_pll_ref_reset.reset
		);

