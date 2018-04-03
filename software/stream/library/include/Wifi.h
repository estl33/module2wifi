#ifndef WIFI_H_
#define WIFI_H_

#include <string>

using namespace std;

#define Wifi_Control (*(volatile unsigned char *)(0x84000220))
#define Wifi_Status (*(volatile unsigned char *)(0x84000220))
#define Wifi_TxData (*(volatile unsigned char *)(0x84000222))
#define Wifi_RxData (*(volatile unsigned char *)(0x84000222))
#define Wifi_Baud 	(*(volatile unsigned char *)(0x84000224))

class Wifi {
public:
	static Wifi* getInstance();
	void init();
	void write(string);
	string* readLine();
	int isDataAvailable();
private:
	void putChar(char c);
	char getChar();
};

#endif
