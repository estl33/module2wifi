#include "Serial.h"

void Serial::init() {
#ifdef SIM_CODE
  serialPort = new SerialPort("/dev/pts/2", BaudRate::B_57600);
  serialPort->Open();
#else
  //TODO
#endif
}

void Serial::write(string str) {
#ifdef SIM_CODE
  serialPort->Write(str);
#else
  //TODO
#endif
}

string Serial::readLine() {
  return "";
}

int isDataAvailable() {
  return 0;
}
