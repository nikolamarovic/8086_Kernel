#ifndef SLIST_H_
#define SLIST_H_

typedef unsigned SignalId;

class SignalsList {
	struct Elem{
		SignalId id;
		Elem *next;
		Elem(SignalId id,Elem* next = 0){
			this->id = id;
			this->next = next;
		}
	};
	Elem* first, *last;
public:
	SignalsList();
	unsigned size;
	void setSize(unsigned x);
	static unsigned gotZero;
	void add(SignalId id);
	void removeAll();
	void startAll();
	unsigned listSize();
	~SignalsList();
};

#endif /* SLIST_H_ */
