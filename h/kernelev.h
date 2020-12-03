#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "ivtentry.h"
#include "pcb.h"
class IVTable;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	static IVTable table;
	void signal();
	void wait();

	void setOwner(PCB*);
private:
	int value;
	PCB *owner;
};

#endif
