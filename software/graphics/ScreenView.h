#ifndef SCREENVIEW_H_
#define SCREENVIEW_H_

#include "View.h"

extern const void* ScreenView;

struct ScreenView {
	const struct View _;
	int width;
	int height;
};

void initView();

#endif
