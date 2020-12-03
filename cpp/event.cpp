#include "event.h"

#include "ivtable.h"
#include "ivtentry.h"
#include "kernelev.h"
#include "lock.h"


Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
void Event::signal(){myImpl->signal();}

void Event::wait(){myImpl->wait();}

Event::~Event(){delete myImpl;}
