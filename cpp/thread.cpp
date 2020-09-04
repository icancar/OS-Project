#include "thread.h"
#include "PCB.h"
#include "System.h"
#include "HList.h"
#include "SList.h"

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	System::allPCB->add(this->myPCB);
	unlock
}

ID Thread::getRunningId(){
	return System::running->ID;
}

ID Thread::getId(){
	return myPCB->ID;
}

Thread* Thread::getThreadById(ID id) {
	Thread* threadToBeFound = 0;
	if(id<0) return 0;
	lock
	threadToBeFound = System::allPCB->findID(id);
	unlock
	return threadToBeFound;
}

void Thread::start(){
	lock
		if(myPCB==0 || myPCB->ID<0){
			unlock
			return;
		}
		if(myPCB->status==PCB::CREATED && myPCB!=System::idleThread->getPCB()){
			Scheduler::put(myPCB);
			myPCB->status=PCB::READY;
		}
		if(myPCB==System::idleThread->getPCB()){
			myPCB->status=PCB::READY;
		}
		unlock
}

void Thread::waitToComplete(){

	 lock
	 if(System::running==0){
		 unlock
		 return;
	 }
	 if(System::running->status==PCB::FINISHED || System::running->ID<0){
		 unlock
		 return;
	 }
	 if((System::running!=this->myPCB) && (this->myPCB->status!=PCB::FINISHED)&& (this->myPCB!=System::idleThread->getPCB())){  //ne poziva za samu sebe
			 	 System::running->status=PCB::BLOCKED;
			 	 this->myPCB->waitingForMe->add((PCB*)System::running);
			 	 unlock
			 	 dispatch();
			 	 return;

		 }
		 unlock
}


Thread::~Thread(){
	lock
	waitToComplete();
	System::allPCB->removeID(myPCB->ID);
	myPCB->waitingForMe->unblockAll();
	delete myPCB;
	unlock
}


void dispatch(){
	lock
	System::contextSwitchOnDemand = 1;
	System::timer();
	unlock
}

void Thread::signal(SignalId signal){
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	this->myPCB->arrivedSignals->add(signal);
	System::lockFlag=1;
};

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	this->myPCB->handlers[signal]->add(handler);
	System::lockFlag=1;
};

void Thread::unregisterAllHandlers(SignalId id){
	if(id<0 || id>15) return;
	System::lockFlag=0;
	this->myPCB->handlers[id]->unregisterAll();
	System::lockFlag=1;
};
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(id<0 || id>15 || hand1==0 || hand2==0) return;
	System::lockFlag=0;
	this->myPCB->handlers[id]->swapHandlers(hand1, hand2);
	System::lockFlag=1;
};

void Thread::blockSignal(SignalId signal) {
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	this->myPCB->blockedSignals[signal]=1;
	System::lockFlag=1;
};
void Thread::blockSignalGlobally(SignalId signal){
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	System::globalBlockedSignals[signal]=1;
	System::lockFlag=1;
}
void Thread::unblockSignal(SignalId signal) {
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	if(	this->myPCB->blockedSignals[signal]==1)
		this->myPCB->blockedSignals[signal]=0;
	System::lockFlag=1;
};
void Thread::unblockSignalGlobally(SignalId signal) {
	if(signal<0 || signal>15) return;
	System::lockFlag=0;
	if(System::globalBlockedSignals[signal]==1)
		System::globalBlockedSignals[signal]=0;
	System::lockFlag=1;
};


