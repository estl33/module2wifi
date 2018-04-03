#ifndef SERIAL_H_
#define SERIAL_H_

#include <string>

using namespace std;

class Serial {
#ifdef SIM_CODE
  SerialPort* serialPort;
#else

#endif

public:
  void init();
  void write(string );
  string readLine();
  int isDataAvailable();
};

#endif
