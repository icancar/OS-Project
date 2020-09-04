/*
 * Semaphor.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
#include "KSem.h"
#include "System.h"
#include "SemList.h"

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

Semaphore::Semaphore(int init){
	lock
	myImpl=new KernelSem(init);
	System::allSem->add(myImpl);
	unlock
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
	lock
	int ret=myImpl->wait(maxTimeToWait);
	unlock
	return ret;

}

int Semaphore::signal(int n){
	lock
	int ret=myImpl->signal(n);
	unlock
	return ret;

}

int Semaphore::val()const {
	return myImpl->value();
}

