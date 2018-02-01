#ifndef SHAPES_H_
#define SHAPES_H_

#include "View.h"

extern const void* Circle;

struct Circle {
	const struct View _;
	int rad;
};

extern const void* Rect;

struct Rect {
	const struct View _;
};

void initShapes();

#endif
