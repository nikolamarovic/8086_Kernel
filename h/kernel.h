#ifndef KERNEL_H_
#define KERNEL_H_

#include<dos.h>
#include<iostream.h>

#include "event.h"
#include "idle.h"
#include "ivtable.h"
#include "ivtentry.h"
#include "kernelev.h"
#include "kernelse.h"
#include "pcb.h"
#include "pcblist.h"
#include "pcbqueue.h"
#include "schedule.h"
#include "semaphor.h"
#include "semlist.h"
#include "syprintf.h"
#include "system.h"
#include "thread.h"
#include "timer.h"
#include "hlist.h"
#include "slist.h"

extern int userMain(int argc, char **args);

#endif
