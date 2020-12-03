#include "idle.h"
Idle* Idle::singleIdle = 0;

void Idle::run() {
	while(1){
		int i = 0;
		while (i<30000){
			int j = 0;
			for (int k = 0; k<10000; ++k)
				if (k%5==0) j++;
			j++;
			if (i==30000) i=0;
		}
	}
}

