#include "ivtable.h"

#include "ivtentry.h"
#include "lock.h"

IVTable::IVTable() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	for(int i=0;i<maxTableSize;i++){tableEntries[i] = 0;}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

IVTable::~IVTable() {delete tableEntries;}

