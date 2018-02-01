#include "ScreenView.h"

static void* ScreenView_ctor(void* _self, va_list* app)
{
	struct ScreenView* self = super_ctor(ScreenView, _self, app);

	self->width = va_arg(*app, int);
	self->height = va_arg(*app, int);

	return self;
}

static void ScreenView_draw(const void* _self, int x, int y)
{
	const struct ScreenView* self = _self;

	draw(super(self), x, y);
}

const void* ScreenView;

void initScreenView()
{
	if(!ScreenView)
	{
		initView();
		ScreenView = new(ViewClass, "ScreenView", View,
					 sizeof(struct ScreenView), ctor, ScreenView_ctor,
					 draw, ScreenView_draw,0);
	}
}
