#include "Application.h"

#include "Assert.h"

Context::Context() {
	M_Stack();
	application = NULL;
	M_Unstack();
}

Context::~Context() {
	M_Stack();
	detachApplication();
	M_Unstack();
}

void Context::attachApplication(Application* app) {
	M_Stack();
	application = app;
	M_Unstack();
}

void Context::detachApplication() {
	M_Stack();
	application = NULL;
	M_Unstack();
}

Serial* Context::getSerial() {
	M_Stack();
	M_Assert(application != NULL, "Application is not attached! Have you called attachApplication()?");
	M_Unstack();
	return application->serial;
}

void Context::startActivity(Activity* activity) {
	M_Stack();
	M_Assert(application != NULL, "Application is not attached! Have you called attachApplication()?");
	application->startActivity(activity);
	M_Unstack();
}
