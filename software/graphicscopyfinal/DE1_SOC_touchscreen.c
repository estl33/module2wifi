#include <stdlib.h>
#include <unistd.h>

#include "DE1_SOC_touchscreen.h"

struct Point mouseLocation;

#ifdef SIM_CODE

void DE1_touchInit()
{

}

int DE1_isScreenTouched()
{
  return mouse_LeftClick;
}

struct Point DE1_getLastTouchLocation()
{
  if(mouse_LeftClick) {
    mouseLocation.x = mouse_x;
    mouseLocation.y = mouse_y;
  }

  return mouseLocation;
}

#else

int putcharTouch(int c)
{
 // poll Tx bit(bit 0) in 6850 status register. Wait for it to become '1'
 while((TOUCH_STATUS >> 1) % 2 == 0);
 TOUCH_TXDATA = c;
 return c;
}

int getcharTouch()
{
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 while(TOUCH_STATUS % 2 == 0);
 return TOUCH_RXDATA;
}

void DE1_touchInit()
{
  printf("Init touch\n");
  TOUCH_CONTROL = 0x15;
  TOUCH_BAUD = 0x05; //9600
  putcharTouch(0x12);
}

int DE1_isScreenTouched()
{
  return 0x01 & TOUCH_STATUS;
}

struct Point DE1_getLastTouchLocation()
{
	if(DE1_isScreenTouched()) {
		char byte1 = getcharTouch();
		if((byte1 & 0xff) == 0x80) {
			mouseLocation.status = RELEASED;
		} else if((byte1 & 0xff) == 0x81) {
			mouseLocation.status = PRESSED;
		} else {
			mouseLocation.status = NONE;
			return mouseLocation;
		}

		char byte2 = getcharTouch();
		char byte3 = getcharTouch();
		char byte4 = getcharTouch();
		char byte5 = getcharTouch();

		int x = ((double) (byte2 >> 1) + (byte3 << 7)) / 4096 * 770 + 30 + 0.5;
		int y = ((double) (byte4 >> 1) + (byte5 << 7)) / 4096 * 480 + 0.5;

		if(x >= 0 && x <= 800 && y >= 0 && y <= 480) {
			mouseLocation.x = x;
			mouseLocation.y = y;
		}

		return mouseLocation;

	} else {
		return mouseLocation;
	}
}

void DE1_clearLastTouchLocation()
{
	mouseLocation.x = 0;
	mouseLocation.y = 0;
	mouseLocation.status = NONE;
}

#endif
