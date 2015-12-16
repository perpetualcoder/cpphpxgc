#ifndef _PTRHOLDER_H
#define _PTRHOLDER_H

#include <vector>

using namespace std;

class PtrHolder {
	public:
	// Vector of outgoing members.
	std::vector<void *> members;
	// Which vector of outgoing members. (This is part of the GC algorithm data).
	std::vector<int> whichs;
	// Phantom status of the object.
	bool phantom;
	// Scheduled to be deleted. For now, it is used here due to recursion. But in future
	// this will not be here.
	bool schd;
	// Index of the next outgoing link to be inserted in the members vector.
	int index;
	// which bit of the object. (GC based part).
	int which;
	// Three reference counts. First two are Strong and Weak Reference count. The which field points to
	// Strong reference count and 1 - which points to weak reference count. Always last reference count is 
	// phantom reference count. All three reference counts are part of the GC algorithm.
	int rc[3];

	PtrHolder() {
		index = 0;
		which = 0;
		rc[0] = rc[1] = rc[2] = 0;
		phantom = false;
		schd = false;
	}

	~PtrHolder() {
		std::cout<<"Object is being destroyed"<<std::endl;
	}

	void printNode() {
		cout<<"Object"<<std::endl;
		printVector();
		printRC();

	}
	
	// During Edge/Link creation, GC algorithm requires to look at the outgoing status of the object. So this
	// method returns if there are any valid outgoing edges. This method is linear in terms of the possible outgoing
	// edges. This can be made constant through some optimization.
	bool hasOutgoing() {
		for (unsigned int i = 0; i < members.size(); i++) {
			if (members[i] != NULL) return true;
		}
		return false;
	}
	
	
	bool isDeletable() {
		if (rc[0] == rc[1] && rc[1] == rc[2] && rc[2] == 0)
			return true;
		return false;
	}

	void simpleDecrementRC(int wbit) {
		if (which == wbit) 
                         rc[which]--;
                else if(wbit != 2 && which != wbit)
                         rc[1-which]--;
		else
			rc[wbit]--;
	}

	void decrementRC(int wbit) {
		if (which == wbit) 
			rc[which]--;
		else if(wbit != 2 && which != wbit) 
			rc[1-which]--;
		else 
			rc[wbit]--;
		if (isDeletable()) {
			for(unsigned int i=0; i< members.size();i++) {
				void *m = members[i];
				if (m != 0) {
				PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
				p->decrementRC(whichs[i]);			
				}
			}
			if (!schd) {
				cout<<"Deleting this object"<<std::endl;
				delete this;
			}
		} // The next section is where phantomization starts because there are nodes weak supported.
		else if(rc[which] == 0 && rc[2] == 0 && rc[1-which] != 0) {
			cout<<"Phantomization starts"<<std::endl;
			std::vector<PtrHolder *> process_list;
			collapseStart(process_list);
			cout<<"List size"<<process_list.size()<<std::endl;
			bool recovery = false;
			for (unsigned int i = 0 ; i < process_list.size(); i++ ) {
				PtrHolder *p = process_list[i];
				if (p->rc[p->which] > 0) recovery = true;

			}
			if (!recovery) {
			schd = true;
			for(unsigned int i=0; i< members.size();i++) {
                                 void *m = members[i];
                                 if (m != 0) {
                                 PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
                                 p->decrementRC(whichs[i]);
				 members[i] = NULL;
                                 }
                         }
			 delete this;
			}
			else {
				cout<<"Recovery procedure initiated."<<std::endl;
				for(unsigned int i = 0; i < process_list.size(); i++) {
					PtrHolder *p = process_list[i];
					if (p->rc[p->which] > 0 ) {
						p->recover();
					}

				}
				if (!phantom && rc[which] > 0) {
					cout<<"Collapse Recovery is successful!"<<std::endl;
					
				}

			}
		}
	}

