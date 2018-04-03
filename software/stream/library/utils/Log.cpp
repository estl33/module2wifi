#include "Log.h"

void __Log_Message(string header, string msg, int level) {
  switch (level) {
    case ERROR:
    std::cerr << "ERROR - "<< header <<": " << msg << "\n";
    break;
    case WARN:
    std::cout << "WARN - "<< header <<": " << msg << "\n";
    break;
    case INFO:
    std::cout << "INFO - "<< header <<": " << msg << "\n";
    break;
    case LOW:
    std::cout << "LOW - "<< header <<": " << msg << "\n";
    break;
  }
}
