#include <stdlib.h>
#include <assert.h>
#include "Histogram.h"

static void* Histogram_ctor(void* _self, va_list* app)
{
	struct Histogram* self = super_ctor(Histogram, _self, app);

  self -> amplitude = 1;

	return self;
}

static void Histogram_draw(void* _self, struct Bounds* clip)
{
	struct Histogram* self = _self;
	super_draw(Histogram, _self, clip);

	struct Bounds* bounds = bounds(self);

  int width = (width(self)-10*4) / 5;
  int height = height(self) / 2;

  int i;
  for(i = 0; i < 5; i++)
  {
    writeFilledRect(bounds->x1+width*i+10*i,bounds->y1+height*(1-self->amplitude*((3.0-abs(i-2))/3)), bounds->x1+width*(i+1)+10*i, bounds->y2-height*(1-self->amplitude*((3.0-abs(i-2))/3)), WHITE);
  }
}

void setAmplitude(void* _self, double amplitude)
{
	assert(amplitude >= 0 && amplitude <= 1);

	struct Histogram* self = _self;

	if(self->amplitude != amplitude && visible(self) && renderer(self)) {
		invalidate(renderer(self), bounds(self));
	}

	self->amplitude = amplitude;
}

const void* Histogram;

void initHistogram()
{
	if(!Histogram)
		{
			initView();
			Histogram = new(ViewClass, "Histogram", View,
						 sizeof(struct Histogram), ctor, Histogram_ctor,
						 draw, Histogram_draw,0);
		}
}
