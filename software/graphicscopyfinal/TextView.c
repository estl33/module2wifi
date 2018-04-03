#include <stdlib.h>
#include <assert.h>
#include "TextView.h"
#include "OutGraphicsCharFont.h"

static void* TextView_ctor(void* _self, va_list* app)
{
	struct TextView* self = super_ctor(TextView, _self, app);

	self->text = 0;
	self -> color = va_arg(*app, int);

	return self;
}

static void TextView_draw(void* _self, struct Bounds* clip)
{
	struct TextView* self = _self;
	super_draw(TextView, _self, clip);

	struct Bounds* bounds = bounds(self);

	if(self->text) {
		int i;
		for(i = 0; self->text[i] != 0; i++) {
			OutGraphicsCharFont2(bounds->x1+i*12, bounds->y1, self->color, BLACK, self->text[i], 0);
		}
	}
}

void setText(void* _self, char* text)
{
	struct TextView* self = _self;
	self->text = text;
	invalidate(renderer(self), bounds(self));
}

const void* TextView;

void initTextView()
{
	if(!TextView)
		{
			initView();
			TextView = new(ViewClass, "TextView", View,
						 sizeof(struct TextView), ctor, TextView_ctor,
						 draw, TextView_draw,0);
		}
}
