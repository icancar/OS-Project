#include "PCB.h"
#include <dos.h>
#include "PCBList.h"
#include "System.h"
#include "SList.h";
#include "HList.h"


#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}


extern void kill();



PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice) {
	this->MyThread = myThread;

	if (stackSize < 65536) {
		this->stackSize = stackSize;
	} else
		this->stackSize = 65536;
	this->size = stackSize / sizeof(unsigned);

	this->timeSlice = timeSlice;
	this->timeLeft = timeSlice;
	this->timeBlocked=0;
	this->checkTime=0;
	this->ID = System::startID++;
	this->status = PCB::CREATED;
	this->waitingForMe=new PCBList();
	allocateStack();
	if(System::running!=0){
	this->parent=(PCB*)System::running;
	}
	else this->parent=0;
	if (System::running!=0){
	this->parentID=System::running->ID;
	}
	else parentID=0;
	this->arrivedSignals=new SignalList();

	for(int i=0;i<16;i++){
		this->blockedSignals[i]=0;
		this->handlers[i]=new HandlerList();
	}
	if(System::running!=0){  //nasledjuje sva podesavanja od niti koja je kreira
		this->arrivedSignals->deepCopy(System::running->arrivedSignals);
		for(int i=0;i<16;i++){
			this->handlers[i]->deepCopy(System::running->handlers[i]);
			this->blockedSignals[i]=System::running->blockedSignals[i];
		}
	}


}

PCB::~PCB() {
	lock
	delete[] stack;
	delete this->waitingForMe;
	delete this->arrivedSignals;
	for(int i=0;i<16;i++){
		delete this->handlers[i];
	}
	unlock
}

void PCB::wrapper() {

	System::running->MyThread->run();
	lock
	if(System::running->parentID!=-1){
		Thread::getThreadById(System::running->parentID)->signal(1);
	}
	if(System::globalBlockedSignals[2]==0){
		if(System::running->blockedSignals[2]==0){
			System::running->handlers[2]->handleAll();
		}
	}
	System::running->status=PCB::FINISHED;
	System::running->waitingForMe->unblockAll();
	unlock
	dispatch();

}

void PCB::allocateStack() {
lock
	this->stack = new unsigned[size];
	stack[size-1]=0x200; //bit I psw registra=1;
#ifndef BCC_BLOCK_IGNORE
	stack[size-2]=FP_SEG(&(wrapper));
	stack[size-3]=FP_OFF(&(wrapper));

	this->ss=FP_SEG(stack+size-12);
	this->sp=FP_OFF(stack+size-12);
	this->bp=FP_OFF(stack+size-12);

#endif
unlock
}

Thread* PCB::getThread(){
	return this->MyThread;
}



void PCB::checkArrivedSignals(){//provjerava listu signala i po potrebi poziva njihove handlere
	if(this->arrivedSignals->getSize()==0) return;
	else {  //nije prazna
 		SignalList* blocked=new SignalList();
 		while(this->arrivedSignals->isEmpty()==0){
 			SignalId signal=this->arrivedSignals->removeFirst();
 			if(System::globalBlockedSignals[signal]==1 || System::running->blockedSignals[signal]==1)  //blokiran
 				blocked->add(signal);
 			else {   //nije blokiran
 				if(signal!=0){
 					System::lockFlag=0;
 				this->handlers[signal]->handleAll();
 					System::lockFlag=1;
 				}
 				if(signal==0) {
 					System::lockFlag=0;
 					kill();
 					System::lockFlag=1;
 				}
 			}
 		}
 		this->arrivedSignals=blocked;
	}
	if(System::running->status == PCB::FINISHED){
			dispatch();
		}
}


