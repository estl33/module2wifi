#include <unistd.h>
#include "GraphicPrimitives.h"

void DE1_writePixel (int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = PutAPixel;		// give graphics a "write pixel" command
}

void DE1_writeHLine (int y, int x1, int x2, int Colour)
{
	if(x1 < 0) x1 = 0;
	if(x1 > 799) x1 = 799;

	if(x2 < 0) x2 = 0;
	if(x2 > 799) x2 = 799;

	if(y < 0) y = 0;
	if(y > 479) y = 479;

	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsX2Reg = x2;			// write coords to x2, y2
	GraphicsY2Reg = y;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawHLine;		// give graphics a "draw H line" command
}

void DE1_writeVLine (int x, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x;			// write coords to x2, y2
	GraphicsY2Reg = y2;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawVLine;		// give graphics a "draw H line" command
}

void DE1_writeLine (int x1, int y1, int x2, int y2, int color)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;			// write coords to x2, y2
	GraphicsY2Reg = y2;

	GraphicsColourReg = color;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawLine;		// give graphics a "draw H line" command
}

void DE1_writeRect (int x1, int y1, int x2, int y2, int color)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;			// write coords to x2, y2
	GraphicsY2Reg = y2;

	GraphicsColourReg = color;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawRect;		// give graphics a "draw H line" command
}
