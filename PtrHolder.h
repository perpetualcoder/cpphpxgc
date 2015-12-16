#ifndef _PTRHOLDER_H
#define _PTRHOLDER_H

#include <vector>

using namespace std;

class PtrHolder {
	public:
	std::vector<void *> members;
	std::vector<int> whichs;
	bool phantom;
	bool schd;
	int index;
	int which;
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
	void simpleDecrementRC(int wbit) {
	if (which == wbit) 
                         rc[which]--;
                 else if(which != wbit)
                         rc[1-which]--;
	

	}
	void decrementRC(int wbit) {
		if (which == wbit) 
			rc[which]--;
		else if(wbit != 2 && which != wbit) 
			rc[1-which]--;
		else 
			rc[wbit]--;
		if (isDeletable()) {
			for(int i=0; i< members.size();i++) {
				void *m = members[i];
				if (m != 0) {
					cout<<"Printing this"<<this<<std::endl;
				PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
				p->decrementRC(whichs[i]);			
				}
			}
			cout<<"schd"<<schd<<" for "<< this<<std::endl;
			if (!schd) {
				cout<<"Deleting this object"<<std::endl;
				delete this;
			}
		}
		else if(rc[which] == 0 && rc[2] == 0 && rc[1-which] != 0) {
			cout<<"Phantomization starts"<<std::endl;
			std::vector<PtrHolder *> process_list;
			collapseStart(process_list);
			cout<<"List size"<<process_list.size()<<std::endl;
			bool recovery = false;
			for (int i = 0 ; i < process_list.size(); i++ ) {
				PtrHolder *p = process_list[i];
				if (p->rc[p->which] > 0) recovery = true;

			}
			if (!recovery) {
			schd = true;
			for(int i=0; i< members.size();i++) {
                                 void *m = members[i];
                                 if (m != 0) {
                                         cout<<"pppPrinting this"<<this<<std::endl;
                                 PtrHolder *p = reinterpret_cast<PtrHolder*>(m);
                                 p->decrementRC(whichs[i]);
				 members[i] = NULL;
                                 }
                         }
			 delete this;

			}
		}
	}
	void incrementRC(int wbit, std::vector<PtrHolder*>& list) {
		cout<<"Increment RC "<<wbit<<" is called"<<"phantom"<<phantom<<std::endl;
		if (wbit > 1) {
			rc[wbit]++;
			if (phantom) return;
			cout<<"Phantom bit set"<<std::endl;
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
	for (int i = 0; i < members.size(); i++)  {
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
		for (int i = 0; i < members.size(); i++)  {
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
		for (int i = 0; i < members.size(); i++) { 
			std::cout<<members[i]<<',';
		}
		std::cout<<std::endl;	
	}
	void writeVector(void* ptr, int index, int whs) {
		members[index] = ptr;
		whichs[index] = whs;
	}
	void* getVector(int index) {
		///cout<<"index"<<index<<"size"<<members.size()<<std::endl;
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
//		initialize(ph);
//		cout<<"This is exectued! "<<index<<std::endl;
//		if (node == NULL) 
//			return;
//		if (ph.members.size() == index) {
			ptr->members.push_back(node);
			ptr->whichs.push_back(0);
			PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
			if (pht != 0) {
//				ptr->members.push_back(node);
				which = pht->hasOutgoing() ? 1- pht->which : pht->which;
				ptr->writeVector(node, index, which);
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
//		cout<<"Changing index"<<index<<std::endl;
//		cout<<"Deleting link to member"<<ptr->getVector(index)<<std::endl;
		
		T prevnode = (T)ptr->getVector(index);
//		cout<<"Printed this!";
			PtrHolder *prevph = dynamic_cast<PtrHolder*>(prevnode);
			if (prevph != 0 ) {
//				cout<<" Link is "<<which<<prevnode->which;
				prevph->rc[which]--;
			}
//		ptr->writeVector(node, index);
		PtrHolder *pht = dynamic_cast<PtrHolder*>(node);
                if (pht != 0) {
			which = pht->hasOutgoing() ? 1- pht->which : pht->which;
			ptr->writeVector(node, index, which);
			pht->rc[which]++;
		}
		//cout<<"Successfully written!";
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
