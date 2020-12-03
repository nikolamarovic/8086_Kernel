#include "kernelse.h"

#include "lock.h"
#include "pcb.h"
#include "pcbqueue.h"
#include "syprintf.h"
#include "thread.h"

SemList  KernelSem::semList;

KernelSem::KernelSem(int init) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->value = init;
	semList.add(this);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelSem::setVal(int init){this->value = init;}

int KernelSem::getVal() const {return this->value;}

void KernelSem::decrease(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	value += blockedPCBs.decrease();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int KernelSem::signal(int n){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(n>0){
		int oldValue = value;
		int blockedItems = blockedPCBs.getNumOfBlocked();
		if(n>blockedItems){
			for(int i=0;i<blockedItems;i++){
				if(value++ < 0) deblock();
			}
			value = oldValue + n;
			#ifndef BCC_BLOCK_IGNORE
			unlock
			#endif
			return blockedItems;
		}else{
			for(int i=0;i<n;i++){
				if(value++ < 0) deblock();
			}
			value = oldValue + n;
			#ifndef BCC_BLOCK_IGNORE
			unlock
			#endif
			return n;
		}
	}else if(n<0){
		#ifndef BCC_BLOCK_IGNORE
		unlock
		#endif
		return n;
	}else{//jednako nula
		if(value++<0) deblock();
		#ifndef BCC_BLOCK_IGNORE
			unlock
		#endif
		return 0;
	}
}

void KernelSem::deblock(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::running->fromTimer = 1;
	PCB* released = blockedPCBs.deblock();
	released->setState(PCB::READY);
	Scheduler::put(released);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int KernelSem::wait(Time maxTimeToWait){
/*
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(--value<0){
		#ifndef BCC_BLOCK_IGNORE
			unlock
		#endif
		block(maxTimeToWait);
		return PCB::running->fromTimer;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return 1;
*/
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(--value<0){
		#ifndef BCC_BLOCK_IGNORE
			unlock
		#endif
		block(maxTimeToWait);
		if(maxTimeToWait==0) return 1;
		else{
			PCB::running->fromTimer = 1;
			int ret = PCB::running->fromTimer;
			return 0;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return 1;
}

void KernelSem::block(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	PCB::running->setState(PCB::BLOCKED);
	blockedPCBs.block(PCB::running,maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	dispatch();
}



