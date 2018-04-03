/*
 * WifiPi.h
 *
 *  Created on: Mar 27, 2018
 *      Author: ???
 */

#ifndef WIFIPI_H_
#define WIFIPI_H_

void wifiOpen();
char* wifiInit(char* isPlayingVal, char* stationId, char* userId);
void piInit(char* streamInfo);

#endif /* WIFIPI_H_ */
