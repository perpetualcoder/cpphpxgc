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
	void printVector() {
		for (int i = 0; i < members.size(); i++) { 
			std::cout<<members[i]<<',';
		}
		std::cout<<std::endl;	
	}
	void writeVector(void* ptr, int index) {
		members[index] = ptr;
	}
	void* getVector(int index) {
		return members[index];
	}
};

template <class T>
class Ptr {
	PtrHolder* ptr;
	public:
	int index;
	Ptr(PtrHolder& ph, T node) {
		ptr = &ph;
		index = ptr->index++;
//		initialize(ph);
		cout<<"This is exectued! "<<index<<std::endl;
//		if (node == NULL) 
//			return;
//		if (ph.members.size() == index) {
			ptr->members.push_back(node);
//		} else {
//			ph.members[index] = NULL;
//		}
	}
	Ptr(PtrHolder& ph, T node, int idx) {
		ptr = &ph;
		index = idx;	
		cout<<"Changing index"<<index<<std::endl;
		cout<<"Deleting link to member"<<ptr->getVector(index)<<std::endl;
		ptr->writeVector(node, index);
		cout<<"Successfully written!";

	}
//	void initialize(T const ph) {
//		if (index < 0) index = ph->index++;
//		ptr = ph;
//	}
/*	Ptr<T> & operator= (T node) {
		cout<<"removed"<<ptr->members[index]<<std::endl;
		ptr->members[index] = node;
		return Ptr<T>(node);
	}
*/
};
#endif
