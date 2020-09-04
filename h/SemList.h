/*
 * SemList.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "KSem.h"
class KernelSem;

class SemaphoreList{

public:
	SemaphoreList():head(0), tail(0), numofSem(0){};
	~SemaphoreList();
	void add(KernelSem* s);
	void refresh();
private:
	struct Elem {
			KernelSem* sem;
			int size;
			Elem* next;
			Elem(KernelSem* p) :
					sem(p),size(p->blocked->size) ,next(0) {
			}
			;
		};
		Elem* head;
		Elem* tail;
		volatile int numofSem;


};



#endif /* SEMLIST_H_ */
