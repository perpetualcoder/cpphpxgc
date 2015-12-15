#ifndef _PTRHOLDER_H
#define _PTRHOLDER_H

#include <vector>

using namespace std;

class PtrHolder {
	public:
	std::vector<void *> members;
	int index;
	PtrHolder() {
		index = 0;
	}
};

template <class T>
class Ptr {
	PtrHolder* ptr;
	int index;
	public:
	Ptr(PtrHolder& ph, T node) {
		ptr = &ph;
		index = ptr->index++;
//		initialize(ph);
		cout<<"This is exectued! "<<index<<std::endl;
//		if (ph.members.size() == index) {
			ptr->members.push_back(node);
//		} else {
//			ph.members[index] = NULL;
//		}
	}
//	void initialize(T const ph) {
//		if (index < 0) index = ph->index++;
//		ptr = ph;
//	}
	Ptr<T>& operator= (T node) {
		cout<<"removed"<<ptr->members[index]<<std::endl;
		ptr->members[index] = node;
	}

};
#endif
