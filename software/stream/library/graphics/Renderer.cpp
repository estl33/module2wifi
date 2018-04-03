#include "Application.h"

#include "Log.h"
#include "Assert.h"

Renderer* render_instance = 0;

Renderer* Renderer::getInstance(){
	M_Stack();
	if(render_instance == 0) {
		render_instance = new Renderer;
	}
	M_Unstack();
	return render_instance;
}

void Renderer::setRootView(View* view) {
	M_Stack();
	view->parent = 0;
	view->x = 0;
	view->y = 0;
	view->width = XRES;
	view->height = YRES;
	rootView = view;
	M_Unstack();
}

void Renderer::render() {
	M_Stack();
	if(rootView != 0) {
		rootView->measure();
		rootView->draw(*rootView->bounds);
	} else {
		Log_Warn("Renderer", "No view to render");
	}
	M_Unstack();
}


