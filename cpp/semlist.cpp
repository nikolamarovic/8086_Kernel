#include "semlist.h"

#include "kernelse.h"
#include "lock.h"
#include "schedule.h"

SemList::SemList() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = 0;
	last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

SemList::~SemList() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(first){
		Elem *old = first;
		first = first->next;
		old->sem = 0;
		delete old;
	}
	first = 0;last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void SemList::add(KernelSem *newSem){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* newElem = new Elem(newSem);
	if(!first) first = newElem;
	else last->next = newElem;
	last = newElem;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void SemList::decreaseAll(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	for(Elem* curr = first;curr;curr=curr->next) curr->sem->decrease();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

