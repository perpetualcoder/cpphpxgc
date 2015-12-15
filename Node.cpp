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
		std::cout<<"Node before index is "<<next.index<<std::endl;
		next = Ptr<Node*>(*this, &n, next.index);
		std::cout<<"Node next index is"<< next.index<<std::endl;
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
//	r.setNext(n2);
	cout<<"The size of n vector is "<<n.size()<<std::endl;
	n.printVector();
	n.setNext(r);
	n.printVector();
	n.setNext(n2);
	n.printVector();
	cout<<"The size of r vector is "<<r.size()<<std::endl;
	r.printVector();
	return 0;
}
