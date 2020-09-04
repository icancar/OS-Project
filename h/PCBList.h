/*
 * PCBList.h
 *
 *  Created on: Aug 2, 2020
 *      Author: OS1
 */

#ifndef _PCBLIST_H_
#define _PCBLIST_H_
#include "PCB.h"
#include "SCHEDULE.h"
#include <iostream.h>


#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

class PCBList {
public:
	PCBList():head(0), tail(0), size(0) {}
	void add(PCB* p);
	PCB* removeFirst();
	~PCBList();
	Thread* findID(ID id);
	void removeID(ID id);
	void unblockAll();
	int checkTimeBlocked();
	void unblockID(ID id);
	volatile int size;

private:
	friend class PCB;
	friend class KernelSem;
	struct Elem {
		PCB* info;
		Elem* next;
		Elem(PCB* p) :
				info(p), next(0) {
		}
		;
	};
	Elem* head;
	Elem* tail;


};

#endif /* PCBLIST_H_ */
