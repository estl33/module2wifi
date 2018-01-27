#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	(*(volatile unsigned char *)(0x84000204))


#define BlueTooth_Control (*(volatile unsigned char *)(0x84000220))
#define BlueTooth_Status (*(volatile unsigned char *)(0x84000220))
#define BlueTooth_TxData (*(volatile unsigned char *)(0x84000222))
#define BlueTooth_RxData (*(volatile unsigned char *)(0x84000222))
#define BlueTooth_Baud 	(*(volatile unsigned char *)(0x84000224))

#include <time.h>
#include <stdio.h>

void Init_RS232(void);
int putcharRS232(int c);
int getcharRS232( void );
int RS232TestForReceivedData(void);

int main()
{
  printf("Hello from Nios II!\n");
  Init_RS232();
  printf("Finished init RS232\n");
  int wValue = putcharRS232(3);
  printf("Finished putcharRS232. wValue = %d\n", wValue);
  int rValue = getcharRS232();
  printf("Finished getcharRS232. rValue = %d\n", rValue);

  wValue = putcharRS232(255);
  printf("Finished putcharRS232. wValue = %d\n", wValue);
  rValue = getcharRS232();
  printf("Finished getcharRS232. rValue = %d\n", rValue);

  wValue = putcharRS232(50);
  printf("Finished putcharRS232. wValue = %d\n", wValue);
  rValue = getcharRS232();
  printf("Finished getcharRS232. rValue = %d\n", rValue);

  // BlueTooth
  // Initialize BlueTooth
  BlueTooth_Control = 0b00010100;
  BlueTooth_Baud = 0x01;

  // wait 1 second
  usleep(1000 * 1000 * 1);
  printf("Done Sleeping!\n");

  while (BlueTooth_Status && 0x02 != 0x02) {
	  printf("Waiting for TxBit to be 1 to send $$$\n");
  }
  BlueTooth_TxData = "$$$";

  usleep(1000 * 1000 * 1);
  printf("Done Sleeping!\n");

  while (BlueTooth_Status && 0x01 != 0x01) {
	  printf("Waiting for RxBit to be 1 to receive $$$ result\n");
  }
  int c = BlueTooth_RxData;
  printf("Char received from $$$ = %d\n", c);

  while (BlueTooth_Status && 0x02 != 0x02) {
	  printf("Waiting for TxBit to be 1 to sent command\n");
  }
  BlueTooth_TxData = "SN,GordonsDevice\r\n";

  while (BlueTooth_Status && 0x01 != 0x01) {
	  printf("Waiting for RxBit to be 1 to receive command result\n");
  }
  c = BlueTooth_RxData;
  printf("Char received from command = %d\n", c);

  return 0;
}

/**************************************************************************
/* Subroutine to initialise the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to 6850 data sheet for details of registers and
***************************************************************************/
void Init_RS232(void)
{
 // set up 6850 Control Register to utilise a divide by 16 clock,
 // set RTS low, use 8 bits of data, no parity, 1 stop bit,
 // transmitter interrupt disabled
 RS232_Control = 0b00010101;
 // program baud rate generator to use 115k baud
 RS232_Baud = 0x01;
}

int putcharRS232(int c)
{
 // poll Tx bit in 6850 status register. Wait for it to become '1'
 while(RS232_Status && 0x02 != 0x02){
	 printf("RS232_Status = %#010x\n", RS232_Status);
 }
 // write 'c' to the 6850 TxData register to output the character
 RS232_TxData = c;
 return c ; // return c
}

int getcharRS232( void )
{
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 while(RS232_Status && 0x01 != 0x01){
	 printf("RS232_Status = %#010x\n", RS232_Status);
 }
 printf("RS232_Status = %#010x\n", RS232_Status);
 // read received character from 6850 RxData register.
 return RS232_RxData;
}

// the following function polls the 6850 to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read
int RS232TestForReceivedData(void)
{
 // Test Rx bit in 6850 serial comms chip status register
 int Rxbit = RS232_Status && 0x01;
 // if RX bit is set, return TRUE, otherwise return FALSE
 return (Rxbit == 1);
}
