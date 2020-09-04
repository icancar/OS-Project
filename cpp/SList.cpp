#include "SList.h"

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

SignalList::~SignalList(){
	lock
			while(head){
				Elem *old = head;
				head = head->next;
				old->next = 0;
				delete old;
				size--;
			}
			size = 0;
			head = 0;
			tail = 0;
			unlock
}

void SignalList::add(SignalId id){
	tail=(!head? head: tail->next)=new Elem(id);
		size++;
}

SignalId SignalList::removeFirst(){
	Elem* tmpElem=head;
	SignalId sign=0;
			lock
			if(this->size==0 || tmpElem==0){
				unlock
				return 0;
			}
			sign=tmpElem->signal;
			head=head->next;
			tmpElem->next=0;
			delete tmpElem;
			size--;
			if(head==0) tail=0;
			unlock
			return sign;
}

int SignalList::isEmpty(){
	if(this->size==0|| head==0 )return 1;
	else return 0;
}


void SignalList::deepCopy(SignalList* signals){
	Elem* cur=signals->head;
	while(cur!=0){
		this->add(cur->signal);
		cur=cur->next;
	}
}

int SignalList::getSize(){
	return this->size;
}