	void recover() {
		std::cout<<"Recover called on "<<this<<std::endl;
		if ( phantom ) {
			phantom = false;
			for(unsigned int i = 0; i < members.size(); i++) {
				void *m = members[i];
				if (m != 0) {
					PtrHolder *p  = reinterpret_cast<PtrHolder*>(m);
					p->simpleDecrementRC(whichs[i]);
					if (p->rc[p->which] > 0 ) {
						p->simpleIncrementRC(1 - p->which);
						whichs[i] = 1 - p->which;
					}
					else {
						p->simpleIncrementRC(p->which);
						whichs[i] = p->which;
					}
					p->recover();
				}
			}

		}
	}

	void simpleIncrementRC(int wbit) {
		rc[wbit]++;
	}

	void incrementRC(int wbit, std::vector<PtrHolder*>& list) {
		if (wbit > 1) {
			rc[wbit]++;
			if (phantom) return;
			phantom = true;
			list.push_back(this);
			if (rc[which] > 0) return;
			else if (rc[1-which] > 0) {
				collapseStart(list);

			}	
			else if (rc[which] == 0 && rc[1-which] == 0 && rc[2] > 0) {
				phantomize(list);
			}
		}

	}

	void phantomize(std::vector<PtrHolder*>& list) {
		cout<<"Phantomization is going on"<<std::endl;
		for (unsigned int i = 0; i < members.size(); i++)  {
                         void *m = members[i];
                         if ( m != 0) {
                                 PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
                                 p->simpleDecrementRC(whichs[i]);
                                 whichs[i] = 2;
                                 p->incrementRC(whichs[i], list);
                         }
 
                 }

	} 

	void collapseStart(std::vector<PtrHolder*>& list) {
		cout<<"Collapse started"<<this<<std::endl;
		which = 1 - which;
		phantom = true;
		list.push_back(this);
		for (unsigned int i = 0; i < members.size(); i++)  {
			void *m = members[i];
			if ( m != 0) {
				PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
				p->simpleDecrementRC(whichs[i]);
				whichs[i] = 2;
				p->incrementRC(whichs[i], list);
			}

		}

	}

	void printRC() {
		cout<<"which:"<<which<<":"<<rc[0]<<","<<rc[1]<<","<<rc[2]<<std::endl;
	}

	void printVector() {
		for (unsigned int i = 0; i < members.size(); i++) { 
			std::cout<<members[i]<<',';
		}
		std::cout<<std::endl;	
	}

	void writeVector(void* ptr, int index, int whs) {
		members[index] = ptr;
		whichs[index] = whs;
	}

	void* getVector(int index) {
		if (index < (int) members.size())
			return members[index];
		else
			return NULL;
	}
};

// This class will behave like any RAII pointer. We call it SWP (Strong Weak Phantom) Pointer. The SWP will take care of the memory deletion safely. So user do not 
// have to worry about it anymore!!
template<class T>
class SWPPtr {
	public:
	T p;
	SWPPtr(T ptr) {
		p = ptr;
		p->rc[p->which]++;
	}
	~SWPPtr() {
		cout<<"Destructor called!"<<std::endl;
		p->decrementRC(p->which);
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
		ptr->members.push_back(node);
		ptr->whichs.push_back(0);
		PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
		if (pht != 0) {
			which = pht->hasOutgoing() ? 1- pht->which : pht->which;
			ptr->writeVector(node, index, which);
			pht->rc[which]++;	
		}
		else {
			which = -1;
		}
	}

	Ptr(PtrHolder& ph, T node, int idx) {
		ptr = &ph;
		index = idx;	
		T prevnode = (T)ptr->getVector(index);
		PtrHolder *prevph = dynamic_cast<PtrHolder*>(prevnode);
		if (prevph != 0 ) {
			prevph->rc[which]--;
		}
		PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
                if (pht != 0) {
			which = pht->hasOutgoing() ? 1- pht->which : pht->which;
			ptr->writeVector(node, index, which);
			pht->rc[which]++;
		}
	}

};
#endif
