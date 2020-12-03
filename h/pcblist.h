#ifndef PCBLIST_H_
#define PCBLIST_H_

typedef int ID;
class PCB;

class PCBList {
private:
	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb, Elem* next = 0){
			this->pcb = pcb;
			this->next = next;
		}
	};
	Elem *first, *last;
	PCBList(const PCBList &){first = 0; last = 0; }
	void operator = (const PCBList&){ }
public:
	PCBList();

	PCB* getPCBUsingId(ID id);
	void add(PCB *pcb);
	void remove(ID id);
	void setFree();

	~PCBList();
};

#endif
