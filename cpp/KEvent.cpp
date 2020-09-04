/*
 * KEvent.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "KEvent.h"
#include "System.h"
#include "IVTEntry.h"
#include "Schedule.h"
#include "PCB.h"

KernelEv::KernelEv(IVTNo ivtno){
	this->owner=(PCB*)System::running;
	this->val=0;
	this->entry=ivtno;
	if(System::table[ivtno]!=0) System::table[ivtno]->event=this;

}
KernelEv::~KernelEv(){
	this->owner=0;
	//if(System::table[entry]!=0) System::table[entry]->event=this;

}

void KernelEv::wait() {
	if(System::running==this->owner){
		lock
		if(--val<0){
			this->owner->status=PCB::BLOCKED;
			unlock
			dispatch();
		}
		else unlock
	}

}

void KernelEv::signal(){
	lock
	if(this->val++<0){
		this->owner->status=PCB::READY;
		Scheduler::put(owner);
	}
	unlock
}
