#include "RectangleView.h"

#include "Log.h"
#include "Assert.h"

void RectangleView::setColour(int colour) {
	M_Stack();
  RectangleView::colour = colour;
  M_Unstack();
}

void RectangleView::onDraw(Canvas canvas) {
  M_Stack();
  Log_Low("RectangleView", "onDraw");
  canvas.setFill(colour);
  canvas.drawRect(View::bounds->x1, View::bounds->y1, View::bounds->x2, View::bounds->y2);
  M_Unstack();
}
