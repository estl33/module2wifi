#ifndef BUTTON_H_
#define BUTTON_H_

#include "View.h"

extern const void* Button;

struct Button {
	const struct View _;

	int colour;
  int focusColour;

  char c;

  int focused;
};

void setChar(void* self, char c);

void initButton();

#endif
