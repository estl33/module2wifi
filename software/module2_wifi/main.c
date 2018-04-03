#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WifiPi.h"


//char* isPlayingVal;
//char* currentStreamVal;

char* streamInfo;
int main()
{
	printf("Hello from Nios II!\n");
	const char *isPlayingVal = "1";
	const char *stationId = "\"5ac3cf44ea869483719d8e41\"";
	const char *userId = "\"5ac3d0d38ac834137ba00c4a\"";

	wifiOpen();


	while(1){
		streamInfo = wifiInit(isPlayingVal, stationId, userId);
		piInit(streamInfo);
	}
	return 0;
}

void wifiOpen(){
	const char *dofile_command = "dofile(\"de1_to_server.lua\")\r\n";
	FILE *f = fopen("/dev/wifi_uart", "r+");
	printf("FILE OPENED - WIFI!\n");
	if(f){
		fwrite(dofile_command, strlen(dofile_command), 1, f); // Write a string
		printf("Executing de1_to_server.lua\n");
		fclose(f);
	}
}

char* wifiInit(char* isPlayingVal, char* stationId, char* userId){

	//const char *dofile_command = "dofile(\"de1_to_server.lua\")\r\n";

//	char *update_command = "post_to_server(\"1\",\"5ac3cf44ea869483719d8e41\",\"5ac3d0d38ac834137ba00c4a\")\r\n";
	char *update_command = "post_to_server(\"";
	strcat(update_command, isPlayingVal);
	strcat(update_command, "\", \"");
    strcat(update_command, stationId);
	strcat(update_command, "\", \"");
	strcat(update_command, userId);
	strcat(update_command, "\")\r\n");

		FILE *f = fopen("/dev/wifi_uart", "r+");
		printf("FILE OPENED - WIFI!\n");

		char buffer[1234];
		static char streamInfo[1234];

	  if(f) {
		//fgets(buffer, 50, f);
		fwrite("\r\n", strlen("\r\n"), 1, f);
		fwrite(update_command, strlen(update_command), 1, f);
		printf("POST TO SERVER\n");

		if(ftell(f)!= 0){
		fgets(buffer, 50,f);
		printf("buffer! %s\n", buffer);
		}

		while(strstr(buffer,"mssg") == NULL){
			//printf("buffer! %s\n", buffer);
			fgets(buffer, 70,f);
			if(strstr(buffer,"closed") != NULL){
				fwrite(update_command, strlen(update_command), 1, f);
			}
		}
		printf("FILE BUFFER! %s\n", buffer);
		strcpy(streamInfo, buffer);
		printf("FILE STREAM: %s\n", streamInfo);


		if(strstr(buffer,"mssg") != NULL){
			while(strstr(buffer, "png\"}}")== NULL){
				fgets(buffer, 100, f);
				strcat(streamInfo, buffer);
			}
		}

		printf("STREAM STATUS & URL: %s\n", streamInfo);

		fclose (f);

	  }

	  if(!f){
		  printf("Error: Can't open wifi_uart file\n");
	  }
	  printf("Finished Wifi!\n");

	  return streamInfo;

}

void piInit(char* streamInfo){

	FILE *f = fopen("/dev/pi_uart", "r+");
	printf("FILE OPENED - PI!\n");

	char buffer[1234];
	char* info = strcpy(info,streamInfo);


	if(f) {
		//fwrite(streamInfo, strlen(streamInfo), 1, f); // Write a string
		fwrite(info, strlen(info), 1, f);
		printf("SENT TO PI! %s\n", info);
		//fread(buffer, 30, 1, f);
		//printf("FILE READ! %s\n", buffer);


		fclose (f);
	}

	if(!f){
		printf("Error: Can't open wifi_uart file\n");
	}
	printf("Finished Pi!\n");

}

