#include "kernel.h"

int main(int argc, char *args[]){
	#ifndef BCC_BLOCK_IGNORE
	lock
	#endif
	System::inic();

	Idle *idle = Idle::getIdle(); idle->start();
	Thread *mainThread = PCB::getMainThread();
	int returnValue = userMain(argc, args);

	delete idle;
	delete mainThread;

	System::restore();
	#ifndef BCC_BLOCK_IGNORE
	unlock
	#endif
	return 0;
}

