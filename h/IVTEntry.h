/*
 * IVTEntry.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "KEvent.h"
#include "System.h"

#define PREPAREENTRY(num, callOld)\
	void interrupt intr##num(...);\
	IVTEntry ivt##num(num, intr##num);\
	void interrupt intr##num(...){\
		(System::table[##num])->signalSem();\
		if(callOld==1){\
			ivt##num.oldInterrupt();\
		}\
		dispatch();\
	}\


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char ITVNo;
class KernelEv;


class IVTEntry{
public:
	IVTEntry(IVTNo ivtno, pInterrupt newInterrupt);
	~IVTEntry();
	void signalSem();
	friend class KernelEv;
	KernelEv* event;
	pInterrupt oldInterrupt;
	pInterrupt newInterrupt;
	IVTNo entryNum;

};



#endif /* IVTENTRY_H_ */
