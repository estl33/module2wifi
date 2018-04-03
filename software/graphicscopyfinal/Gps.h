#ifndef GPS_H_
#define GPS_H_

#define GPSHEADER "$GPGGA"
#define GPSStatus 		(*(volatile unsigned char *)(0x84000210))
#define GPSControl 		(*(volatile unsigned char *)(0x84000210))
#define GPSTxData 		(*(volatile unsigned char *)(0x84000212))
#define GPSRxData 		(*(volatile unsigned char *)(0x84000212))
#define GPSBaud    		(*(volatile unsigned char *)(0x84000214))

typedef struct gps_data_t {
	char time_utc[11];
	char latitude[10];
	char NS[2];
	char longitude[11];
	char EW[2];
	char inputHeader[sizeof(GPSHEADER)];
	int headerSize;
	int header_i;
	int csv_i;
	int char_i;
	bool gotHeader;
	bool gotGPSData;
	bool gotParsedTime;
	int hour;
	int minute;
	int second;
	char AM_PM[3];

} gps_data_t;

void initGPS(void);
void putcharGPS(char c);
char getcharGPS(void);
void putCommandGPS(char *command);
void readGPSDataToConsole(void);
void getGPSInfo(gps_data_t *gps_data);
void parseTime(gps_data_t *gps_data);

#endif /* GPS_H_ */
