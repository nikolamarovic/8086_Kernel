#ifndef PCBQUEUE_H_
#define PCBQUEUE_H_

class PCB;

typedef unsigned int Time;

class PCBQueue {
	struct Elem{
		PCB *pcb;
		Time myTimeToWait;
		Elem *next;
		Elem(PCB* pcb,Time myTimeToWait, Elem* next = 0){
			this->pcb = pcb;
			this->myTimeToWait = myTimeToWait;
			this->next = next;
		}
	};
	Elem *first, *last;
	int numOfBlocked;
	PCBQueue(const PCBQueue&){first = 0; last = 0; numOfBlocked = 0;}
	void operator = (const PCBQueue &){}
public:
	PCBQueue();

	void block(PCB* pcb,Time timeToWait);
	PCB* deblock();
	int getNumOfBlocked() const;
	int decrease();
	~PCBQueue();
};

#endif
