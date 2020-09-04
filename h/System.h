/*
 * System.h
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "PCBList.h"
#include "IdleThr.h"
#include "thread.h"
#include <dos.h>

class PCBList;
class Thread;
class IVTEntry;
class SemaphoreList;
typedef void interrupt (*pInterrupt)(...);



void kill();

class System {
public:
	static PCBList* allPCB;
	static IdleThr* idleThread;
	static SemaphoreList* allSem;
	static void createMain();
	static IVTEntry* table[];
	volatile static int contextSwitchOnDemand;
	static int lockFlag;
	static void init();
	static void restore();
	static ID startID ;
	volatile static PCB* running;
	static pInterrupt old;
	volatile static Time counter;
	static int globalBlockedSignals[];
	static void interrupt timer(...);

};

#endif /* SYSTEM_H_ */
