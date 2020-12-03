#ifndef TIMER_H_
#define TIMER_H_

extern void tick();
void interrupt timer();

class Timer {
	Timer(){}
public:
	static int explicitContextChange;
	static unsigned lockFlag;
};

#endif
