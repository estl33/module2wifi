#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

//GRAPHICS
#include "Renderer.h"
#include "ScreenView.h"
#include "ListView.h"
#include "Button.h"
#include "ProgressBar.h"
#include "Histogram.h"
#include "TextView.h"
#include "PlayPauseButton.h"
#include "Bounds.h"

#include "OutGraphicsCharFont.h"

//TOUCH
#include "TouchEventHandler.h"
#include "DE1_SOC_touchscreen.h"

//IO
#include "Wifi.h"
#include "TouchInterrupt.h"
#include "Equalizer.h"
#include "Gps.h"

//TEST UNITS
#include "TestLib.h"

#define Sound_In (*(volatile signed short*) (0x0000020))

struct Renderer renderer;
struct TouchEventHandler touchHandler;
void* rootView;

//Audio bar
void* audioScreen;
void* playButton;
void* settingsButton;
void* channel1Button;
void* channel2Button;
void* timeText;

//Visualization views
void* histogramView;

//Settings view
void* menuScreen;
void* eqButton;
void* colorButton;

void* colorScreen;
void* blueButton;
void* magentaButton;
void* redButton;

void* eqScreen;
void* trebleText;
void* bassText;
void* trebleControl;
void* bassControl;

/**
 * Initialize board components and graphics
 */
void init(int argc, char* argv[])
{
#ifndef NDEBUG
	testAll();
#endif

	//Init views
	initScreenView();
	initListView();
	initButton();
	initProgressBar();
	initHistogram();
	initTextView();
	initPlayPauseButton();

	//Set default color
	programPalette(PRIMARY, 0x000000FF);

	//Init io
	DE1_touchInit();
	Init_Wifi();
	wifi_commands_init();
	usleep(10000);
	Init_Volume();
	Init_Interrupt();
	initGPS();

	//Init renderer
	renderer.displayWidth = 800;
	renderer.displayHeight = 480;
	renderer.damagedRegion = 0;

	//Init touch renderer
	touchHandler.head = 0;
	touchHandler.tail = 0;

	//Init root view
	rootView = new(ScreenView, 800, 480, PRIMARY);
	touchHandler.rootView = rootView;
	setRoot(&renderer, rootView);
	setRenderer(rootView, &renderer);
}

/**
 * Initialize all project views
 */
