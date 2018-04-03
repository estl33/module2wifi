#include "Application.h"

#include "Assert.h"

View::View() {
	M_Stack();
  bounds = 0;
  damaged = 0;
  parent = 0;
  M_Unstack();
}

void View::setX(int x) {
	M_Stack();
  View::x = x;
  invalidate();
  M_Unstack();
}

void View::setY(int y) {
	M_Stack();
  View::y = y;
  invalidate();
  M_Unstack();
}

void View::setWidth(int width) {
	M_Stack();
  View::width = width;
  invalidate();
  M_Unstack();
}

void View::setHeight(int height) {
	M_Stack();
  View::height = height;
  invalidate();
  M_Unstack();
}

void View::onAttach() {
}

void View::onDetach() {
}

void View::onDraw(Canvas canvas) {
	M_Stack();
  Log_Low("View", "onDraw");
  M_Unstack();
}

void View::onMeasure() {
}

int View::onDownPress() {
	return 1;
}

int View::onMovePress() {
	return 1;
}

int View::onUpPress() {
	return 1;
}

int View::onClick() {
	return 1;
}

void View::draw(Bound parent) {
	M_Stack();
  if(damaged != 0) {
	Log_Low("View", "draw");
    Canvas canvas = Canvas(parent.x1, parent.y1);
    canvas.setClippingBound(*damaged);

    this->onDraw(canvas);

    damaged = 0;

    Bound currBound = Bound(parent.x1 + bounds->x1, parent.y1 + bounds->y1, parent.x2 + bounds->x2, parent.y2 + bounds->y2);

    Iterator* iterator = childrenViews.getIteratorStart();
    View* child = (View*) iterator->next();
    while(child != 0) {
      child->draw(currBound);
      child = (View*) iterator->next();
    }
    delete iterator;
  }
  M_Unstack();
}

void View::measure() {
  M_Stack();
  if(bounds != 0) {
    delete bounds;
  }

  bounds = new Bound(x, y, x + width, y + height);

  Iterator* iterator = childrenViews.getIteratorStart();
  View* child = (View*) iterator->next();
  while(child != 0) {
    child->measure();
    child = (View*) iterator->next();
  }
  delete iterator;
  M_Unstack();
}

void View::attachView(View* view) {
	M_Stack();
	M_Assert(view->parent == 0, "Trying to attach a view that is already attached!!");

	childrenViews.insertEnd(view);
	view->parent = this;
	view->onAttach();
	M_Unstack();
}

void View::detachView(View* view) {
}

void View::invalidate() {
	M_Stack();
	if(bounds != 0) {
		if(damaged != 0) delete damaged;
		Bound oldBound = Bound(bounds->x1, bounds->y1, bounds->x2, bounds->y2);
		View::measure();
		Bound newBound = Bound(bounds->x1, bounds->y1, bounds->x2, bounds->y2);
		damaged = newBound.merge(oldBound);
		if(parent != 0) parent->invalidateInParent(*damaged);
	}
	M_Unstack();
}

void View::invalidateInParent(Bound childBound) {
	M_Stack();
	if(bounds == 0 || width == 0 || height == 0) {
		M_Unstack();
		return;
	}
	if(damaged == 0) {
		Bound temp = Bound(x + childBound.x1, y + childBound.y1, x + childBound.x2, y + childBound.y2);
		damaged = temp.intersect(*bounds);
	} else {
		Bound temp = Bound(x + childBound.x1, y + childBound.y1, x + childBound.x2, y + childBound.y2);
		Bound* mergedTemp = temp.merge(*damaged);
		delete damaged;
		damaged = mergedTemp;
	}

	if(parent != 0) parent->invalidateInParent(*damaged);

	M_Unstack();
}
