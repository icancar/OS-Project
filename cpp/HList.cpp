#include "HList.h"
#include "System.h"

HandlerList::~HandlerList(){
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

void HandlerList::add(SignalHandler s){
	tail=(!head? head: tail->next)=new Elem(s);
		size++;
}

void HandlerList::unregisterAll(){
	Elem* tmpElem;
			lock
			while(size!=0){
				tmpElem=this->head;
				size--;
				head=head->next;
				tmpElem->next=0;
				delete tmpElem;
			}

			head=tail=0;
			size=0;
			unlock
}

void HandlerList::swapHandlers(SignalHandler hand1, SignalHandler hand2){
	lock
	Elem* cur=this->head;
	Elem* handler1=0;
	Elem* handler2=0;
	while (cur){
		if(cur->handler==hand1)
			handler1=cur;
		else if (cur->handler==hand2)
			handler2=cur;
		cur=cur->next;
	}
	if(handler1==0 ||handler2==0 ){
		unlock
		return;
	}
	else {
		handler1->handler=hand2;
		handler2->handler=hand1;
	}
	unlock
}

void HandlerList::handleAll(){
	System::lockFlag=0;
	Elem* cur=this->head;
	while(cur){
		(*cur->handler)();
		cur=cur->next;
	}
	System::lockFlag=1;
}

void HandlerList::deepCopy(HandlerList* h){
	Elem* tmp=h->head;
	while(tmp!=0){
		this->add(tmp->handler);
		tmp=tmp->next;
	}
}