void displayScreen(){
		//Audio Bar Init
		audioScreen = new(ScreenView, 770, 80, WHITE);
		setX(audioScreen, 0);
		setY(audioScreen, 400);

		timeText = new(TextView, PRIMARY);
		setX(timeText, 410);
		setY(timeText, 33);
		setWidth(timeText, 100);
		setHeight(timeText, 30);
		setText(timeText, "Loading.");

		playButton = new(PlayPauseButton, PRIMARY, BLACK);
		setX(playButton, 10);
		setY(playButton, 10);
		setWidth(playButton, 60);
		setHeight(playButton, 60);

		settingsButton = new(Button, PRIMARY, BLACK);
		setX(settingsButton, 700);
		setY(settingsButton, 10);
		setWidth(settingsButton, 60);
		setHeight(settingsButton, 60);
		setChar(settingsButton, 'M');

		channel1Button = new(Button, PRIMARY, BLACK);
		setX(channel1Button, 90);
		setY(channel1Button, 10);
		setWidth(channel1Button, 60);
		setHeight(channel1Button, 60);
		setChar(channel1Button, '1');

		channel2Button = new(Button, PRIMARY, BLACK);
		setX(channel2Button, 160);
		setY(channel2Button, 10);
		setWidth(channel2Button, 60);
		setHeight(channel2Button, 60);
		setChar(channel2Button, '2');

		attachView(rootView, audioScreen);
		attachView(audioScreen, timeText);
		attachView(audioScreen, settingsButton);
		attachView(audioScreen, playButton);
		attachView(audioScreen, channel1Button);
		attachView(audioScreen, channel2Button);

		//Histogram init
		histogramView = new(Histogram);
		setX(histogramView, 200);
		setY(histogramView, 100);
		setWidth(histogramView, 370);
		setHeight(histogramView, 200);

		attachView(rootView, histogramView);

		//Settings Screen Init
		menuScreen = new(ScreenView, 170, 300, WHITE);
		setX(menuScreen, 100);
		setY(menuScreen, 50);
		setVisible(menuScreen, 0);

		eqButton = new(Button, PRIMARY, BLACK);
		setX(eqButton, 50);
		setY(eqButton, 75);
		setWidth(eqButton, 70);
		setHeight(eqButton, 70);
		setChar(eqButton, 'E');

		colorButton = new(Button, PRIMARY, BLACK);
		setX(colorButton, 50);
		setY(colorButton, 155);
		setWidth(colorButton, 70);
		setHeight(colorButton, 70);
		setChar(colorButton, 'C');

		colorScreen = new(ScreenView, 370, 300, WHITE);
		setX(colorScreen, 300);
		setY(colorScreen, 50);
		setVisible(colorScreen, 0);

		blueButton = new(Button, BLUE, BLACK);
		setX(blueButton, 80);
		setY(blueButton, 120);
		setWidth(blueButton, 60);
		setHeight(blueButton, 60);

		magentaButton = new(Button, MAGENTA, BLACK);
		setX(magentaButton, 150);
		setY(magentaButton, 120);
		setWidth(magentaButton, 60);
		setHeight(magentaButton, 60);

		redButton = new(Button, RED, BLACK);
		setX(redButton, 220);
		setY(redButton, 120);
		setWidth(redButton, 60);
		setHeight(redButton, 60);

		eqScreen = new(ScreenView, 370, 300, WHITE);
		setX(eqScreen, 300);
		setY(eqScreen, 50);
		setVisible(eqScreen, 0);

		trebleText = new(TextView, PRIMARY);
		setX(trebleText, 50);
		setY(trebleText, 59);
		setWidth(trebleText, 100);
		setHeight(trebleText, 14);
		setText(trebleText, "Treble");

		trebleControl = new(ProgressBar, PRIMARY);
		setX(trebleControl, 50);
		setY(trebleControl, 75);
		setWidth(trebleControl, 270);
		setHeight(trebleControl, 50);

		bassText = new(TextView, PRIMARY);
		setX(bassText, 50);
		setY(bassText, 175);
		setWidth(bassText, 100);
		setHeight(bassText, 14);
		setText(bassText, "Bass");

		bassControl = new(ProgressBar, PRIMARY);
		setX(bassControl, 50);
		setY(bassControl, 191);
		setWidth(bassControl, 270);
		setHeight(bassControl, 50);

		attachView(rootView, menuScreen);
		attachView(menuScreen, eqButton);
		attachView(menuScreen, colorButton);
		attachView(rootView, colorScreen);
		attachView(colorScreen, blueButton);
		attachView(colorScreen, magentaButton);
		attachView(colorScreen, redButton);
		attachView(rootView, eqScreen);
		attachView(eqScreen, trebleText);
		attachView(eqScreen, bassText);
		attachView(eqScreen, trebleControl);
		attachView(eqScreen, bassControl);

		inline int onPlayClick(void* _self) {
			wifi_play_pause_command();
			waitWifi();
			printf("PLAY PAUSE \n");

			return 1;
		}

		inline int onCh1Click(void* _self) {
			wifi_ch1_command();
			waitWifi();
			printf("RADIO CH1 \n");

			return 1;
		}

		inline int onCh2Click(void* _self) {
			wifi_ch2_command();
			waitWifi();
			printf("RADIO CH2\n");

			return 1;
		}

		inline int onSettingsClick(void* _self) {
			struct View* self1 = menuScreen;
			struct View* self2 = eqScreen;
			struct View* self3 = colorScreen;
			struct View* self4 = histogramView;
			if(self4->visible) {
				setVisible(self1, 1);
				setVisible(self2, 1);
				setVisible(self3, 0);
				setVisible(self4, 0);
				setChar(settingsButton, 'X');
			} else {
				setVisible(self1, 0);
				setVisible(self2, 0);
				setVisible(self3, 0);
				setVisible(self4, 1);
				setChar(settingsButton, 'M');
			}
			return 1;
		}

		inline int onEQClick(void* _self) {
			struct View* self1 = eqScreen;
			struct View* self2 = colorScreen;
			setVisible(self1, 1);
			setVisible(self2, 0);
			return 1;
		}

		inline int onColorClick(void* _self) {
			struct View* self1 = eqScreen;
			struct View* self2 = colorScreen;
			setVisible(self1, 0);
			setVisible(self2, 1);
			return 1;
		}

		inline int onBlueClick(void* _self) {
			programPalette(PRIMARY, 0x000000FF);
			return 1;
		}

		inline int onMagentaClick(void* _self) {
			programPalette(PRIMARY, 0x00FF00FF);
			return 1;
		}

		inline int onRedClick(void* _self) {
			programPalette(PRIMARY, 0x00FF0000);
			return 1;
		}



		addEventListener(playButton, CLICK_EVENT, onPlayClick);
		addEventListener(channel1Button, CLICK_EVENT, onCh1Click);
		addEventListener(channel2Button, CLICK_EVENT, onCh2Click);
		addEventListener(settingsButton, CLICK_EVENT, onSettingsClick);
		addEventListener(eqButton, CLICK_EVENT, onEQClick);
		addEventListener(colorButton, CLICK_EVENT, onColorClick);
		addEventListener(blueButton, CLICK_EVENT, onBlueClick);
		addEventListener(magentaButton, CLICK_EVENT, onMagentaClick);
		addEventListener(redButton, CLICK_EVENT, onRedClick);

		render(&renderer);
}

