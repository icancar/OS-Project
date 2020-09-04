/*
 * KEvent.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef KEVENT_H_
#define KEVENT_H_
typedef unsigned char IVTNo;

class PCB;

class KernelEv {
public:
	KernelEv(IVTNo ivtno);
	~KernelEv();
	void wait();
	void signal();
private:
	int val;
	IVTNo entry;
	PCB* owner;


};



#endif /* KEVENT_H_ */
