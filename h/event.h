#ifndef EVENT_H_
#define EVENT_H_

#include "ivtentry.h"
typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};


#define PREPAREENTRY(number, toCallOld)\
void interrupt interrupt_##number(...);\
IVTEntry entry_##number(number, interrupt_##number);\
void interrupt interrupt_##number(...) {\
entry_##number.signal();\
	if (toCallOld) entry_##number.callOld();\
}

#endif
