#include "slist.h"
#include "lock.h"
#include "pcb.h"
#include "syprintf.h"
unsigned SignalsList::gotZero = 0;

SignalsList::SignalsList() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = 0;
	last = 0;
	size = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
void SignalsList::add(SignalId id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem *curr = new Elem(id);
	size++;
	//syncPrintf("UBACUJE SE THREAD(%d) I PRIMA SIGNAL(%d)\n",PCB::running->getId(),id);
	if(id == 0) gotZero = 1;
	if(first) last->next = curr;
	else first = curr;
	last = curr;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void SignalsList::startAll(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	/*
	if(gotZero!=0){
		removeAll();
		gotZero = 0;//aloo
		PCB::running->killThread();
		return;
	}
	for(Elem* curr=first;curr;curr=curr->next) PCB::running->handlersList[curr->id].startAll();

	*/
	for(Elem* curr=first;curr;curr=curr->next){
		if(curr->id !=0){
			if(!PCB::running->blockedSignals[curr->id] && !PCB::blockedSignalsGlobally[curr->id]){
				PCB::running->handlersList[curr->id].startAllHandlers();
			}
		}
		else{
			if(!PCB::running->blockedSignals[curr->id] && !PCB::blockedSignalsGlobally[curr->id]) {
				removeAll();
				//PCB::running->killThread();
#ifndef BCC_BLOCK_IGNORE
				unlock
#endif
				PCB::running->handlersList[0].startAllHandlers();
				return;
			}
		}
	}
	removeAll();
#ifndef BCC_BLOCK_IGNORE
				unlock
#endif
}

void SignalsList::setSize(unsigned x){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->size = x;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
unsigned SignalsList::listSize(){
	return size;
}

void SignalsList::removeAll(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(first){
		Elem* old = first;
		first = first->next;
		delete old;
	}
	first = 0;
	last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
SignalsList::~SignalsList() {
	removeAll();
}

