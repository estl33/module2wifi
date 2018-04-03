/*
 * Wifi.h
 *
 *  Created on: Mar 21, 2018
 *      Author: ???
 */

#ifndef WIFI_H_
#define WIFI_H_

//using GIO_1[32] and GPIO_1[34]
#define Wifi_Control (*(volatile unsigned char *)(0x84000220))
#define Wifi_Status (*(volatile unsigned char *)(0x84000220))
#define Wifi_TxData (*(volatile unsigned char *)(0x84000222))
#define Wifi_RxData (*(volatile unsigned char *)(0x84000222))
#define Wifi_Baud 	(*(volatile unsigned char *)(0x84000224))

//WIFI
void initWifi(void);
void putcharWifi(char c);
char getcharWifi(void);
void waitWifi(void);
void dofile_command(void);
void dofile_server(void);
void update_stream(char* isPlayingVal, char* currStreamVal);
void getreply(void);
void getlongreply(int k);


#endif /* WIFI_H_ */
