/*
 * KSem.h
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_


#include "PCBList.h"
#include "thread.h"


class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();
	int value();
	int wait(Time maxTimeToWait);
	int signal(int n = 0);
	PCBList*  blocked;
		int val;
private:
	friend class PCBList;
	friend class PCB;
	friend class System;
	friend class SemList;

};

#endif /* KSEM_H_ */
