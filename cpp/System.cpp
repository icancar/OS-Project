#include "System.h"
#include "SemList.h"
#include "PCB.h"
#include "HList.h"
#include "IVTEntry.h"
#include <dos.h>


#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}




PCBList* System::allPCB=new PCBList();
IdleThr* System::idleThread=0;
SemaphoreList* System::allSem=new SemaphoreList();
int System::lockFlag=1;
volatile PCB* System::running;
pInterrupt System::old=0;
ID System::startID = 1;
IVTEntry* System::table[256];
int System::globalBlockedSignals[16];
volatile int System::contextSwitchOnDemand=0;
Thread* mainT;
volatile unsigned tsp, tss, tbp;
unsigned oldTimerOFF, oldTimerSEG;
extern void tick();

void interrupt System::timer(...){
	if(!System::contextSwitchOnDemand){
		System::allSem->refresh();
		tick();
		asm int 60h;

	}
	if(System::contextSwitchOnDemand==0 && System::running->timeSlice!=0){
				System::running->timeLeft--;
			}

	if((System::running->timeLeft==0 && System::running->timeSlice!=0) || System::contextSwitchOnDemand==1){
		if(System::lockFlag==1){
			System::contextSwitchOnDemand=0;
			asm{
				mov tsp,sp
				mov tss,ss
				mov tbp,bp
			}
			System::running->sp=tsp;
			System::running->ss=tss;
			System::running->bp=tbp;

			if(System::running->status==PCB::READY  && System::running!=System::idleThread->getPCB() ){
				Scheduler::put((PCB*)System::running);
			}

			System::running= Scheduler::get();


			if(System::running==0){
				System::running = System::idleThread->getPCB();
			}
			if(System::running!=System::idleThread->getPCB()){
													System::running->MyThread->myPCB->checkArrivedSignals();
												}

			tsp = System::running->sp;
			tss=System::running->ss;
			tbp=System::running->bp;
			System::running->timeLeft=System::running->timeSlice;

			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

		}else{
			System::contextSwitchOnDemand=1;
		}
	}


}

void System::init() {
	lock

#ifndef BCC_BLOCK_IGNORE
	old = getvect(0x8);
	setvect(0x8,&timer);
	setvect(0x60, old);
#endif

//kreiranje main niti
	for(int i=0;i<15;i++){
		System::globalBlockedSignals[i]=0;
	}
	mainT=new Thread(defaultStackSize);
	System::running=mainT->myPCB;
	System::running->status=PCB::READY;
	idleThread=new IdleThr();
	System::idleThread->start();
unlock
}



void System::restore(){
#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, old);
#endif
	System::lockFlag=0;
	lock
	mainT->myPCB->status=PCB::FINISHED;
	delete idleThread;
	delete mainT;
	delete allPCB;
	delete allSem;
	unlock

}
void kill() {
	if(System::running->parentID!=0){
		Thread::getThreadById(System::running->parentID)->signal(1);
	}
	if(System::globalBlockedSignals[2]==0 && System::running->blockedSignals[2]==0)
		System::running->handlers[2]->handleAll();

	System::running->status=PCB::FINISHED;
	System::running->waitingForMe->unblockAll();

}


