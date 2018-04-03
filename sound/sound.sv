module sound (CLOCK_50, CLOCK2_50, KEY,
         AUD_DACLRCK, AUD_ADCLRCK, AUD_BCLK,AUD_ADCDAT,
			FPGA_I2C_SDAT, FPGA_I2C_SCLK,AUD_DACDAT,AUD_XCK, SW, sound_in, sound_out, 	
	ADC_CS_N,
	ADC_DIN,
	ADC_DOUT,
	ADC_SCLK,
	CLOCK3_50,
	CLOCK4_50, LEDR, volume_in, treble, bass);
			
input CLOCK_50,CLOCK2_50,AUD_DACLRCK, AUD_ADCLRCK, AUD_BCLK,AUD_ADCDAT;
input [3:0] KEY;
inout FPGA_I2C_SDAT;
output FPGA_I2C_SCLK,AUD_DACDAT,AUD_XCK;

input signed [15:0] sound_in;
output signed [15:0] sound_out;

input						CLOCK3_50;
input						CLOCK4_50;

// ADC
inout						ADC_CS_N;
output					ADC_DIN;
input						ADC_DOUT;
output					ADC_SCLK;

// Define an enumerated type for our state machine

typedef enum {state_init, state_assert_address, state_readdatavalid, state_read_address_write_sample1,
				state_write_sample2, state_increment_count, state_done, state_wait_until_ready, state_send_sample, 
				state_wait_for_accepted, state_wait_until_read_ready, state_read_sample} state_type;

// signals that are used to communicate with the audio core

reg read_ready, write_ready, write_s;
reg [15:0] writedata_left, writedata_right;
reg [15:0] readdata_left, readdata_right;	
wire reset, read_s;

// some signals I will use in my always block

integer cnt;
//state_type state;
reg signed [31:0] sample;

// instantiate the parts of the audio core. 
/*
clock_generator my_clock_gen (CLOCK2_50, reset, AUD_XCK);
audio_and_video_config cfg (CLOCK_50, reset, FPGA_I2C_SDAT, FPGA_I2C_SCLK);
audio_codec codec (CLOCK_50,reset,read_s,write_s,writedata_left, writedata_right,AUD_ADCDAT,AUD_BCLK,AUD_ADCLRCK,AUD_DACLRCK,read_ready, write_ready,readdata_left, readdata_right,AUD_DACDAT);
*/
// The audio core requires an active high reset signal

assign reset = ~(KEY[0]);

// we will never read from the microphone in this lab, so we might as well set read_s to 0.

//assign read_s = 1'b0;

// The main state machine in the design.  The purpose of this state machien
// is to send samples to the audio core.  This machine will send 91 high samples
// followed by 91 low samples, and repeat.  It turns out that this square wave 
// will sound like a single tone when played.  In the lab, you will modify this
// to send the actual samples (which descirbe a waveform much more complex
// than just a square wave).

wire clk, resetb;

assign clk = CLOCK_50;
assign resetb = KEY[0];

wire            flash_mem_read;
wire            flash_mem_waitrequest;
wire    [22:0]  flash_mem_address;
wire    [31:0]  flash_mem_readdata;
wire            flash_mem_readdatavalid;
wire    [3:0]   flash_mem_byteenable;

