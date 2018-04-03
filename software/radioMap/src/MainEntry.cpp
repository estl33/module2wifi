//#include "Application.h"

//#include "MainActivity.h"
#include <string>
//#include "WifiActivity.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

int main(int argc, char const *argv[]) {

 // Application app = Application();
 // MainActivity startActivity = MainActivity();

//  app.startApplication(&startActivity);

//  WifiActivity radioStream = WifiActivity();
//  radioStream.onCreate();
//  string reply;
//  reply = radioStream.updateStream("true","123str");
//  cout << reply;

	// write buffer to file
	    if(std::FILE* f1 = std::fopen("/dev/wifi_uart", "wb")) {
	        char* v = "dofile(\"de1_to_server.lua\")\r\n"; // underlying storage of std::array is an array
	        std::fwrite(v, sizeof v[0], 1, f1);
	        std::fclose(f1);
	    }

	    // read the same data and print it to the standard output
	    if(std::FILE *f2 = std::fopen("/dev/wifi_uart", "rb")) {
	        char rbuf[10]; // underlying storage of std::vector is also an array
	        std::size_t sz = std::fread(rbuf, sizeof rbuf[0], 1, f2);
	        std::fclose(f2);
	        for(std::size_t n = 0; n < sz; ++n) {
	            std::printf("%d\n", rbuf[n]);
	        }
	    }
  //We should never be going here...
  return 0;
}
