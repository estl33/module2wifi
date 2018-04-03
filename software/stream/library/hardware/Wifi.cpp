#include "Wifi.h"

#include <unistd.h>

#include "Log.h"
#include "Assert.h"

Wifi* wifi_instance = 0;

Wifi* Wifi::getInstance(){
	M_Stack();
	if(wifi_instance == 0) {
		wifi_instance = new Wifi;
	}
	M_Unstack();
	return wifi_instance;
}

void Wifi::init() {
	M_Stack();
	Wifi_Control = 0b00010101;
	Wifi_Baud = 0x01;	//115k

	string command = "dofile(\"de1_to_server.lua\")\r\n";
	for(int i = 0; i < command.length(); i++){
		putChar(command[i]);
	}
	M_Unstack();
}

void Wifi::write(string message) {
	M_Stack();
	for(int i = 0; i < message.length(); i++) {
		putChar(message[i]);
	}
	putChar('\n');
	M_Unstack();
}

string* Wifi::readLine() {
	M_Stack();
	string* response = new string;
	while (!(Wifi_Status & 0x1));
	char next;

	while(next != '\n'){
	next = getChar();
	*response += next;
	//Log_Low("Response", *response);
	}

/*	while(next != '\n') {
		*response += next;
		next = getChar();
	}*/

	M_Unstack();
	return response;
}

char* Wifi::getReply(){
	M_Stack();
	char reply[100];
	int i = 0;
	while(!(Wifi_Status & 0x01));

	for(i = 0; i < 30; i++){
		reply[i] = getChar();
	}

	//printf("reply : %s\n", reply);
	M_Unstack();
	return reply;
}
int Wifi::isDataAvailable() {
  return !(Wifi_Status & 0x01);
}

void Wifi::putChar(char c) {
	M_Stack();
	while (!(Wifi_Status & 0x2));

	Wifi_TxData = c;
	//usleep(300);
	int i = 0;
	while(i>0){
		i--;
	}

	M_Unstack();
}

char Wifi::getChar() {
	M_Stack();
	while (!(Wifi_Status & 0x1));
	M_Unstack();
	return Wifi_RxData;
}
