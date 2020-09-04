/*
 * KSem.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */


#include "KSem.h"
#include "System.h"
#include "PCB.h"
#include "Schedule.h"

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

KernelSem::KernelSem(int init){
	lock
	this->val=init;
	this->blocked=new PCBList();
	unlock
}

KernelSem::~KernelSem()	{
	delete blocked;
}

int KernelSem::value() {
	return this->val;
}

int KernelSem::wait(Time maxTimeToWait){
	int ret=0;
	lock

	if(--val<0){
		if(maxTimeToWait==0) {  //wait se ponasa standardno, nit treba blokirati na neodredjeno vrijeme
			System::running->checkTime=0;
			System::running->timeBlocked=0;
			System::running->status=PCB::BLOCKED;
			this->blocked->add((PCB*)System::running);
			ret=1;
			unlock
			dispatch();
		}
		else {   //nit se blokira maxTimeToWait time sliceova
			System::running->timeBlocked=maxTimeToWait;
			System::running->checkTime=1;
			System::running->status=PCB::BLOCKED;
			this->blocked->add((PCB*)System::running);
			unlock
			dispatch();
			if(System::running->checkTime==1){ //nit deblokirana zbog isteka vremena
					System::running->checkTime=0;
					ret=1;
			}
		}
		//unlock
		return ret;
	}
	unlock
	return 1;
}

int KernelSem::signal(int i){
	lock
	if(i==0){ //operacija signal se ponasa na standardan nacin
		if(val++<0){ //treba odblokirati jednu nit
			PCB* toUnblock=this->blocked->removeFirst();
			if(toUnblock!=0){
				if(toUnblock->checkTime){
					toUnblock->timeBlocked=0;
					}
				toUnblock->status=PCB::READY;
				Scheduler::put(toUnblock);
			}
		}
		unlock
		return 0; //povratna vrijednost 0
	}
	if(i>0) {  //treba odblokirati onoliko niti koliko je parametar i,ako nema toliko blokiranih, odblokirati sve
		this->val+=i;
		if(i>=this->blocked->size){
			this->blocked->unblockAll();
			unlock
			return i;  //povratna vrijednost je koliko je niti odblokirano
		}
		else {
			int a;// size od liste je veci od i;
			for(a=0;a<i;a++){
				PCB* toUnblock=this->blocked->removeFirst();
							if(toUnblock!=0){
								if(toUnblock->checkTime){
									toUnblock->timeBlocked=0;
									}
								toUnblock->status=PCB::READY;
								Scheduler::put(toUnblock);
							}
			}
			unlock
			return i;
		}
	}
	else {
		unlock
		return i; //povratna vrijednost je onolika koliko je i, jer je negativno
	}
}

