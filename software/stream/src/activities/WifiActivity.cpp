#include "MainActivity.h"

#include <unistd.h>
#include <stdio.h>

#include "Log.h"

const char *dofile_command = "dofile(\"de1_to_server.lua\")\r\n";

void WifiActivity::onCreate() {
  Log_Low("WifiActivity", "OnCreate()");
  printf("Hello from Nios II!\n");

    FILE *f = fopen("/dev/wifi_uart", "r+");
    printf("FILE OPENED!\n");

    char buffer[1234];

    if(f) {
  	fwrite(dofile_command, strlen(dofile_command), 1, f); // Write a string
  	printf("FILE WRITTEN!\n");
  	fread(buffer, 20, 3, f);
  	printf("FILE READ! %s\n", buffer);
  	printf("Executing de1_to_server.lua\n");
  	usleep(100);

  	fclose (f);
    }

    if(!f){
  	  printf("Error: Can't open wifi_uart file\n");
    }

    printf("Finished!\n");


}

string WifiActivity::updateStream(string isPlayingVal, string currStreamVal) {

	printf("update stream\n");
    string streamInfo;
    //string *isPlayingVal = "true";
    //string *currStreamVal = "123str";

    string *update_command = "post_to_server(\"true\",\"123str\")\r\n";

	fwrite(update_command, strlen(update_command), 1, f);
  	printf("FILE WRITTEN!\n");
  	fread(streamInfo, 10, 3, f);

  	printf("Reply: \n%s\n", streamInfo);
  	return streamInfo;
}
