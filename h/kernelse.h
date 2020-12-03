#ifndef KERNELSE_H_
#define KERNELSE_H_
#include "pcbqueue.h"
#include "semlist.h"

typedef unsigned int Time;

class KernelSem {
public:
	KernelSem(int init = 1);

	void setVal(int);
	int getVal() const;

	void decrease();

	int signal(int n = 0);
	int wait(Time maxTimeToWait);

	static SemList semList;
private:
	int value;
	void block(Time maxTimeToWait);
	void deblock();
	PCBQueue blockedPCBs;
};

#endif
