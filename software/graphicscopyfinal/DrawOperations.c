#include <stdlib.h>
#include <assert.h>

#include "DrawOperations.h"
#include "DE1_SOC_graphics.h"

struct Bounds* internal_clip;

/**
 * Used to set partial rerendering
 */
void setClippingBounds(struct Bounds* clip) {
	internal_clip = clip;
}

void writeAPixel (int x, int y, int color)
{
		DE1_writePixel(x, y, color);
}

int readAPixel (int x, int y)
{
	(void) x;
	(void) y;
	return 0;
}

void programPalette(int PaletteNumber, int RGB)
{
	WAIT_FOR_GRAPHICS;
	GraphicsColourReg = PaletteNumber;
	GraphicsX1Reg = RGB >> 16;
	GraphicsY1Reg = RGB;
	GraphicsCommandReg = ProgramPaletteColour;
}

void writeHline (int y, int x1, int x2, int color)
{
	assert(x1 <= x2);

	struct Bounds line;
	line.x1 = x1;
	line.x2 = x2;
	line.y1 = y;
	line.y2 = y;

	struct Bounds* result = cohen_sutherland(&line, internal_clip);

	if(result) {
		DE1_writeHLine(result -> y1, result -> x1, result -> x2, color);
		free(result);
	}
}

void writeVline (int x, int y1, int y2, int color)
{
	assert(y1 <= y2);

	struct Bounds line;
	line.x1 = x;
	line.x2 = x;
	line.y1 = y1;
	line.y2 = y2;

	struct Bounds* result = cohen_sutherland(&line, internal_clip);

	if(result) {
		DE1_writeVLine(result -> x1, result -> y1, result -> y2, color);
		free(result);
	}
}


void writeLine(int x1, int y1, int x2, int y2, int color)
{
	assert(x1 <= x2);
	assert(y1 <= y2);

	struct Bounds line;
	line.x1 = x1;
	line.x2 = x2;
	line.y1 = y1;
	line.y2 = y2;

	struct Bounds* result = cohen_sutherland(&line, internal_clip);

	if(result) {
		DE1_writeLine(result -> x1, result -> y1, result -> x2, result -> y2, color);
		free(result);
	}
}

void writeRect(int x1, int y1, int x2, int y2, int borderWidth, int color)
{
	assert(x1 <= x2);
	assert(y1 <= y2);

	int i;
	for(i = 0; i < borderWidth; i++)
	{
		writeHline(y1 + i, x1, x2, color);
		writeHline(y2 - i, x1, x2, color);
		writeVline(x1 + i, y1, y2, color);
		writeVline(x2 - i, y1, y2, color);
	}
}

void writeFilledRect(int x1, int y1, int x2, int y2, int color)
{
	assert(x1 <= x2);
	assert(y1 <= y2);

	struct Bounds bounds;
	bounds.x1 = x1;
	bounds.x2 = x2;
	bounds.y1 = y1;
	bounds.y2 = y2;

	struct Bounds* result = intersect(&bounds, internal_clip);
	DE1_writeRect (result->x1, result->y1, result->x2, result->y2, color);
	free(result);
}

void clear() {
	writeFilledRect(0,0,770,480,BLACK);
}

void writeFilledRectWithBorder(int x1, int y1, int x2, int y2, int borderWidth, int color, int colorBorder)
{
	assert(x1 <= x2);
	assert(y1 <= y2);

	writeFilledRect(x1, y1, x2, y2, color);
	writeRect(x1, y1, x2, y2, borderWidth, colorBorder);
}

void writeCircle(int x1, int y1, int r, int color)
{
	assert(r > 0);

	int x = r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (r << 1);

	while(x >= y) {
		writeAPixel(x1 + x, y1 + y, color);
		writeAPixel(x1 + y, y1 + x, color);
		writeAPixel(x1 - y, y1 + x, color);
		writeAPixel(x1 - x, y1 + y, color);
		writeAPixel(x1 - x, y1 - y, color);
		writeAPixel(x1 - y, y1 - x, color);
		writeAPixel(x1 + y, y1 - x, color);
		writeAPixel(x1 + x, y1 - y, color);

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

void writeFilledCircle(int x1, int y1, int r, int color)
{
	assert(r > 0);

	int x = r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (r << 1);

	while(x >= y) {
		writeHline(y1 + y, x1 - x, x1 + x, color);
		writeHline(y1 + x, x1 - y, x1 + y, color);
		writeHline(y1 - y, x1 - x, x1 + x, color);
		writeHline(y1 - x, x1 - y, x1 + y, color);

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

void writeFilledBottomFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
	assert(y1 <= y2);
	assert(y1 <= y3);
	assert(y2 == y3);
	assert(x2 <= x3);

	double slope1 = ((double)(x2 - x1)) / (y2 - y1);
	double slope2 = ((double)(x3 - x1)) / (y3 - y1);

	float currX1 = x1;
	float currX2 = x1;

	int y;
	for(y = y1; y <= y2; y++) {
		writeHline(y, currX1, currX2, color);
		currX1 += slope1;
		currX2 += slope2;
	}
}

void writeFilledTopFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
	assert(y1 == y2);
	assert(y3 >= y2);
	assert(y3 >= y1);
	assert(x1 <= x2);

	double slope1 = ((double)(x3 - x1)) / (y3 - y1);
	double slope2 = ((double)(x3 - x2)) / (y3 - y2);

	float currX1 = x3;
	float currX2 = x3;

	int y;
	for(y = y3; y >= y2; y--) {
		writeHline(y, currX1, currX2, color);
		currX1 -= slope1;
		currX2 -= slope2;
	}
}
