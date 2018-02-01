#include "Button.h"
#include "Shapes.h"

static void* Button_ctor(void* _self, va_list* app)
{
	struct Button* self = super_ctor(Button, _self, app);

	width(self) = va_arg(*app, int);
	height(self) = va_arg(*app, int);

	attachChild(self, new(Rect, 0, 0, width(self), height(self)));
	attachChild(self, new(Circle, width(self)/2, height(self)/2, (width(self) < height(self) ? width(self) : height(self))/2));

	return self;
}

static void Button_draw(const void* _self, int x, int y)
{	printf("Button_draw\n");
	const struct Button* self = _self;
	super_draw(Button, self, x, y);
}

const void* Button;

void initButton()
{
	if(!Button)
	{
		initShapes();
		Button = new(ViewClass, "Button", View,
					 sizeof(struct Button), ctor, Button_ctor,
					 draw, Button_draw,0);
	}
}



