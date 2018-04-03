#include "TouchEventHandler.h"

#include <stdlib.h>
#include <unistd.h>

#include "View.h"
#include "DE1_SOC_touchscreen.h"
#include "Bounds.h"

void removeRestOfPath(struct TouchEventPath* next)
{
  while(next) {
    struct View* view = next->view;
    struct EventListener* nextListener = view->listenerHead;
    while(nextListener)
    {
      if(UP_EVENT == nextListener->type) {
        nextListener->callback(view);
      }
      nextListener = nextListener->next;
    }

    struct TouchEventPath* temp = next;
    next = next->next;
    free(temp);
  }
}

void handleDown(struct TouchEventHandler* handler, struct Point point)
{
  generateTouchList(handler, point.x, point.y);
  propagateTouchEvent(handler, DOWN_EVENT);
  handler->pastPressed = 1;
  handler->x = point.x;
  handler->y = point.y;
}

void handleMove(struct TouchEventHandler* handler, struct Point point)
{
  handler->dx = point.x - handler->x;
  handler->dy = point.y - handler->y;
  if(handler->moving) {
    handler->x = point.x;
    handler->y = point.y;
    propagateTouchEvent(handler, MOVE_EVENT);
  } else {
    if(abs(handler->dx) > 12 || abs(handler->dy) > 12) {
      handler->dx = 0;
      handler->dy = 0;
      handler->moving = 1;
      handler->x = point.x;
      handler->y = point.y;
      propagateTouchEvent(handler, MOVE_EVENT);
    }
  }
}

void handleUp(struct TouchEventHandler* handler, struct Point point)
{
  struct TouchEventPath* next = handler->head;
  while(next) {
    struct View* view = next->view;
    int withinBounds = containsPoint(handler->x, handler->y, view->absBounds);
    struct EventListener* nextListener = view->listenerHead;
    while(nextListener)
    {
      if(UP_EVENT == nextListener->type) {
        nextListener->callback(view, handler->x, handler->y);
      } else if(withinBounds && CLICK_EVENT == nextListener->type) {
        nextListener->callback(view);
      }
      nextListener = nextListener->next;
    }

    struct TouchEventPath* temp = next;
    next = next->next;
    free(temp);
  }

  handler->head = handler->tail = 0;
  handler->pastPressed = 0;
  handler->moving = 0;
}
int count = 0;
void handleTouch(struct TouchEventHandler* handler)
{
	if(DE1_isScreenTouched()) {
		struct Point point = DE1_getLastTouchLocation();
		if(point.status == PRESSED) {
		  if(!handler->pastPressed) {
			handleDown(handler, point);
		  } else {
			handleMove(handler, point);
		  }
		} else if(point.status == RELEASED) {
		  if(handler->pastPressed) {
			  handleUp(handler, point);
			  DE1_clearLastTouchLocation();
		  }
		}
	} else {
		count++;
		if(count == 9) {
			struct Point point = DE1_getLastTouchLocation();
			if(handler->pastPressed) {
				handleUp(handler, point);
				DE1_clearLastTouchLocation();
			}
			count = 0;
		}
	}
}

void propagateTouchEvent(struct TouchEventHandler* handler, int type)
{
  struct TouchEventPath* next = handler->head;
  while(next) {
    struct View* view = next->view;
    struct EventListener* nextListener = view->listenerHead;
    while(nextListener)
    {
      if(type == nextListener->type) {
        int result = 0;
        switch (type) {
          case DOWN_EVENT:;
          result = nextListener->callback(view, handler->x - view->absBounds->x1, handler->y - view->absBounds->y1);
          break;
          case UP_EVENT:;
          result = nextListener->callback(view, handler->x - view->absBounds->x1, handler->y - view->absBounds->y1);
          break;
          case MOVE_EVENT:;
          result = nextListener->callback(view, handler->x - view->absBounds->x1, handler->y - view->absBounds->y1, handler->dx, handler->dy);
          break;
          case CLICK_EVENT:;
          result = nextListener->callback(view);
          break;
        }
        if(result){
          removeRestOfPath(next->next);
          next->next = 0;
        }
      }
      nextListener = nextListener->next;
    }

    next = next->next;
  }
}

void generateTouchListChildren(struct TouchEventHandler* handler, void* _view, int x, int y)
{
  struct View* view = _view;
  if(view->visible && containsPoint(x, y, view->absBounds)) {
    struct TouchEventPath* path = malloc(sizeof(struct TouchEventPath));
    path->view = _view;
    path->next = 0;
    if(handler->tail) {
      handler->tail->next = path;
      handler->tail = path;
    } else {
      handler->head = handler->tail = path;
    }

    struct ChildView* prevView = view->viewTail;
    while(prevView)
    {
      struct View* childView = prevView->view;
      generateTouchListChildren(handler, childView, x, y);
      prevView = prevView->prev;
    }
  }
}

void generateTouchList(struct TouchEventHandler* handler, int x, int y)
{
  generateTouchListChildren(handler, handler->rootView, x, y);
}
