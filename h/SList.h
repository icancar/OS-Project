
#ifndef SLIST_H_
#define SLIST_H_

typedef unsigned SignalId;

class SignalList {
public:
	SignalList():head(0),tail(0),size(0){};
	~SignalList();
	SignalId removeFirst();
	void add(SignalId id);
	int isEmpty();

	int getSize();
	void deepCopy (SignalList* signals);
	struct Elem {
				SignalId signal;
				Elem* next;
				Elem(SignalId sig) :
						signal(sig), next(0) {
				}
				;
			};
			Elem* head;
			Elem* tail;
			int size;
};



#endif /* SLIST_H_ */
