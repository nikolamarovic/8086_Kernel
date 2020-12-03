#include "semaphor.h"
#include "kernelse.h"
#include "lock.h"

Semaphore::Semaphore(int init){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
int Semaphore::val() const { return myImpl->getVal();}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

Semaphore::~Semaphore(){
	delete myImpl;
}
