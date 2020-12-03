#include "pcblist.h"
#include "lock.h"
#include "pcb.h"
#include "schedule.h"

PCBList::PCBList(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = 0; last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCBList::~PCBList(){
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
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCBList::add(PCB *pcb){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* tmp = new Elem(pcb);
	if(first) last->next = tmp;
	else first = tmp;
	last = tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCBList::remove(ID id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem *curr = first, *prev = 0;
	unsigned int end = 0;
	while(curr && !end){
		if(curr->pcb->getId()>id) end = 1;
		if(curr->pcb->getId() == id){
			if(prev == 0){
				first = first->next;
				if(!curr->next) last = 0;
			}else{
				prev->next = curr->next;
				if(!curr->next) last = prev;
			}
			curr->pcb = 0;
			curr->next = 0;
			delete curr;
			end = 1;
		}
		prev = curr;
		curr = curr->next;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB* PCBList::getPCBUsingId(ID id){
	Elem* curr;
	for(curr = first;curr;curr=curr->next) if(curr->pcb->getId()>=id) break;
	if(curr && (curr->pcb->getId() == id)) return curr->pcb;
	else return 0;
}

void PCBList::setFree(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(first){
		Elem* old = first;
		first->pcb->setState(PCB::READY);
		Scheduler::put(first->pcb);
		first = first->next;
		old->pcb = 0;
		delete old;
	}
	first = 0; last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
