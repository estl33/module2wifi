#include <stdlib.h>
#include "View.h"

static void* View_ctor(void* _self, va_list* app)
{
	struct View* self = super_ctor(View, _self, app);

	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);

	self->head = self->tail = 0;

	return self;
}

static void View_draw(const void* _self, int x, int y)
{	printf("View_draw\n");
	const struct View* self = _self;

	struct Children* next = self->head;
	while(next) {
		if(self->invalid) draw(next->child, x + self->x, y + self->y);
		next = next->next;
	}
}

void draw(const void* _self, int x, int y)
{
	const struct ViewClass* class = classOf(_self);

	class->draw(_self, x, y);
}

void super_draw(const void* _class, const void* _self, int x, int y)
{
	const struct ViewClass *superClass = super(_class);

	superClass->draw(_self, x, y);
}

static void View_onPress(const void* _self, int x, int y)
{
	const struct View* self = _self;

	//Do nothing
}

void onPress(const void* _self, int x, int y)
{
	const struct ViewClass* class = classOf(_self);

	class->onPress(_self, x, y);
}

void super_onPress(const void* _class, const void* self, int x, int y) {
	const struct ViewClass *superClass = super(_class);

	superClass->onPress(superClass, x, y);
}

void move(void* _self, int dx, int dy)
{
	struct View* self = _self;

	self -> x += dx;
	self -> y += dy;

	invalidate(self);
}

void setVisibility(void* _self, int visibility)
{
	struct View* self = _self;

	self -> visibility = visibility;
}

void attachChild(void* _self, void* child)
{
	printf("attachChild\n");
	struct View* self = _self;

	parent(child) = _self;

	struct Children* newChild = malloc(sizeof(struct Children));
	newChild->child = child;

	if(self->tail) {
		self->tail->next = newChild;
	} else {
		self->head = self->tail = newChild;
	}
}

void detachChild(void* _self, void* child) {
	//Do nothing
}

void invalidate(void* _self)
{	printf("invalidate\n");
	struct View* self = _self;
	self->invalid = 1;

	if(self->parent) {
		invalidateChildrenInParent(self->parent);
	}
}

void invalidateChildrenInParent(void* _self)
{
	printf("invalidateChildrenInParent\n");
	struct View* self = _self;

	self->invalid = 1;

	if(self->parent && !self->opaque) {
		invalidateChildrenInParent(self->parent);
	}

	struct Children* next = self->head;
	while(next) {
		invalidateChildren(next->child);
		next = next->next;
	}
}

void invalidateChildren(void* _self)
{
	printf("invalidateChildren\n");
	struct View* self = _self;
	self->invalid = 1;

	struct Children* next = self->head;
	while(next) {
		invalidateChildren(next->child);
	}
}

static void* ViewClass_ctor(void* _self, va_list* app) {
	struct ViewClass* self = super_ctor(ViewClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;

#ifdef va_copy
	va_list ap;
	va_copy(ap, *app);
#else
	va_list ap = *app;
#endif

	while((selector = va_arg(ap, voidf)))
	{
		voidf method = va_arg(ap, voidf);

		if(selector == (voidf) draw)
			* (voidf*) & self->draw = method;

		if(selector == (voidf) onPress)
					* (voidf*) & self->onPress = method;
	}

#ifdef va_copy
	va_end(ap);
#endif

	return self;
}

const void* ViewClass, *View;

void initView()
{
	if(!ViewClass)
		ViewClass = new(Class, "ViewClass", Class, sizeof(struct ViewClass),
						ctor, ViewClass_ctor, 0);
	if(!View)
		View = new(ViewClass, "View", Object, sizeof(struct View),
				   ctor, View_ctor, draw, View_draw, onPress, View_onPress, 0);
}