flash flash_inst (
    .clk_clk                 (clk),
    .reset_reset_n           (resetb),
    .flash_mem_write         (1'b0),
    .flash_mem_burstcount    (1'b1),
    .flash_mem_waitrequest   (flash_mem_waitrequest),
    .flash_mem_read          (flash_mem_read),
    .flash_mem_address       (flash_mem_address),
    .flash_mem_writedata     (),
    .flash_mem_readdata      (flash_mem_readdata),
    .flash_mem_readdatavalid (flash_mem_readdatavalid),
    .flash_mem_byteenable    (flash_mem_byteenable)
);

assign flash_mem_byteenable = 4'b1111;


// the rest of your code goes here.  Don't forget to instantiate the on-chip memory
assign flash_mem_write = 1'b0;
assign flash_mem_writedata = 32'b0;

assign flash_mem_burstcount = 6'b000001;

reg [31:0] samplepair;

reg [7:0] address; 
reg [15:0] data, q;
reg wren;

s_memory2 u0(address, CLOCK_50, data, wren, q);

integer count;

integer samplenumber;
	
typedef enum {normal, fast, slow} playback;
playback mode;

input [9:0] SW;
output [9:0] LEDR;
	
	/*
always @(SW) begin
	case (SW) 
		10'b0000000010: mode = fast;
		10'b0000000100: mode = slow;
		default: mode = normal;
	endcase
end	
	*/
integer loopnumber;	
	
wire pause;
assign pause = ~KEY[3];
reg should_pause;

always_ff @(posedge pause, posedge reset)
	if (reset == 1'b1) begin
		should_pause = 0;
	end else if (pause == 1'b1) begin
		if (should_pause == 1'b1) begin
			should_pause = 0;
		end else begin
			should_pause = 1'b1;
		end //endif should_pause
	end //endif
	
reg signed [47:0] lastOutput;
reg signed [47:0] distanceToGo;

reg signed [47:0] withoutTreble;
reg signed [47:0] trebleOnly;
reg signed [47:0] bassOnly;
	
	/*
reg signed [3:0] treble;
reg signed [3:0] bass;	


wire trebleKey;
assign trebleKey = ~KEY[1];

always_ff @(posedge trebleKey, posedge reset)
	if (reset == 1'b1) begin
		treble = 1;
	end else begin
		treble = treble + 1;
		if (treble > 5) begin
			treble = 0;
		end
	end //endif
	
wire bassKey;
assign bassKey = ~KEY[2];

always_ff @(posedge bassKey, posedge reset)
	if (reset == 1'b1) begin
		bass = 0;
	end else begin
		bass = bass + 1;
		if (bass > 4) begin
			bass = -1;
		end
	end //endif
	*/
	
input signed [3:0] treble;
input signed [3:0] bass;
	
input [15:0] volume_in;	
	
reg [31:0] bus_addr ; // Avalon address
// see 
// ftp://ftp.altera.com/up/pub/Altera_Material/15.1/University_Program_IP_Cores/Audio_Video/Audio.pdf
// for addresses
wire [31:0] audio_base_address = 32'h00003040 ;  // Avalon address
wire [31:0] audio_fifo_address = 32'h00003044 ;  // Avalon address +4 offset
wire [31:0] audio_data_left_address = 32'h00003048 ;  // Avalon address +8
wire [31:0] audio_data_right_address = 32'h0000304c ;  // Avalon address +12
reg [3:0] bus_byte_enable ; // four bit byte read/write mask
reg bus_read  ;       // high when requesting data
reg bus_write ;      //  high when writing data
reg [31:0] bus_write_data ; //  data to send to Avalog bus
wire bus_ack  ;       //  Avalon bus raises this when done
wire [31:0] bus_read_data ; // data from Avalon bus
reg [30:0] timer ;
reg [3:0] state ;
wire state_clock ;

// current free words in audio interface
reg [7:0] fifo_space ;	
reg [31:0] right_audio_input, left_audio_input ;	
	
always @(posedge CLOCK_50, posedge reset) begin //CLOCK_50

	// reset state machine and read/write controls
	if (reset == 1'b1) begin
		state <= 0 ;
		bus_read <= 0 ; // set to one if a read opeation from bus
		bus_write <= 0 ; // set to one if a write operation to bus
	end else if (should_pause == 1'b1) begin
		// do nothing
	end else begin

	// === writing stereo to the audio FIFO ==========

	// set up read FIFO available space
	if (state==4'd0) begin
		bus_addr <= audio_fifo_address ;
		bus_read <= 1'b1 ;
		bus_byte_enable <= 4'b1111;
		state <= 4'd1 ; // wait for read ACK
	end
	
	// wait for read ACK and read the fifo available
	// bus ACK is high when data is available
	if (state==4'd1 && bus_ack==1) begin
		state <= 4'd2 ; //4'd2
		// FIFO write space is in high byte
		fifo_space <= (bus_read_data>>24) ;
		// end the read
		bus_read <= 1'b0 ;
	end
	
	// When there is room in the FIFO
	// -- compute next DDS sine sample
	// -- start write to fifo for each channel
	// -- first the left channel
	if (state==4'd2 && fifo_space>8'd2) begin // 
		state <= 4'd3;	
		// IF SW=10'h200 
		// and Fout = (sample_rate)/(2^32)*{SW[9:0], 16'b0}
		// then Fout=48000/(2^32)*(2^25) = 375 Hz
		
		if (SW[9] == 1'b1) begin
		
		if (volume_in == 16'b0) sample = sample/2048;						//min volume  
						else if(volume_in < 16'b0000000000111100) sample = sample/512; //vol < 60
						else if(volume_in < 16'b0000000001111000) sample = sample/256; // vol < 120
						else if(volume_in < 16'b0000000010110100) sample = sample/128; // vol < 180
						else if(volume_in < 16'b0000000011110000) sample = sample/64; 	// vol < 240
						else if(volume_in < 16'b0000000100101100) sample = sample/32; 	// vol < 300
						else if(volume_in < 16'b0000000101101000) sample = sample/16; 	// vol < 360
						else if(volume_in < 16'b0000000110100100) sample = sample/4; 	// vol < 420
						else if(volume_in < 16'b0000000110100100) sample = sample/2; 	// vol < 480
						else sample = sample;	
		end
	
		// Equalize
		distanceToGo = {sample, 16'b0000000000000000} - withoutTreble;
		withoutTreble = withoutTreble + (distanceToGo >>> 3);
		trebleOnly = {sample, 16'b0000000000000000} - withoutTreble;
						
		distanceToGo = withoutTreble - bassOnly;
		bassOnly = bassOnly + (distanceToGo >>> 3);
		
		lastOutput <= withoutTreble + trebleOnly * treble + bassOnly * (bass - 1);
		//
		
		// TODO: May have to convert sound_out to 32 bits
		sound_out <= lastOutput[31: 16];
		
		// write to left audio
		bus_write_data <= lastOutput[47:16];//left_audio_input ;
		bus_addr <= audio_data_left_address ;
		bus_byte_enable <= 4'b1111;
		bus_write <= 1'b1 ;
	end	
	// if no space, try again later
	else if (state==4'd2 && fifo_space<=8'd2) begin
		state <= 4'b0 ;
	end
	
	// detect bus-transaction-complete ACK 
	// for left channel write
	// You MUST do this check
	if (state==4'd3 && bus_ack==1) begin
		state <= 4'd4 ; // include right channel
		//state <= 4'd0 ; // left channel only!
		bus_write <= 0;
	end
	
	// -- now the right channel
	if (state==4'd4) begin // 
		state <= 4'd5;	
		// loop back audio input data
		// write to right audio
		bus_write_data <= lastOutput[47:16];//right_audio_input ;
		bus_addr <= audio_data_right_address ;
		bus_write <= 1'b1 ;
	end	
	
	// detect bus-transaction-complete ACK
	// for right channel write
	// You MUST do this check
	if (state==4'd5 && bus_ack==1) begin
		// state <= 4'd0 ; // for write only function
		state <= 4'd6 ; // for read/write  function
		bus_write <= 0;
	end
	
	// === reading stereo from the audio FIFO ==========

	// set up read FIFO for available read values
	if (state==4'd6 ) begin
		bus_addr <= audio_fifo_address ;
		bus_read <= 1'b1 ;
		bus_byte_enable <= 4'b1111;
		state <= 4'd7 ; // wait for read ACK
	end
	
	// wait for read ACK and read the fifo available
	// bus ACK is high when data is available
	if (state==4'd7 && bus_ack==1) begin
		state <= 4'd8 ; //4'dxx
		// FIFO read space is in low byte
		// which is zero when empty
		fifo_space <= bus_read_data & 8'hff ;
		// end the read
		bus_read <= 1'b0 ;
	end
	
	// When there is data in the read FIFO
	// -- read it from both channels
	// -- first the left channel
	if (state==4'd8 && fifo_space>8'd0) begin // 
		state <= 4'd9;	
		bus_addr <= audio_data_left_address ;
		bus_byte_enable <= 4'b1111;
		bus_read <= 1'b1 ;
	end	
	// if no space, try again later
	else if (state==4'd8 && fifo_space<=8'd0) begin
		state <= 4'b0 ;
	end
	
	// detect bus-transaction-complete ACK 
	// for left channel read
	// You MUST do this check
	if (state==4'd9 && bus_ack==1) begin
		state <= 4'd10 ; // include right channel
		left_audio_input <= bus_read_data ;
		bus_read <= 0;
	end
	
	// When there is data in the read FIFO
	// -- read it from both channels
	// -- now right channel
	if (state==4'd10) begin // 
		state <= 4'd11;	
		bus_addr <= audio_data_right_address ;
		bus_byte_enable <= 4'b1111;
		bus_read <= 1'b1 ;
	end	
	
	// detect bus-transaction-complete ACK 
	// for left channel read
	// You MUST do this check
	if (state==4'd11 && bus_ack==1) begin
		state <= 4'd0 ; // back to beginning
		//right_audio_input <= bus_read_data ;
		sample <= bus_read_data;
		bus_read <= 0;
	end
	end
end // always @(posedge state_clock)


Computer_System The_System (
	////////////////////////////////////
	// FPGA Side
	////////////////////////////////////

	// Global signals
	.system_pll_ref_clk_clk					(CLOCK_50),
	.system_pll_ref_reset_reset			(1'b0),
	.sdram_clk_clk								(state_clock),

	// AV Config
	.av_config_SCLK							(FPGA_I2C_SCLK),
	.av_config_SDAT							(FPGA_I2C_SDAT),

	// Audio Subsystem
	.audio_pll_ref_clk_clk					(CLOCK3_50),
	.audio_pll_ref_reset_reset				(1'b0),
	.audio_clk_clk								(AUD_XCK),
	.audio_ADCDAT								(AUD_ADCDAT),
	.audio_ADCLRCK								(AUD_ADCLRCK),
	.audio_BCLK									(AUD_BCLK),
	.audio_DACDAT								(AUD_DACDAT),
	.audio_DACLRCK								(AUD_DACLRCK),

	// bus-master state machine interface
	.bus_master_audio_external_interface_address     (bus_addr),     
	.bus_master_audio_external_interface_byte_enable (bus_byte_enable), 
	.bus_master_audio_external_interface_read        (bus_read),        
	.bus_master_audio_external_interface_write       (bus_write),       
	.bus_master_audio_external_interface_write_data  (bus_write_data),  
	.bus_master_audio_external_interface_acknowledge (bus_ack),                                  
	.bus_master_audio_external_interface_read_data   (bus_read_data)/*,   
	
	
	////////////////////////////////////
	// HPS Side
	////////////////////////////////////
	// DDR3 SDRAM
	.memory_mem_a			(HPS_DDR3_ADDR),
	.memory_mem_ba			(HPS_DDR3_BA),
	.memory_mem_ck			(HPS_DDR3_CK_P),
	.memory_mem_ck_n		(HPS_DDR3_CK_N),
	.memory_mem_cke		(HPS_DDR3_CKE),
	.memory_mem_cs_n		(HPS_DDR3_CS_N),
	.memory_mem_ras_n		(HPS_DDR3_RAS_N),
	.memory_mem_cas_n		(HPS_DDR3_CAS_N),
	.memory_mem_we_n		(HPS_DDR3_WE_N),
	.memory_mem_reset_n	(HPS_DDR3_RESET_N),
	.memory_mem_dq			(HPS_DDR3_DQ),
	.memory_mem_dqs		(HPS_DDR3_DQS_P),
	.memory_mem_dqs_n		(HPS_DDR3_DQS_N),
	.memory_mem_odt		(HPS_DDR3_ODT),
	.memory_mem_dm			(HPS_DDR3_DM),
	.memory_oct_rzqin		(HPS_DDR3_RZQ),
		  
	// Ethernet
	.hps_io_hps_io_gpio_inst_GPIO35	(HPS_ENET_INT_N),
	.hps_io_hps_io_emac1_inst_TX_CLK	(HPS_ENET_GTX_CLK),
	.hps_io_hps_io_emac1_inst_TXD0	(HPS_ENET_TX_DATA[0]),
	.hps_io_hps_io_emac1_inst_TXD1	(HPS_ENET_TX_DATA[1]),
	.hps_io_hps_io_emac1_inst_TXD2	(HPS_ENET_TX_DATA[2]),
	.hps_io_hps_io_emac1_inst_TXD3	(HPS_ENET_TX_DATA[3]),
	.hps_io_hps_io_emac1_inst_RXD0	(HPS_ENET_RX_DATA[0]),
	.hps_io_hps_io_emac1_inst_MDIO	(HPS_ENET_MDIO),
	.hps_io_hps_io_emac1_inst_MDC		(HPS_ENET_MDC),
	.hps_io_hps_io_emac1_inst_RX_CTL	(HPS_ENET_RX_DV),
	.hps_io_hps_io_emac1_inst_TX_CTL	(HPS_ENET_TX_EN),
	.hps_io_hps_io_emac1_inst_RX_CLK	(HPS_ENET_RX_CLK),
	.hps_io_hps_io_emac1_inst_RXD1	(HPS_ENET_RX_DATA[1]),
	.hps_io_hps_io_emac1_inst_RXD2	(HPS_ENET_RX_DATA[2]),
	.hps_io_hps_io_emac1_inst_RXD3	(HPS_ENET_RX_DATA[3]),

	// Flash
	.hps_io_hps_io_qspi_inst_IO0	(HPS_FLASH_DATA[0]),
	.hps_io_hps_io_qspi_inst_IO1	(HPS_FLASH_DATA[1]),
	.hps_io_hps_io_qspi_inst_IO2	(HPS_FLASH_DATA[2]),
	.hps_io_hps_io_qspi_inst_IO3	(HPS_FLASH_DATA[3]),
	.hps_io_hps_io_qspi_inst_SS0	(HPS_FLASH_NCSO),
	.hps_io_hps_io_qspi_inst_CLK	(HPS_FLASH_DCLK),

	// Accelerometer
	.hps_io_hps_io_gpio_inst_GPIO61	(HPS_GSENSOR_INT),

	//.adc_sclk                        (ADC_SCLK),
	//.adc_cs_n                        (ADC_CS_N),
	//.adc_dout                        (ADC_DOUT),
	//.adc_din                         (ADC_DIN),

	// General Purpose I/O
	.hps_io_hps_io_gpio_inst_GPIO40	(HPS_GPIO[0]),
	.hps_io_hps_io_gpio_inst_GPIO41	(HPS_GPIO[1]),

	// I2C
	.hps_io_hps_io_gpio_inst_GPIO48	(HPS_I2C_CONTROL),
	.hps_io_hps_io_i2c0_inst_SDA		(HPS_I2C1_SDAT),
	.hps_io_hps_io_i2c0_inst_SCL		(HPS_I2C1_SCLK),
	.hps_io_hps_io_i2c1_inst_SDA		(HPS_I2C2_SDAT),
	.hps_io_hps_io_i2c1_inst_SCL		(HPS_I2C2_SCLK),

	// Pushbutton
	.hps_io_hps_io_gpio_inst_GPIO54	(HPS_KEY),

	// LED
	.hps_io_hps_io_gpio_inst_GPIO53	(HPS_LED),

	// SD Card
	.hps_io_hps_io_sdio_inst_CMD	(HPS_SD_CMD),
	.hps_io_hps_io_sdio_inst_D0	(HPS_SD_DATA[0]),
	.hps_io_hps_io_sdio_inst_D1	(HPS_SD_DATA[1]),
	.hps_io_hps_io_sdio_inst_CLK	(HPS_SD_CLK),
	.hps_io_hps_io_sdio_inst_D2	(HPS_SD_DATA[2]),
	.hps_io_hps_io_sdio_inst_D3	(HPS_SD_DATA[3]),

	// SPI
	.hps_io_hps_io_spim1_inst_CLK		(HPS_SPIM_CLK),
	.hps_io_hps_io_spim1_inst_MOSI	(HPS_SPIM_MOSI),
	.hps_io_hps_io_spim1_inst_MISO	(HPS_SPIM_MISO),
	.hps_io_hps_io_spim1_inst_SS0		(HPS_SPIM_SS),

	// UART
	.hps_io_hps_io_uart0_inst_RX	(HPS_UART_RX),
	.hps_io_hps_io_uart0_inst_TX	(HPS_UART_TX),

	// USB
	.hps_io_hps_io_gpio_inst_GPIO09	(HPS_CONV_USB_N),
	.hps_io_hps_io_usb1_inst_D0		(HPS_USB_DATA[0]),
	.hps_io_hps_io_usb1_inst_D1		(HPS_USB_DATA[1]),
	.hps_io_hps_io_usb1_inst_D2		(HPS_USB_DATA[2]),
	.hps_io_hps_io_usb1_inst_D3		(HPS_USB_DATA[3]),
	.hps_io_hps_io_usb1_inst_D4		(HPS_USB_DATA[4]),
	.hps_io_hps_io_usb1_inst_D5		(HPS_USB_DATA[5]),
	.hps_io_hps_io_usb1_inst_D6		(HPS_USB_DATA[6]),
	.hps_io_hps_io_usb1_inst_D7		(HPS_USB_DATA[7]),
	.hps_io_hps_io_usb1_inst_CLK		(HPS_USB_CLKOUT),
	.hps_io_hps_io_usb1_inst_STP		(HPS_USB_STP),
	.hps_io_hps_io_usb1_inst_DIR		(HPS_USB_DIR),
	.hps_io_hps_io_usb1_inst_NXT		(HPS_USB_NXT)
	*/
);
	
	/*
reg signed [15:0] sampleleft;
reg signed [15:0] sampleright;
	
always_ff @(posedge CLOCK_50, posedge reset)
   if (reset == 1'b1) begin
         state <= state_init;
         write_s <= 1'b0;
	end else if (should_pause == 1'b1) begin
		// do nothing
	end else begin
      case (state)
		state_init: begin
			write_s <= 1'b0;
			read_s <= 1'b0;
			state <= state_wait_until_ready;
		end // state_init
		state_wait_until_read_ready: begin
			read_s <= 1'b1;
         if (read_ready == 1'b1)
				state <= state_read_sample;
			end // state_wait_until_read_ready		
		state_read_sample: begin
			read_s <= 1'b0;
			sampleleft <= readdata_left;
			sampleright <= readdata_right;
			state <= state_wait_until_ready;
		end // state_read_sample
		state_wait_until_ready: begin 
			write_s <= 1'b0;
         if (write_ready == 1'b1)  
	          state <= state_send_sample;
		end // state_wait_until_ready	
		state_send_sample: begin
		

				writedata_right <= sampleright/64;
				writedata_left <= sampleleft/64;
				
			   write_s <= 1'b1;  // indicate we are writing a value
				
            state <= state_wait_for_accepted;
		end // state_send_sample	
		state_wait_for_accepted: begin
					// now we have to wait until the core has accepted
	             // the value. We will know this has happened when
	             // write_ready goes to 0.   Once it does, we can 
					 // go back to the top, set write_s to 0, and 
					 // wait until the core is ready for a new sample.
					 
				if (write_ready == 1'b0) 
					state <= state_wait_until_ready;
			end // state_wait_for_accepted
		default: begin
				    // should never happen, but good practice
                state <= state_init;
				 end // default
			endcase
     end //endif
	*/
	/*
always_ff @(posedge CLOCK_50, posedge reset)
   if (reset == 1'b1) begin
         state <= state_init;
         write_s <= 1'b0;
	end else if (should_pause == 1'b1) begin
		// do nothing
	end else begin
      case (state)
		state_init: begin
			flash_mem_address = 23'b00000000000000000000000;
			flash_mem_read = 1'b0;
			address = 8'b00000000;
			wren = 1'b0;
			count = 0;
			write_s <= 1'b0;
			state <= state_assert_address;
		end // case state_init
			state_assert_address: begin
			flash_mem_read = 1'b1;
			state <= state_readdatavalid;
		end
		state_readdatavalid: begin
			if (flash_mem_readdatavalid == 1'b1) begin
				flash_mem_read = 1'b0;
				state <= state_read_address_write_sample1;
			end else begin
				state <= state_readdatavalid;
			end
		end
		state_read_address_write_sample1: begin
			samplepair = flash_mem_readdata;
			wren = 1'b1;
			data = samplepair[15:0];
			state <= state_write_sample2;
		end
		state_write_sample2: begin
			address = address + 1'b1;
			wren = 1'b1;
			data = samplepair[31:16];
			state <= state_increment_count;
		end
		state_increment_count: begin
			wren = 1'b0;
			count = count + 1;
			samplenumber = 1;
			loopnumber = 1;
			if (count < 1048576) begin
				flash_mem_address = flash_mem_address + 1'b1;
				address = address + 1'b1;
				state <= state_wait_until_ready;
			end else begin
				flash_mem_address = 23'b00000000000000000000000;
				flash_mem_read = 1'b0;
				address = 8'b00000000;
				count = 0;
				state <= state_wait_until_ready;
			end
		end
         state_wait_until_ready: begin
				 
				    // In this state, we set write_s to 0,
					 // and wait for write_ready to become 1.
					 // The write_ready signal will go 1 when the FIFOs
					 // are ready to accept new data.  We can't do anything
					 // until this signal goes to a 1.
					 
				write_s <= 1'b0;
                if (write_ready == 1'b1)  
	                 state <= state_send_sample;
				end // state_wait_until_ready				   
   
         state_send_sample: begin
				 
				    // Now that the core has indicated that it is ready to 
					 // accept a sample, send one.  In this case, our samples are
					 // calculated (rather than read from the flash memory)
					
					if (samplenumber == 1) begin
						sample = samplepair[15:0];
					end else begin
						sample = samplepair[31:16];
					end
					
					// divide sample by 64 to lower volume
					sample = sample/64;
					
					sound_out <= sample;
                // send the sample to the core (it is added to the two FIFOs
                // as explained in the handout.  We need to be sure to send data
					 // to both the right and left queue.  Since we are only playing a
					 // mono sound (not stereo) we send the same sample to both FIFOs.
					 // You will do the same in your implementation in the final task.
					 
					 if (SW[9] == 1) begin
					 
						distanceToGo = {sample, 16'b0000000000000000} - withoutTreble;
						withoutTreble = withoutTreble + (distanceToGo >>> 3);
						trebleOnly = {sample, 16'b0000000000000000} - withoutTreble;
						
						distanceToGo = withoutTreble - bassOnly;
						bassOnly = bassOnly + (distanceToGo >>> 3);
						
						lastOutput = withoutTreble + trebleOnly * treble + bassOnly * bass;

						writedata_left <= lastOutput[31:16];
						writedata_right <= lastOutput[31:16];
					 end else if (SW[8] == 1) begin
					 
						writedata_right <= sound_in;
						writedata_left <= sound_in;
						
					 end else begin
					 	writedata_right <= sample;
						writedata_left <= sample;
					 end
			       write_s <= 1'b1;  // indicate we are writing a value
                state <= state_wait_for_accepted;
				   end // state_send_sample
					
		       state_wait_for_accepted: begin

                // now we have to wait until the core has accepted
	             // the value. We will know this has happened when
	             // write_ready goes to 0.   Once it does, we can 
					 // go back to the top, set write_s to 0, and 
					 // wait until the core is ready for a new sample.
					 
				    if (write_ready == 1'b0) 
						if(samplenumber == 1) begin
							if (mode == slow) begin					// if mode == slow, samplenumber remains the same for 2 cycles, seen by loopnumber
								if (loopnumber == 1) begin
									loopnumber = 2;
									samplenumber = 1;
								end else begin
									loopnumber = 1;
									samplenumber = 2;
								end
								state <= state_wait_until_ready;
							end else begin
								samplenumber = 2;
								if (mode == normal) begin
									state <= state_wait_until_ready;
								end else if (mode == fast) begin
									state <= state_assert_address;
								end 
							end
						end else begin
							if (mode == slow) begin
								if (loopnumber == 1) begin
									loopnumber = 2;
									samplenumber = 2;
									state <= state_wait_until_ready;
								end else begin
									loopnumber = 1;
									samplenumber = 1;
									state <= state_assert_address;
								end
							end else begin
								state <= state_assert_address;
							end
						end
						
					end // state_wait_for_accepted
					
	          default: begin
				 
				    // should never happen, but good practice
					 
                state <= state_init;
					 
				 end // default
			endcase
     end  // if 
*/

endmodule

