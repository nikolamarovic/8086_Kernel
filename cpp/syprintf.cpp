#include "syprintf.h"

#include <stdio.h>
#include <stdarg.h>
#include "timer.h"

void syncPrintf(const char* format, ...) {
	Timer::lockFlag = 0;
	va_list args;
	va_start (args, format);
	vprintf(format, args);
	va_end(ap);
	Timer::lockFlag = 1;
}


