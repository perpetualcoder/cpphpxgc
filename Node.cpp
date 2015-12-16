#include <iostream>
#include "PtrHolder.h"

class Node : public PtrHolder {
	int data;
	Ptr<Node*> next;
	Ptr<Node*> prev;

	public: 
	Node () : PtrHolder(), next(*this, NULL), prev(*this , NULL) {
		data = 0;
	}
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
//	Node r(5);
//	Node t;
//	t = n;
	Node *n = new Node(4);
	Node *p = new Node(5);
	SWPPtr<Node*> head(n);
	n->setNext(*p);
	p->setNext(*n);
	//	Node n2(56);
//	r.setNext(*n);
//	r.setNext(n2);
	cout<<"The size of n vector is "<<n->size()<<std::endl;
	n->printVector();
//	n->setNext(r);
	p->printVector();
//	n->setNext(n2);
//	n->printVector();
//	n2.setNext(*n);	
//	cout<<"The size of r vector is "<<r.size()<<std::endl;
//	r.printVector();

	cout<<"---------------------"<<std::endl;
//	r.printNode();
	n->printNode();
	p->printNode();
//	n2.printNode();
	return 0;
}
