#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Idle: public Thread {
	Idle(): Thread(defaultStackSize, 1){}
	static Idle* singleIdle;
public:
	static Idle* getIdle() {
		if (singleIdle==0)singleIdle = new Idle();
		return singleIdle;
	}
protected:
	void run();
};
#endif
