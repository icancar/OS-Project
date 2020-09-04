/*
 * HList.h
 *
 *  Created on: Aug 24, 2020
 *      Author: OS1
 */

#ifndef HLIST_H_
#define HLIST_H_
#include "thread.h"

class HandlerList{
public:
	HandlerList():head(0),tail(0), size(0) {};
	~HandlerList();
	void add(SignalHandler	sig);
	void unregisterAll();
	void swapHandlers(SignalHandler hand1, SignalHandler hand2);
	void handleAll();
	void deepCopy(HandlerList* handler) ;
private:
	struct Elem {
			SignalHandler handler;
			Elem* next;
			Elem(SignalHandler sig) :
					handler(sig), next(0) {
			}
			;
		};
		Elem* head;
		Elem* tail;
		int size;
};



#endif /* HLIST_H_ */
