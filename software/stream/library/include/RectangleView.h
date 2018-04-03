#ifndef RECTANGLE_VIEW_
#define RECTANGLE_VIEW_

#include "Application.h"
#include "Canvas.h"

class RectangleView : public View {
  int colour;
public:
  void onDraw(Canvas canvas);
  void setColour(int colour);
};

#endif
