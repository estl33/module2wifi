#ifndef TEXTVIEW_H_
#define TEXTVIEW_H_

#include "View.h"

extern const void* TextView;

struct TextView {
	const struct View _;

	char* text;
	int color;
};

void setText(void* _self, char* text);

void initTextView();


#endif /* TEXTVIEW_H_ */
