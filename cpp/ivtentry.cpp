#include "ivtentry.h"

#include <dos.h>
#include "ivtable.h"
#include "kernelev.h"
#include "lock.h"


IVTEntry::IVTEntry(int id, void interrupt (*newRoutine)(...)): kernelEv(0), oldRoutine(0) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->id = id;
	KernelEv::table[id] = this;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(id);
	setvect(id,newRoutine);
	unlock
#endif
}

void IVTEntry::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	kernelEv->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void IVTEntry::setKernelEv(KernelEv* ev){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->kernelEv = ev;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void IVTEntry::callOld(){
	if(oldRoutine != 0) (*oldRoutine)();
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock
	KernelEv::table[id] = 0;
	setvect(id,oldRoutine);
	unlock
#endif
}

