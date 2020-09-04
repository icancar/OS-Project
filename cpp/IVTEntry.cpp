/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */
#include <dos.h>
#include "IVTEntry.h"
#include "System.h"


IVTEntry::IVTEntry(IVTNo ivtn, pInterrupt newInterrupt) {
	this->newInterrupt=newInterrupt;
	this->entryNum=ivtn;
	this->event=0;
#ifndef BCC_BLOCK_IGNORE
	this->oldInterrupt=getvect(ivtn);
	setvect(ivtn, newInterrupt);
#endif
	System::table[ivtn]=this;

}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum, oldInterrupt);
	System::table[entryNum]=0;
#endif
}

void IVTEntry::signalSem(){
	if(this->event!=0) event->signal();
}
