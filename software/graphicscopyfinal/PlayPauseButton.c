#include <stdlib.h>
#include "PlayPauseButton.h"
#include "TouchEventHandler.h"

int onPlayPauseButtonDown(void* _self, int x, int y)
{
  (void) x;
  (void) y;
  struct PlayPauseButton* self = _self;
  if(!self->focused) {
	  self -> focused = 1;
	  invalidate(renderer(self), bounds(self));
  }
  return 1;
}

int onPlayPauseButtonUp(void* _self, int x, int y)
{
  (void) x;
  (void) y;
  struct PlayPauseButton* self = _self;
  if(self->focused) {
	  self -> focused = 0;
	  invalidate(renderer(self), bounds(self));
  }
  return 1;
}

int onPlayPauseButtonClick(void* _self, int x, int y)
{
  (void) x;
  (void) y;
  struct PlayPauseButton* self = _self;
  self->play = !self->play;
  invalidate(renderer(self), bounds(self));
  return 0;
}

static void* PlayPauseButton_ctor(void* _self, va_list* app)
{
	struct PlayPauseButton* self = super_ctor(PlayPauseButton, _self, app);

	self -> colour = va_arg(*app, int);
	self -> focusColour = va_arg(*app, int);

	self -> focused = 0;
	self -> play = 1;

	addEventListener(self, DOWN_EVENT, onPlayPauseButtonDown);
	addEventListener(self, UP_EVENT, onPlayPauseButtonUp);
	addEventListener(self, CLICK_EVENT, onPlayPauseButtonClick);

	return self;
}

static void PlayPauseButton_draw(void* _self, struct Bounds* clip)
{
	struct PlayPauseButton* self = _self;
	super_draw(PlayPauseButton, _self, clip);

	struct Bounds* bounds = bounds(self);

	if(self->play) {
		writeFilledBottomFlatTriangle(bounds->x1, bounds->y1, bounds->x1, (bounds->y1+bounds->y2)/2, bounds->x2, (bounds->y1+bounds->y2)/2, self -> focused ? self -> focusColour : self -> colour);
		writeFilledTopFlatTriangle(bounds->x1, (bounds->y1+bounds->y2)/2, bounds->x2, (bounds->y1+bounds->y2)/2, bounds->x1, bounds->y2, self -> focused ? self -> focusColour : self -> colour);
	} else {
		int width = width(self)/3;
		writeFilledRect(bounds->x1, bounds->y1, bounds->x1 + width, bounds->y2, self -> focused ? self -> focusColour : self -> colour);
		writeFilledRect(bounds->x2-width, bounds->y1, bounds->x2, bounds->y2, self -> focused ? self -> focusColour : self -> colour);
	}
}

const void* PlayPauseButton;

void initPlayPauseButton()
{
	if(!PlayPauseButton)
		{
			initView();
			PlayPauseButton = new(ViewClass, "PlayPauseButton", View,
						 sizeof(struct PlayPauseButton), ctor, PlayPauseButton_ctor,
						 draw, PlayPauseButton_draw,0);
		}
}



