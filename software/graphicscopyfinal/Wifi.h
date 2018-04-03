/*
 * Wifi.h
 *
 *  Created on: Feb 13, 2018
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

void Init_Wifi(void);
void putcharWifi(char c);
char getcharWifi(void);
void waitWifi(void);
void dofile_command(void);
void check_wifi_command(void);
void wifi_setup_command(void);
void wifi_play_pause_command(void);
void wifi_ch1_command(void);
void wifi_ch2_command(void);
void wifi_ch3_command(void);
void wifi_commands_init(void);
void getreply(void);

#endif /* WIFI_H_ */
