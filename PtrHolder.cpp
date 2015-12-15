#ifndef _PTRHOLDER_CPP
#define _PTRHOLDER_CPP
#include <vector>

using namespace std;

class PtrHolder {
	public:
	std::vector<void*> members;

	void visit(void (*func)(void*)) {
		std::vector<void*>::iterator iter;
		for(iter = members.begin(); iter != members.end(); ++iter) {
//			PtrHolder *ph = dynamic_cast<PtrHolder*>(*iter);
//			if (ph!=0) {
//				ph->visit(func);
//				(*func)(*iter);
//			}
		}
	}

};

template <class T>
class Ptr {
	T ptr;
	Ptr(T* const ph) {
		ph.members.push_back(this);
	}

};
#endif
