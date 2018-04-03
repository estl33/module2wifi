

#include <stdio.h>
#include <string.h>
#include "Wifi.h"

/*int main()
{
  printf("Hello from Nios II!\n");
  initWifi();
  printf("Finished Init_Wifi\n");

  waitWifi();
  waitWifi();
  waitWifi();
  waitWifi();

  dofile_command();


  waitWifi();
  waitWifi();


  wifi_setup_command();
  usleep(1000);
  url_get_command();

  return 0;
}*/


void initWifi(void){
	 Wifi_Control = 0b00010101;
	 Wifi_Baud = 0x01;	//115k
	 printf("Init wifi\n");
}

void putcharWifi(char c){

	while (!(Wifi_Status & 0x2));

	Wifi_TxData = c;
	printf("%c", c);

	//usleep(300);
	int i = 10000; //50000;
	while(i > 0){
		i--;
	}
}

char getcharWifi(void){

	while (!(Wifi_Status & 0x1));

	return Wifi_RxData;

}

void waitWifi(void){

	char command[] = "\r\n";
	int len = strlen(command);
	int i = 0;
	for(i = 0; i < len; i++){
		putcharWifi (command[i]);
	}
	printf("<\n");
}

void dofile_command(void){

	char command[] = "dofile(\"send_text_message.lua\")\r\n";
	int len = strlen(command);
	int i = 0;
	for(i = 0; i < len; i++){
		putcharWifi (command[i]);
	}
}


void dofile_server(void){
	char command[] = "dofile(\"de1_to_server.lua\")\r\n";
	int len = strlen(command);
	int i = 0;
	for(i = 0; i < len; i++){
		putcharWifi (command[i]);
	}
	//getreply();
}

void update_stream(char* isPlayingVal, char* currStreamVal){

	//printf("currstrmval %s\n", currentStreamVal);
	char command[] = "post_to_server(\"";

	strcat(command, isPlayingVal);
	strcat(command, "\", \"");
	strcat(command, currStreamVal);
	strcat(command, "\")\r\n");

	int len = strlen(command);
	int i = 0;
	for(i = 0; i < len; i++){
		putcharWifi (command[i]);
	}

//	getreply();
}


void getreply(void){


//	printf("status : %#010x\n", Wifi_Status);
//	printf("rx : %#010x\n", Wifi_RxData);

	char reply[100];
	int i = 0;
	while(!(Wifi_Status & 0x01));

	for(i = 0; i < 50; i++){
		reply[i] = getcharWifi();
		//reply[i] = Wifi_RxData;
	}

	printf("reply : %s\n", reply);

}

void getlongreply(int k){
	char reply[100];
	int i = 0;
	while(!(Wifi_Status & 0x01));

	//printf("entering reply getchar\n");
	for(i = 0; i < 75; i++){
		reply[i] = getcharWifi();
		//reply[i] = Wifi_RxData;
	}
	printf("reply : %s\n", reply);
}


void wifi_setup_command(void){
	char command[] = "wifi_setup()\r\n";
	int len = strlen(command);
	int i = 0;
	for(i = 0; i < len; i++){
		putcharWifi (command[i]);
	}
	getreply();
}


void wifi_commands_init(void){
	waitWifi();
	waitWifi();
	waitWifi();
	waitWifi();

	dofile_server();

	waitWifi();
	waitWifi();
	getreply();

	printf("All done wifi_commands_init\n");
}

