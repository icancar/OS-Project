
#include "Event.h"
#include "KEvent.h"
#include "System.h"


Event::Event(IVTNo ivtno){
	lock
	this->myImpl=new KernelEv(ivtno);
	unlock
}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
	lock
	myImpl->wait();
	unlock
}

void Event::signal() {
	lock
	myImpl->signal();
	unlock
}
