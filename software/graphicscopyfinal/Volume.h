#define TxData 		(*(volatile unsigned char *)(0x84000202))
#define RxData 		(*(volatile unsigned char *)(0x84000202))
#define Status 		(*(volatile unsigned char *)(0x84000200))
#define Control 	(*(volatile unsigned char *)(0x84000200))
#define Baud    	(*(volatile unsigned char *)(0x84000204))
#define pot		 	(*(volatile signed short *)(0x00000000))


short getSensorReading(void);
char getCharArduino(void);
void volume_control(char* data_in, short* reading_in, short* reading_in_prev, short* reading_in_prev_1, short* reading_in_next);
void Init_Volume(void);
void setVolume(void);
