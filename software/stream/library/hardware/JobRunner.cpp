#include "JobRunner.h"

#include "Assert.h"

JobRunner* timer_instance = 0;

alt_u32 alarm_handler(void* context) {
	Alarm* alarm = (Alarm*) context;
	if(alarm->fctPtr()) return 0;
	else return alt_ticks_per_second()*alarm->delay/1000.0;
}

JobRunner* JobRunner::getInstance(){
	M_Stack();
	if(timer_instance == 0) {
		timer_instance = new JobRunner;
	}
	M_Unstack();
	return timer_instance;
}

Alarm* JobRunner::createAlarm(int when, int delay, timer_fct fctPtr) {
	M_Stack();
	Alarm* alarm = new Alarm;
	alarms.insertEnd((void *) alarm);
	alarm->when = when;
	alarm->delay = delay;
	alarm->fctPtr = fctPtr;
	M_Unstack();
	return alarm;
}

void JobRunner::schedule(timer_fct fctPtr, int when) {
	return JobRunner::scheduleRepeating(fctPtr, when, 0);
}

void JobRunner::scheduleRepeating(timer_fct fctPtr, int when, int delay) {
	M_Stack();
	M_Assert(when >= 0, "when must be positive");
	M_Assert(delay >= 0, "delay must be positive");
	M_Assert(fctPtr != 0, "fctPtr cannot be NULL");

	Alarm* alarm = JobRunner::createAlarm(when, delay, fctPtr);
	alt_alarm_start(&alarm->alarm, alt_ticks_per_second()*when/1000.0, alarm_handler, (void*) alarm);
	M_Unstack();
}




