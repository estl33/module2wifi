#include "Application.h"

#include "Assert.h"
#include "Log.h"

void Activity::onCreate() {
	M_Stack();
	Log_Low("Activity", "OnCreate");
	M_Unstack();
}

void Activity::onDestroy() {
	M_Stack();
	M_Unstack();
}

string Activity::getName() {
	M_Stack();
	M_Assert(0, "Activity::getName() is not implemented!");
	M_Unstack();
	return "";
}

void Activity::attachView(View* view) {
	M_Stack();
	rootView.attachView(view);
	M_Unstack();
}

void Activity::detachView() {
	M_Stack();
	M_Unstack();
}
