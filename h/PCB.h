/*
 * PCB.h
 *
 *  Created on: Aug 2, 2020
 *      Author: OS1
 */

#ifndef _PCB_H_
#define _PCB_H_
#include "thread.h"
#include <iostream.h>

class PCBList;
class SignalList;
class HandlerList;

class PCB {
public:
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice );
	enum Status {READY, BLOCKED, RUNNING, FINISHED, CREATED, IDLE};
	PCBList* waitingForMe;
	ID ID;
	Status getStatus();
	Thread* getThread();
	static void wrapper();
	void allocateStack();
	volatile Status status;
	PCB* parent;
	int parentID;

	volatile unsigned ss;
	volatile unsigned sp;
	volatile unsigned bp;
	Time timeSlice;
	volatile Time timeBlocked;
	volatile Time timeLeft;
	int checkTime;


	SignalList* arrivedSignals;
	HandlerList* handlers[16];
	int blockedSignals[16];
	void checkArrivedSignals();

	unsigned* stack;
	Thread* MyThread;
	StackSize stackSize;
	volatile int size;
	~PCB();
private:

	friend class KernelSem;
	friend class System;
	friend class PCBList;

};




#endif /* PCB_H_ */
