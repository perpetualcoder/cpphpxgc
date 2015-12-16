#ifndef _PTRHOLDER_H
#define _PTRHOLDER_H

#include <vector>

using namespace std;

class PtrHolder {
	public:
	std::vector<void *> members;
	int index;
	int which;
	int rc[3];
	PtrHolder() {
		index = 0;
		which = 0;
		rc[0] = rc[1] = rc[2] = 0;
	}
	~PtrHolder() {
		std::cout<<"Object is being destroyed"<<std::endl;
	}
	void printNode() {
		cout<<"Object"<<std::endl;
		printVector();
		printRC();

	}
	bool hasOutgoing() {
		for (int i = 0; i < members.size(); i++) {
			if (members[i] != NULL) return true;
		}
		return false;

	}
	bool isDeletable() {
		if (rc[0] == rc[1] && rc[1] == rc[2] && rc[2] == 0)
			return true;
		return false;
	}
	void printRC() {
		cout<<"which:"<<which<<":"<<rc[0]<<","<<rc[1]<<","<<rc[2]<<std::endl;
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
		cout<<"index"<<index<<"size"<<members.size()<<std::endl;
		if (index < members.size())
			return members[index];
		else
			return NULL;
	}
};

template<class T>
class SWPPtr {
	public:
	T p;
	SWPPtr(T ptr) {
		p = ptr;
		p->rc[p->which]++;
	}
	~SWPPtr() {
		cout<<"Destructor called!";
		p->rc[p->which]--;
		if (p->isDeletable())
			delete p;
	}


};

template <class T>
class Ptr {
	PtrHolder* ptr;
	int which;
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
			PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
			if (pht != 0) {
//				ptr->members.push_back(node);
				which = pht->hasOutgoing() ? 1- pht->which : pht->which;
				pht->rc[which]++;	
			}
			else {
				which = -1;
			}
//		} else {
//			ph.members[index] = NULL;
//		}
	}
	Ptr(PtrHolder& ph, T node, int idx) {
		ptr = &ph;
		index = idx;	
		cout<<"Changing index"<<index<<std::endl;
		cout<<"Deleting link to member"<<ptr->getVector(index)<<std::endl;
		
		T prevnode = (T)ptr->getVector(index);
		cout<<"Printed this!";
			PtrHolder *prevph = dynamic_cast<PtrHolder*>(prevnode);
			if (prevph != 0 ) {
				cout<<" Link is "<<which<<prevnode->which;
				prevph->rc[which]--;
			}
		ptr->writeVector(node, index);
		PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
                if (pht != 0) {
			which = pht->hasOutgoing() ? 1- pht->which : pht->which;
			pht->rc[which]++;
		}
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
