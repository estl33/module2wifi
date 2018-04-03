#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Gps.h"

//int main() {
//
//	initGPS();
//	gps_data_t *gps_data = malloc(sizeof(gps_data));
//	gps_data->headerSize = sizeof(GPSHEADER)-1;
//	gps_data->header_i = 0;
//	gps_data->gotGPSData = false;
//	gps_data->gotHeader = false;
//	gps_data->inputHeader[gps_data->headerSize] = '\0';
//
//	while (!gps_data->gotGPSData) {
//		getGPSInfo(gps_data);
//	}
//
//	printf("Time: %s\n", gps_data->time_utc);
//	printf("Latitude: %s\n", gps_data->latitude);
//	printf("N/S: %s\n", gps_data->NS);
//	printf("Longitude: %s\n", gps_data->longitude);
//	printf("E/W: %s\n", gps_data->EW);
//	printf("before parse \n");
//	//Parse and convert time zone
//	parseTime(gps_data);
//	printf("after parse \n");
//	return 0;
//}

void initGPS(void){
	// set up 6850 Control Register to utilise a divide by 16 clock,
	// set RTS low, use 8 bits of data, no parity, 1 stop bit,
	// program baud rate generator to use 9600 baud
	GPSBaud = 0x05;
	GPSControl = 0x15; // 0001 0101
}

void putcharGPS(char c) {
	//wait for tx register to be empty
	while (!(GPSStatus & 0x2)) { }
	GPSTxData = c;
}

char getcharGPS(void){
	// wait for rx register to have data
	while (!(GPSStatus & 0x1)) { }
	return GPSRxData;
}

//for testing
void readGPSDataToConsole() {
	char c;
	while (1) {
		c = getcharGPS();
		printf("%c", c);
	}
}

//Parse utc time to vancouver time and store in gps data struct
void parseTime(gps_data_t *gps_data) {

	int hour = 0;
	int minute = 0;
	int second = 0;
	char *time_utc = gps_data->time_utc;

	hour = 10 * (time_utc[0] - '0') + (time_utc[1] - '0');
	minute = 10 * (time_utc[2] - '0') + (time_utc[3] - '0');
	second = 10 * (time_utc[4] - '0') + (time_utc[5] - '0');

	if(hour > 8){
		hour = hour - 8;
	}
	else {
		hour = hour + 16;
	}

	if (hour > 12) {
		hour = hour - 12;
		gps_data->AM_PM[0] = 'P';

	}
	else {
		gps_data->AM_PM[0] = 'A';
	}


	gps_data->AM_PM[1] = 'M';
	gps_data->AM_PM[2] = '\0';

	gps_data->hour = hour;
	gps_data->minute = minute;
	gps_data->second = second;
	gps_data->gotParsedTime = true;
	printf("Time array utc: %s\n", gps_data->time_utc);
	printf("Hour: %d:%d:%d %s\n", hour, minute, second, gps_data->AM_PM);


}

//This function gets ONE READING of the gps data and stores it in gps data struct
void getGPSInfo(gps_data_t *gps_data) {

	//wait for the GPGGA header
	//printf("gotheader is %d\n", gps_data->gotHeader);
	if (!gps_data->gotHeader) {
		//printf("in loop for header\n");
		if (gps_data->header_i < 6){//gps_data->headerSize) {
			gps_data->inputHeader[gps_data->header_i] = getcharGPS();

			if(gps_data->inputHeader[gps_data->header_i] == '$'){
				gps_data->header_i = 0;
				gps_data->inputHeader[0] = '$';
			}
			gps_data->header_i += 1;
		}
		else {
			gps_data->header_i = 0;
			//printf("comparing headers input is '%s'\n", gps_data->inputHeader);
			if (!strcmp(GPSHEADER, gps_data->inputHeader)) {
				gps_data->gotHeader = true;
				gps_data->csv_i = 0;
				gps_data->char_i = 0;
				gps_data->gotGPSData = false;
				printf("got to header");
			}
		}
	}
	else {
		char input = getcharGPS();

		if (input == ',') {
			input = getcharGPS();
			gps_data->csv_i += 1;
			gps_data->char_i = 0;
		}
		else if (input == '*') {
				gps_data->time_utc[10] = '\0';
				gps_data->latitude[9] = '\0';
				gps_data->NS[1] = '\0';
				gps_data->longitude[10] = '\0';
				gps_data->EW[1] = '\0';
				gps_data->gotGPSData = true;
				return;
		}
		else {
			gps_data->char_i += 1;
		}
		//printf("got to csv parsing csv is %d\n", gps_data->csv_i);
		if (gps_data->csv_i == 1) {
			gps_data->time_utc[gps_data->char_i] = input;
		}
		else if (gps_data->csv_i == 2) {
			gps_data->latitude[gps_data->char_i] = input;
		}
		else if (gps_data->csv_i == 3) {
			gps_data->NS[gps_data->char_i] = input;
		}
		else if (gps_data->csv_i == 4) {
			gps_data->longitude[gps_data->char_i] = input;
		}
		else if (gps_data->csv_i == 5) {
			gps_data->EW[gps_data->char_i] = input;
		}
	}

}
