#include "kernelev.h"

#include<dos.h>
#include "ivtable.h"
#include "thread.h"

IVTable KernelEv::table;

KernelEv::KernelEv(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	value = 0;
	table[ivtNo]->setKernelEv(this);
	setOwner(PCB::running);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelEv::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(value){
		value = 0;
		owner->setState(PCB::READY);
		Scheduler::put(owner);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelEv::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(owner == PCB::running){
		value = 1;
		owner->setState(PCB::BLOCKED);
		#ifndef BCC_BLOCK_IGNORE
		unlock
		#endif
		dispatch();
		return;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
void KernelEv::setOwner(PCB* pcb){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	owner = pcb;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