int main(int argc, char* argv[])
{
	int magnitude;
	init(argc, argv);
	printf("INIT All DONE\n");

	displayScreen();
	printf("DISPLAY DONE\n");

	int hexHour;
	int hexMin;
	int hexSec;
	int timerCount = 0;

	gps_data_t *gps_data = malloc(sizeof(gps_data));
	gps_data->headerSize = sizeof(GPSHEADER)-1;
	gps_data->header_i = 0;
	gps_data->gotGPSData = false;
	gps_data->inputHeader[gps_data->headerSize] = '\0';
	gps_data->gotHeader = false;
	gps_data->gotParsedTime = false;

	while(!(gps_data->gotGPSData && !gps_data->gotParsedTime)){
		getGPSInfo(gps_data);
	}

	parseTime(gps_data);

	hexHour = gps_data->hour;
	hexMin =  gps_data->minute;
	hexSec = gps_data->second;

	while(1) {
		if(timerCount % 5 == 0) {
			magnitude = Sound_In;
			setAmplitude(histogramView, abs(magnitude)/40000.0);

			IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE, 0);

			struct ProgressBar* trebleView = trebleControl;
			update_treble(trebleView->percentage*5+0.5);

			struct ProgressBar* bassView = bassControl;
			update_bass(bassView->percentage*5+0.5);
		}
		if(timerCount >= 60){
			hexSec ++;
			if(hexSec == 60){
				hexSec = 0;
				hexMin ++;
			}
			if(hexMin == 60){
				hexMin = 0;
				hexHour ++;
			}
			if(hexHour == 24){
				hexHour = 1;
			}

			char time[50];
			sprintf(time, "%02d:%02d:%02d", hexHour, hexMin, hexSec);
			setText(timeText, time);
			timerCount = 0;
		} else {
			timerCount ++;
		}


		handleTouch(&touchHandler);
		partialRender(&renderer);
		usleep(1000000/60);
	}

	return 0;
}

