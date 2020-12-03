#include "pcb.h"
#include "schedule.h"
#include "syprintf.h"
#include "thread.h"

PCB* PCB::running = 0;
PCB* PCB::idlePCB = 0;
PCB* PCB::mainPCB = 0;
Thread* PCB::mainThread = 0;

unsigned PCB::blockedSignalsGlobally[16] = {0};

ID PCB::IDtmp = -2;
PCBList PCB::PCBs;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->myStackSize = stackSize;
	this->myTimeSlice = timeSlice;
	this->myThread = myThread;
	this->id = ++IDtmp;
	this->state = NEW;
	this->fromTimer = 1;
	this->stack = 0;

	if(id==-1) {//idle
		idlePCB = this;
		parentId = -2;
		for(int i=0;i<16;i++) blockedSignals[i] = 1;//uzeta pretpostavka da su svi blokirani u idle
	}
	else if(id>0) {//userThread
		PCBs.add(this);
		for(int i=0;i<16;i++){
			handlersList[i].copyList(running->handlersList[i]);
			blockedSignals[i] = running->blockedSignals[i];
		}
	}else{//mainThread
		parentId = -3;
		handlersList[0].add(killThread);
		for(int i=0;i<16;i++) blockedSignals[i] = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

/*bez signala
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->myStackSize = stackSize;
	this->myTimeSlice = timeSlice;
	this->myThread = myThread;
	this->id = ++IDtmp;
	if(id==-1) idlePCB = this;
	else if(id>0) PCBs.add(this);
	this->state = NEW;
	this->threadFromTimer = 1;
	this->stack = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
*/
}

void PCB::createStack(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	if(myStackSize>maxStackSize) myStackSize = maxStackSize;
	else if(myStackSize<minStackSize) myStackSize = minStackSize;
	stack = new unsigned[myStackSize];

	stack[myStackSize-1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit

#ifndef BCC_BLOCK_IGNORE
	stack[myStackSize-2] = FP_SEG(startRunMethod);
	stack[myStackSize-3] = FP_OFF(startRunMethod);

	sp = FP_OFF(stack+myStackSize-12); //svi sacuvani registri pri ulasku u interrupt rutinu
	ss = FP_SEG(stack+myStackSize-12);
	bp = FP_OFF(stack+myStackSize-12);
#endif


#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread* PCB::getParent(ID id){ return PCB::getThreadById(id);}

void PCB::startRunMethod(){//wrapper
	running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	running->state = FINISHED;
	running->waitingPCBs.setFree();

	Thread* myParent = getParent(running->parentId);
	if(myParent!=0) myParent->myPCB->signal(1);//roditelju

	if(!running->blockedSignals[2] && !PCB::blockedSignalsGlobally[2]) running->handlersList[2].startAllHandlers();

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	dispatch();

/* bez signala
running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	running->state = FINISHED;
	running->waitingPCBs.setFree();

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	dispatch();
*/
}

void PCB::start(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(state == NEW){
		createStack();
		state = READY;
		Scheduler::put(this);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::setId(ID id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->id = id;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

unsigned PCB::isFinished(PCB::State st){
	if(st==PCB::FINISHED) return 1;
	else return 0;
}
ID PCB::getId(){return id;}

void PCB::setState(State s){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	state = s;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB::State PCB::getState(){return state;}

Time PCB::getTimeSlice(){return myTimeSlice;}

PCB* PCB::getIdlePCB(){ return idlePCB;}

Thread* PCB::getThreadById(ID id){
	if(id == 0) return mainThread;
	if(id == -1) return idlePCB->myThread;

	PCB* tmp = PCBs.getPCBUsingId(id);
	if(tmp!= 0) return tmp->myThread;

	return 0;
}

Thread* PCB::getMainThread(){
	if(mainThread == 0){
		mainThread = new Thread(defaultStackSize, defaultTimeSlice);
		mainPCB = mainThread->myPCB;
		mainPCB->state = RUNNING;
		running = mainPCB;
	}
	return mainThread;
}

void PCB::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(state == FINISHED || state == NEW || running == this || running == idlePCB || mainPCB == this || idlePCB == this){
		#ifndef BCC_BLOCK_IGNORE
			unlock
		#endif
	}else{
		running->state = BLOCKED;
		waitingPCBs.add(running);
		#ifndef BCC_BLOCK_IGNORE
		unlock
		#endif
		dispatch();
	}
}

PCB::~PCB(){
	PCBs.remove(id);
	delete stack;
}

//signali

void PCB::delPCB(){
	PCBs.remove(id);
	delete stack;
}

unsigned PCB::sizeWaitingSignalsList(){return waitingSignals.listSize();}

void PCB::startAllSignals(){
	//syncPrintf("VELICINA LISTE: %d\n",waitingSignals.listSize());
	waitingSignals.startAll();
}
void PCB::killThread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//syncPrintf("USAO U KILL THREAD\n");
	running->setState(PCB::FINISHED);
	running->waitingPCBs.setFree();
	//running->delPCB();
	//Thread *parent = getParent(running->parentId);
	//if(parent!=0) parent->myPCB->signal(1);

	//running->waitingSignals.removeAll();
	//PCBs.remove(running->getId());

	//delete running->stack;
	//running->stack = 0;
	//running->myThread = 0;
	//for(unsigned i=0;i<16;i++) running->handlersList[i].removeAll();

	//syncPrintf("IZASAO IZ KILL THREAD\n");
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	dispatch();
}

void PCB::signal(SignalId signal){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(signal!=2 && signal!=1 && signal<=15){
		//syncPrintf("POZIVA SE SIGNAL %d\n",signal);
		waitingSignals.add(signal);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::registerHandler(SignalId signal, SignalHandler handler){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(signal!=0 && signal>2 && signal<=15) handlersList[signal].add(handler);
	else if(signal==0) handlersList[0].add(killThread);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
void PCB::unregisterAllHandlers(SignalId id){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(id>2 && id<=15) handlersList[id].removeAll();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(id>2 && id<=15) handlersList[id].swap(hand1,hand2);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::blockSignal(SignalId signal){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	blockedSignals[signal] = 1;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::blockSignalGlobally(SignalId signal){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	blockedSignalsGlobally[signal] = 1;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::unblockSignal(SignalId signal){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	blockedSignals[signal] = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

}

void PCB::unblockSignalGlobally(SignalId signal){
	if(isFinished(running->state)) return;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	blockedSignalsGlobally[signal] = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
