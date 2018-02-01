#ifndef VIEW_H_
#define VIEW_H_

#include "DrawOperations.h"
#include "Object.h"

extern const void* View;

struct View {
	const struct Object _;
	int x, y, width, height;
	int visibility;

	void* parent;
	struct Children* head;
	struct Children* tail;

	int opaque;
	int invalid;
};

struct Children {
	struct Children* next;
	void* child;
};

#define x(p)(((struct View*)(p)) -> x)
#define y(p)(((struct View*)(p)) -> y)
#define width(p)(((struct View*)(p)) -> width)
#define height(p)(((struct View*)(p)) -> height)
#define visibility(p)(((struct View*)(p)) -> visibility)
#define opaque(p)(((struct View*)(p)) -> opaque)
#define invalid(p)(((struct View*)(p)) -> invalid)
#define parent(p)(((struct View*)(p)) -> parent)

void draw(const void* self, int x, int y);
void move(void* self, int dx, int dy);
void setVisibility(void* self, int visible);

void onPress(const void* self, int x, int y);

void attachChild(void* self, void* child);
void detachChild(void* self, void* child);

void invalidate(void* self);
void invalidateChildrenInParent(void* self);
void invalidateChildren(void* self);

extern const void* ViewClass;

void super_draw(const void* class, const void* self, int x, int y);
void super_onPress(const void* class, const void* self, int x, int y);

struct ViewClass {
	const struct Class _;
	void (*draw) (const void* self, int x, int y);
	void (*onPress) (const void* self, int x, int y);
};

void initView(void);

#endif /* VIEW_H_ */
