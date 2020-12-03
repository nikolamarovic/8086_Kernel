#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB = new PCB(stackSize, timeSlice, this);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getId() {
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::running->getId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock //asm cli;
#endif
	Timer::explicitContextChange = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	unlock //asm sti;
#endif
}

//signali
void Thread::signal(SignalId signal){
	myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal,handler);
}
void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id,hand1,hand2);
}

void Thread::blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal){
	PCB::unblockSignalGlobally(signal);
}
