#ifndef MAIN_ACTIVITY_H_
#define MAIN_ACTIVITY_H_

#include <string>

using namespace std;

#include "Application.h"

class MainActivity : public Activity {
public:
  void onCreate();
  string getName();
};

#endif
