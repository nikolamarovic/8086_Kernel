#ifndef IVTABLE_H_
#define IVTABLE_H_

#define maxTableSize 256

class IVTEntry;

class IVTable {
public:
	IVTable();
	IVTEntry *&operator[](int index){return tableEntries[index];}
	virtual ~IVTable();
private:
	IVTEntry* tableEntries[maxTableSize];
};

#endif
