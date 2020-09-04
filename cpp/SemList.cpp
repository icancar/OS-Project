
#include "SemList.h"

SemaphoreList::~SemaphoreList(){

	while(head){
				Elem *old = head;
				head = head->next;
				old->next = 0;
				delete old;
				numofSem--;
			}
			numofSem = 0;
			head = 0;
			tail = 0;
}

void SemaphoreList::add(KernelSem* s){
	tail=(!head? head: tail->next)=new Elem(s);
		numofSem++;
}

void SemaphoreList::refresh(){ //update time on waiting threads
	if(this->numofSem>0){
		Elem* cur=head;
	while(cur){
		if(cur->sem->blocked->size){
			int inc=cur->sem->blocked->checkTimeBlocked();
			cur->sem->val+=inc;
		}
		cur=cur->next;
	}
	}
}
