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

void Init_BlueTooth(void);
int putcharBlueTooth(int c);
int getcharBlueTooth( void );
void sendStringBlueTooth(char* str);
void sendCommand(char* str);

int main()
{
	/*
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
*/
  // BlueTooth
  // Initialize BlueTooth
  Init_BlueTooth();

  // Enter command mode with $$$, these do not need \r\n.
  // Wait 1 second before and after calling it
  usleep(1000 * 1000 * 1);
  printf("Done Sleeping!\n");
  sendStringBlueTooth("$$$");
  usleep(1000 * 1000 * 1);
  printf("Done Sleeping!\n");

  // Send commands:
  // If not the last command, only send \r. If you add \n the rest of the commands may not work
  // Set authorization to 4, this makes it so your phone will be asked for a pin
  sendStringBlueTooth("SA,4\r");
  // Set the device name
  sendStringBlueTooth("SN,Gord5\r");
  // Set the pin
  sendStringBlueTooth("SP,9999\r");

  // This is superstitious 1 second waiting... might not be needed
  usleep(1000 * 1000 * 1);

  // Reboot the chip so the changes will show up
  sendStringBlueTooth("R,1\r\n");

  // This is not needed right now, but should be used if one wants to exit command mode without rebooting
//  sendStringBlueTooth("---\r");

  return 0;
}

// Initializes the BlueTooth Control register to 115k baud, 8 bits, no parity, 1 stop bit, 16 divisor
void Init_BlueTooth(void) {
	BlueTooth_Control = 0b00010101;
	BlueTooth_Baud = 0x01;
}

// Sends str, which should be a pointer to an array of characters to the bluetooth chip
void sendStringBlueTooth(char* str) {
	char *c = str;
	// iterate until end of str
	while (*c != '\0') {
		// add 2ms delay between sending characters
		usleep(1000 * 2);
		// send the character
		putcharBlueTooth(*c);
		c++;
		usleep(1000 * 2);
	}
}

// Waits for BlueTooth_Status TxBit to be 1 and puts c into BlueTooth_TxData
int putcharBlueTooth(int c)
{
	printf("PutChar: %c, %d\n", c, c);
	printf("PutChar BlueTooth_Status 1 = %#010x\n", BlueTooth_Status);

	 // poll Tx bit in 6850 status register. Wait for it to become '1'
	while((BlueTooth_Status & 0x02) % 4 == 0){
		printf("PutChar BlueTooth_Status 2 = %#010x\n", BlueTooth_Status);
	}

	printf("PutChar BlueTooth_Status 3 = %#010x\n", BlueTooth_Status);

	// write 'c' to the 6850 TxData register to output the character
	BlueTooth_TxData = c;
	return c ; // return c
}

// Waits for BlueTooth_Status RxBit to be 1 and reads from BlueTooth_RxData
int getcharBlueTooth( void )
{
	printf("GetCharBlueTooth 1 BlueTooth_Status = %#010x\n", BlueTooth_Status);

	// poll Rx bit in 6850 status register. Wait for it to become '1'
	while((BlueTooth_Status & 0x01) % 2 == 0){
		printf("GetCharBlueTooth 2 = %#010x\n", BlueTooth_Status);
	}

	printf("GetCharBlueTooth 3 = %#010x\n", BlueTooth_Status);

	// read received character from 6850 RxData register.
	int c = BlueTooth_RxData;
	printf("GetCharBluetooth: %c, %#010x\n", c, c);
	return c;
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
 while(RS232_Status & 0x02 != 0x02){
	 printf("RS232_Status = %#010x\n", RS232_Status);
 }
 // write 'c' to the 6850 TxData register to output the character
 RS232_TxData = c;
 return c ; // return c
}

int getcharRS232( void )
{
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 while(RS232_Status & 0x01 != 0x01){
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
 int Rxbit = RS232_Status & 0x01;
 // if RX bit is set, return TRUE, otherwise return FALSE
 return (Rxbit == 1);
}
