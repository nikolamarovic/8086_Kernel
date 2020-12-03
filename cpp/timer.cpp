#include "timer.h"
#include "syprintf.h"
#include <dos.h>
#include "kernelse.h"
#include "pcb.h"
#include "schedule.h"
#include "thread.h"

static int counter = defaultTimeSlice;
unsigned Timer::lockFlag = 1;
int Timer::explicitContextChange = 0;
//pomocne sp,ss,bp
static unsigned tsp = 0;
static unsigned tss = 0;
static unsigned tbp = 0;

void interrupt timer(){
	if(!Timer::explicitContextChange){
		tick();
		KernelSem::semList.decreaseAll();
	}

	if(!(PCB::running->getTimeSlice()==0) && !Timer::explicitContextChange) counter--;
	if(Timer::explicitContextChange || (counter==0 && !(PCB::running->getTimeSlice()==0))){
		if(Timer::lockFlag){
			Timer::explicitContextChange = 0;
			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tsp,sp
				mov tss,ss
				mov tbp,bp
			}
			#endif

			PCB::running->ss = tss;
			PCB::running->sp = tsp;
			PCB::running->bp = tbp;
			//syncPrintf("TIMER! ID PRE:%d\n",PCB::running->getId());
			if(PCB::running!=0 && PCB::running->getState() == PCB::RUNNING && PCB::running!=PCB::getIdlePCB()){
				//syncPrintf("TIMER! ID POSLE:%d\n",PCB::running->getId());

				Scheduler::put(PCB::running);
				PCB::running->setState(PCB::READY);
			}

			PCB::running=Scheduler::get();
			if(PCB::running == 0) PCB::running = PCB::getIdlePCB();

			PCB::running->setState(PCB::RUNNING);


			tss = PCB::running->ss;
			tsp = PCB::running->sp;
			tbp = PCB::running->bp;

			counter = PCB::running->getTimeSlice();

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif

			if(PCB::running->sizeWaitingSignalsList()>0) PCB::running->startAllSignals();
		}
		else Timer::explicitContextChange = 1;
	}
	#ifndef BCC_BLOCK_IGNORE
	if(!Timer::explicitContextChange) asm int 60h;
	#endif
}


