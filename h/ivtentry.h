#ifndef IVTENTRY_H_
#define IVTENTRY_H_

class KernelEv;
typedef unsigned char IVTNo;

class IVTEntry {
public:
	IVTEntry(int id, void interrupt (*newRoutine)(...));
	void callOld();
	void signal();
	void setKernelEv(KernelEv* ev);
	~IVTEntry();
private:
	KernelEv *kernelEv;
	void interrupt (*oldRoutine)(...);
	int id;
};

#endif
