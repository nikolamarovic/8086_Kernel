#ifndef HLIST_H_
#define HLIST_H_

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class HandlersList {
	struct Elem{
		Elem *next;
		SignalHandler handler;
		Elem(SignalHandler handler, Elem* next = 0){
			this->handler = handler;
			this->next = next;
		}
	};
	Elem* first, *last;
public:
	HandlersList();
	~HandlersList();
	void removeAll();
	void startAllHandlers();
	void add(SignalHandler hand);
	void swap(SignalHandler handler1,SignalHandler handler2);
	void copyList(const HandlersList &list);
};

#endif /* HLIST_H_ */
