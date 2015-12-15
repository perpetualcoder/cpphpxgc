#include <iostream>
#include "PtrHolder.h"

class Node : public PtrHolder {
	int data;
	Ptr<Node*> next;
	Ptr<Node*> prev;

	public:
	Node(int dat) : PtrHolder(), next(*this, NULL), prev(*this, NULL) { 
		data = dat;
	}
	void setNext(Node& n) {
		next = &n;
	}
//	void setPrev(Node& p) {
//		prev = p;
//	}
	int size () {
		return members.size();
	}

};

int main() {
	using std::cout;
	cout<<"Hello World!"<<std::endl;
	Node r(5);
	Node n(4);
	Node n2(56);
	r.setNext(n);
	r.setNext(n2);
	cout<<"The size of n vector is "<<n.size()<<std::endl;
	cout<<"The size of r vector is "<<r.size()<<std::endl;
	return 0;
}
