#include "IdleThr.h"
#include "PCB.h"
#include <iostream.h>


void IdleThr::run(){
	while(1){
		int dummy=0;
		if(dummy==0) {dummy=1;}
		else dummy=0;
	}
}

ID IdleThr::getID(){
	return this->getID();
}

PCB* IdleThr::getPCB() {
	return this->myPCB;
}

void IdleThr::kreni() {
	this->myPCB->status=PCB::READY;
}
