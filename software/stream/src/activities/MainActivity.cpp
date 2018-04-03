#include "MainActivity.h"

#include <unistd.h>

#include "Log.h"
#include "RectangleView.h"

void MainActivity::onCreate() {
  Log_Low("MainActivity", "OnCreate()");

  RectangleView* view = new RectangleView();
  Activity::attachView(view);

  view->setX(300);
  view->setY(100);
  view->setWidth(100);
  view->setHeight(100);
  view->setColour(2);

//  Bound bound = Bound(0,0,800,480);

//  view->measure();
//  view->invalidate();
//  view->draw(bound);

//  int i = 10;
//
//  while(1) {
//    usleep(1000000);
//    Log_Low("MainActivity", "Move");
//    view->setX(300+i);
//    i += 10;
//    view->draw(bound);
//  }
}

string MainActivity::getName() {
  return "MainActivity";
}
