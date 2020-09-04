#include "PCBList.h"
#include <stdio.h>

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

void PCBList::add(PCB* p){

	tail=(!head? head: tail->next)=new Elem(p);
	size++;

}

PCB* PCBList::removeFirst(){

	Elem* tmpElem=head;
		PCB* tmpPCB=0;
		lock
		if(this->size==0 || tmpElem==0){
			unlock
			return 0;
		}
		tmpPCB=tmpElem->info;
		head=head->next;
		tmpElem->next=0;
		delete tmpElem;
		size--;
		if(head==0) tail=0;
		unlock
		return tmpPCB;
}

PCBList::~PCBList(){
		lock
		while(head){
			Elem *old = head;
			head = head->next;
			old->next = 0;
			size--;
			delete old;
		}
		size = 0;
		head = 0;
		tail = 0;
		unlock
}
Thread* PCBList::findID(ID id){
	Elem* temp = head;
		lock
		if(head == 0){
			unlock
			return 0;
		}
		while(temp->info->ID != id){
			temp = temp->next;
		}
		unlock
		return temp->info->MyThread;
}


void PCBList::removeID(ID id){
	Elem* temp = head;
		Elem* prev = 0;
		lock
		while(temp){
			if(temp->info->ID == id){
				Elem* old = temp;
				if(prev == 0)
					head = temp->next;
				else
					prev->next = temp->next;
				if(tail == old)
					tail = prev;
				size--;
				temp = temp->next;
				old->next = 0;
				delete old;
			}
			else{
				prev = temp;
				temp = temp->next;
			}
		}
		unlock
}

void PCBList::unblockAll(){//provjeriti da li radi, mozda pravi probleme
		Elem* tmpElem;
		PCB* tmpPCB;
		lock
		while(size!=0){
			tmpElem=this->head;
			tmpPCB=tmpElem->info;
			tmpPCB->status=PCB::READY;
			if(tmpPCB->checkTime){
			tmpPCB->checkTime=0;	}
			Scheduler::put((PCB*)tmpPCB);
			size--;
			head=head->next;
			tmpElem->next=0;
			delete tmpElem;
		}

		head=tail=0;
		size=0;
		unlock
}

int PCBList::checkTimeBlocked(){
	int ret=0;
	Elem* cur=head;
	while(cur){
		if(cur->info->checkTime==1){
			cur->info->timeBlocked--;
			if(cur->info->timeBlocked==0){
				Elem* old=cur;
				cur=cur->next;
				this->unblockID(old->info->ID);
				ret++;
			}
			else cur=cur->next;
		}else cur=cur->next;
	}
	return ret;
}

void PCBList::unblockID(ID id){

	Elem* prev=0;
		Elem* cur=head;
		while(cur){
			if(cur->info->ID==id){//treba izbaciti
			Elem* old=cur;
			if(head==old){ //prvi element
				head=cur->next;
			}

			else if(tail==old){ //zadnji element
				tail=prev;
				tail->next=0;
			}
			else prev->next=cur->next;


			size--;
			cur=cur->next;
			old->info->status=PCB::READY;
		if(old->info->checkTime){
				old->info->checkTime=0;
							}
			Scheduler::put(old->info);
			old->next=0;
			delete old;
			}
			else {
				prev=cur;
				cur=cur->next;
			}
		}
	}

