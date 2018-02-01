#include "DrawOperations.h"

void WriteAPixel (int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = PutAPixel;		// give graphics a "write pixel" command
}

int ReadAPixel (int x, int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;		// return the palette number (colour)
}


void ProgramPalette(int PaletteNumber, int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16   ;          // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB ;                	 // program green and blue into 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour;	// issue command
}

void WriteHline (int y, int x1, int x2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsX2Reg = x2;			// write coords to x2, y2
	GraphicsY2Reg = y;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawHLine;		// give graphics a "draw H line" command
}

void WriteVline (int x, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x;			// write coords to x2, y2
	GraphicsY2Reg = y2;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawVLine;		// give graphics a "draw H line" command
}

void WriteLine (int x1, int x2, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;			// write coords to x2, y2
	GraphicsY2Reg = y2;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawLine;		// give graphics a "draw H line" command
}

void WriteRect(int x1, int x2, int y1, int y2, int borderWidth, int Colour) {
	int i;
	for(i = 0; i < borderWidth; i++)
	{
		WriteHline(y1 + i, x1, x2, Colour);
		WriteHline(y2 - i, x1, x2, Colour);
		WriteVline(x1 + i, y1, y2, Colour);
		WriteVline(x2 - i, y1, y2, Colour);
	}
}

void WriteFilledRect(int x1, int x2, int y1, int y2, int Colour)
{
	int i;
	for(i = y1; i < y2; i++)
	{
		WriteHline(i, x1, x2, Colour);
	}
}

void clear() {
	WriteFilledRect(0, XRES, 0, YRES, BLACK);
}

void WriteFilledRectWithBorder(int x1, int x2, int y1, int y2, int borderWidth, int Colour, int ColourBorder)
{
	WriteFilledRect(x1, x2, y1, y2, Colour);
	WriteRect(x1, x2, y1, y2, borderWidth, ColourBorder);
}

void WriteCircle(int x1, int y1, int r, int Colour)
{
	int x = r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (r << 1);

	while(x >= y) {
		WriteAPixel(x1 + x, y1 + y, Colour);
		WriteAPixel(x1 + y, y1 + x, Colour);
		WriteAPixel(x1 - y, y1 + x, Colour);
		WriteAPixel(x1 - x, y1 + y, Colour);
		WriteAPixel(x1 - x, y1 - y, Colour);
		WriteAPixel(x1 - y, y1 - x, Colour);
		WriteAPixel(x1 + y, y1 - x, Colour);
		WriteAPixel(x1 + x, y1 - y, Colour);

		if(err <= 0) {
			y++;
			err += dy;
			dy += 2;
		} else {
			x--;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

void WriteFilledCircle(int x1, int y1, int r, int Colour)
{
	int x = r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (r << 1);

	while(x >= y) {
		WriteHline(y1 + y, x1 - x, x1 + x, Colour);
		WriteHline(y1 + x, x1 - y, x1 + y, Colour);
		WriteHline(y1 - y, x1 - x, x1 + x, Colour);
		WriteHline(y1 - x, x1 - y, x1 + y, Colour);

		if(err <= 0) {
			y++;
			err += dy;
			dy += 2;
		} else {
			x--;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}



