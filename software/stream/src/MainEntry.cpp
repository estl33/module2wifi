//#include "Application.h"

//#include "MainActivity.h"

int main(int argc, char const *argv[]) {

 // Application app = Application();
 // MainActivity startActivity = MainActivity();

//  app.startApplication(&startActivity);

  WifiActivity radioStream = WifiActivity();
  string reply = radioStream::updateStream();
  printf("%s\n",reply);
  //We should never be going here...
  return 0;
}
