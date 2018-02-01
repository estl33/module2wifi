#include "Shapes.h"

static void* Circle_ctor(void* _self, va_list* app)
{
	struct Circle* self = super_ctor(Circle, _self, app);

	self->rad = va_arg(*app, int);

	width(self) = self->rad*2;
	height(self) = self->rad*2;

	return self;
}

static void Circle_draw(const void* _self, int x, int y)
{	printf("Circle_draw\n");
	const struct Circle* self = _self;

	WriteFilledCircle(x + x(self), y + y(self), self->rad, WHITE);
}

static void* Rect_ctor(void* _self, va_list* app)
{	printf("Rect_draw\n");
	struct Rect* self = super_ctor(Rect, _self, app);

	width(self) = va_arg(*app, int);
	height(self) = va_arg(*app, int);

	return self;
}

static void Rect_draw(const void* _self, int x, int y)
{
	const struct Rect* self = _self;

	WriteFilledRect(x+ x(self), x + x(self) + width(self), y + y(self), y + y(self) + height(self), RED);
}

const void* Circle;
const void* Rect;

void initShapes()
{
	if(!Circle)
	{
		initView();
		Circle = new(ViewClass, "Circle", View,
					 sizeof(struct Circle), ctor, Circle_ctor,
					 draw, Circle_draw,0);
	}

	if(!Rect)
		{
			initView();
			Rect = new(ViewClass, "Rect", View,
						 sizeof(struct Rect), ctor, Rect_ctor,
						 draw, Rect_draw,0);
		}
}


