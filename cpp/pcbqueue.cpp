#include "pcbqueue.h"
#include "lock.h"
#include "pcb.h"
#include "pcblist.h"
#include "schedule.h"

PCBQueue::PCBQueue() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = 0;last = 0; numOfBlocked = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCBQueue::block(PCB *pcb,Time timeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* newPCB = new Elem(pcb,timeToWait);
	if(first) last->next = newPCB;
	else first = newPCB;
	last = newPCB;
	numOfBlocked++;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


PCB* PCBQueue::deblock(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem *old = 0;
	PCB* oldPCB = 0;
	if(first!=0){
		old = first;
		oldPCB = first->pcb;
		first = first->next;
		if(!first) last = 0;
		old->pcb = 0;
		delete old;
		numOfBlocked--;
	}else{
		#ifndef BCC_BLOCK_IGNORE
			unlock
		#endif
		return 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return oldPCB;
}

int PCBQueue::decrease(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem *curr = first, *prev = 0, *old = 0;
	int released = 0,changed = 0;
	while(curr){
		if(curr->myTimeToWait){
			curr->myTimeToWait--;
			if(curr->myTimeToWait==0){
				curr->pcb->fromTimer = 0;
				curr->pcb->setState(PCB::READY);
				Scheduler::put(curr->pcb);
				old = curr;
				if(!prev){
					curr = first->next;
					first = curr;
					if(!first) last = 0;
				}else{
					curr = curr->next;
					prev->next = curr;
					if(!curr) last = prev;
				}
				old->pcb = 0;
				delete old;
				released++;
				changed = 1;
			}
		}
		if(!changed){
			prev = curr;
			curr = curr->next;
		}else changed = 0;
	}
}

int PCBQueue::getNumOfBlocked() const {return numOfBlocked;}

PCBQueue::~PCBQueue() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(first){
		Elem *old = first;
		first = first->next;
		old->pcb = 0;
		delete old;
	}
	first = 0; last = 0;
	numOfBlocked = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

