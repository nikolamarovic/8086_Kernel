#include "hlist.h"
#include "lock.h"

HandlersList::HandlersList() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = 0; last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void HandlersList::removeAll(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(first){
		Elem* old = first;
		first = first->next;
		old->handler = 0;
		delete old;
	}
	first = 0;
	last = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void HandlersList::startAllHandlers(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	for(Elem* curr=first;curr!=0;curr=curr->next) curr->handler();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void HandlersList::copyList(const HandlersList& list){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	first = last = 0;//da li treba obrisati listu mozda prvo?
	Elem *curr = list.first;
	while(curr){
		add(curr->handler);
		curr=curr->next;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void HandlersList::swap(SignalHandler handler1, SignalHandler handler2){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* remember1 = 0, *remember2 = 0;
	for(Elem* curr = first;curr!=0;curr=curr->next){
		if(curr->handler == handler1) remember1 = curr;
		if(curr->handler == handler2) remember2 = curr;
	}

	if(remember1!=0 && remember2!=0){
		remember1->handler = handler2;
		remember2->handler = handler1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void HandlersList::add(SignalHandler hand){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* curr = new Elem(hand);
	if(first) last->next = curr;
	else first = curr;
	last = curr;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
HandlersList::~HandlersList() {
	removeAll();
}

