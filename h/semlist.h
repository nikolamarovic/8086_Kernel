#ifndef SEMLIST_H_
#define SEMLIST_H_

class KernelSem;

class SemList {
private:
	struct Elem{
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem* sem, Elem* next = 0){
			this->sem = sem;
			this->next = next;
		}
	};
	Elem *first, *last;
	SemList(const SemList &){first = 0; last = 0; }
	void operator = (const SemList&){ }
public:
	SemList();

	void add(KernelSem *newSem);
	void decreaseAll();


	~SemList();
};


#endif /* SEMLIST_H_ */
