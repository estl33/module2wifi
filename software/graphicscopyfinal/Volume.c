#include <time.h>
#include <stdio.h>
#include <math.h>
#include "Volume.h"


void volume_control(char* data_in, short* reading_in, short* reading_in_prev, short* reading_in_prev_1, short* reading_in_next) {
	printf("volume control\n");
		printf("while loop\n");
		pot = *reading_in;
		*reading_in_next = getSensorReading();
		printf("reading_in_next :%d\n", *reading_in_next);
		printf("reading_in_prev_1 :%d\n", *reading_in_prev_1);
		printf("Absolute value of reading_in_next and reading_in_prev1: %d\n", abs(*reading_in_next - *reading_in_prev_1));
		if(abs(*reading_in_next - *reading_in_prev_1) > 10) {
			if(*reading_in_next > *reading_in_prev_1) *reading_in = *reading_in_next;
			else {
				printf("reset next reading to prev_1\n");
				*reading_in = *reading_in_prev_1;
			}
		}
		else
			*reading_in = *reading_in_next;
		printf("reading_in_maxdiff :%d\n", *reading_in);
		//reading_in = (short) (reading_in_next + reading_in_prev) / 2;
		if(abs(*reading_in) > 500 ) *reading_in = *reading_in_prev_1;
		printf("reading_in_capped :%d\n", *reading_in);
		*reading_in_prev_1 = *reading_in_prev;
		*reading_in_prev = *reading_in_next;

}

void Init_Volume(){
	printf("Init volume\n");
	Baud = 0x05;
	Control = 0x15;
}
short getSensorReading(){
	short data = 0;
	char data_in = getCharArduino();
	//if(data_in == 'N') return 350;
	short data_in_num = data_in - '0';
	while(data_in != ','){
		data = 10*data + (data_in_num);
		data_in = getCharArduino();
		//if(data_in == 'N') return 350;
		data_in_num = data_in - '0';
	}
	return data;
}

char getCharArduino(){
	int i = 0;
	while (!(Status & 0x1)) {
		//return 'N';
	}
	return RxData;
}

void setVolume(){
	short reading = getSensorReading();
	short reading1 = getSensorReading();
	short reading2 = getSensorReading();

	char c = RxData;


	short average = (reading + reading1 + reading2)/3;

	if(reading > average){
		pot = reading;
	}
	else if(reading1 > average){
		pot = reading1;
	}
	else if(reading2 > average){
		pot = reading2;
	}

	//short finalValue = pot;
	//printf("Volume: %d\n", finalValue);
	usleep(10000);

}

