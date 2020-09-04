/*
 * IdleThr.h
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_
#include "Thread.h"

class PCB;

class IdleThr:public Thread{
public:
	IdleThr():Thread(1024, 1) {
	};
	void run();
	ID getID();
	void kreni();
	PCB* getPCB();
};



#endif /* IDLETHR_H_ */
