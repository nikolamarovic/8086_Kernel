#ifndef PCB_H_
#define PCB_H_
#include "pcblist.h"
#include "Thread.h"
#include "hlist.h"
#include "slist.h"
class Thread;

typedef unsigned long StackSize;
const StackSize minStackSize = 64, maxStackSize = 4096;
typedef unsigned int Time;
typedef int ID;

class PCB {
public:
	enum State{NEW,READY, RUNNING, BLOCKED, FINISHED};

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
	void waitToComplete();
	void start();//setThreadReady

	unsigned int fromTimer;

	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;

	Time getTimeSlice();
	void setId(ID id);
	ID getId();
	void setState(State s);
	State getState();

	static PCB* running;
	static PCB* getIdlePCB();
	static Thread* getMainThread();
	static Thread* getThreadById(ID id);
	static void startRunMethod();
	static unsigned isFinished(State st);
	static Thread* getParent(ID id);
	void delPCB();

	~PCB();

	//signali
	unsigned blockedSignals[16];
	static unsigned blockedSignalsGlobally[16];
	HandlersList handlersList[16];
	void startAllSignals();
	unsigned sizeWaitingSignalsList();

	static void killThread();

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
private:
	static ID IDtmp;
	ID id;
	unsigned int* stack;
	StackSize myStackSize;
	Time myTimeSlice;
	State state;
	Thread* myThread;

	PCBList waitingPCBs;

	static PCBList PCBs;
	static PCB* idlePCB;
	static PCB* mainPCB;
	static Thread* mainThread;

	void createStack();

	//signali
	SignalsList waitingSignals;
	ID parentId;
};

#endif
