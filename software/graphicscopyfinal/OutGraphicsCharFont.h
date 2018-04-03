/*
 * OutGraphicsCharFont.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ???
 */

#ifndef OUTGRAPHICSCHARFONT_H_
#define OUTGRAPHICSCHARFONT_H_

#define XRES 800
#define YRES 480
#define XRES 800
#define YRES 480
#define FONT2_XPIXELS	10				// width of Font2 characters in pixels (no spacing)
#define FONT2_YPIXELS	14				// height of Font2 characters in pixels (no spacing)


extern const unsigned char Font5x7[][7] ;
extern const unsigned short int Font10x14[][14] ;

void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);
void OutGraphicsCharFont2(int x, int y, int colour, int backgroundcolour, int c, int Erase);

#endif /* OUTGRAPHICSCHARFONT_H_ */
