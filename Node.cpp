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
//		std::cout<<"Node before index is "<<next.index<<std::endl;
		next = Ptr<Node*>(*this, &n, next.index);
//		std::cout<<"Node next index is"<< next.index<<std::endl;
	}
//	void setPrev(Node& p) {
//		prev = p;
//	}
	int size () {
		return members.size();
	}

};

Node* simpleLinkedList() {
	Node *a = new Node(1);
	Node *b = new Node(2);
	Node *c = new Node(3);
	a->setNext(*b);
	b->setNext(*c);
	return a;
}
Node* circularLinkedList() {
	Node *a = new Node(1);
	Node *b = new Node(2);
	Node *c = new Node(3);
	cout<<a<<","<<b<<","<<c<<endl;
	a->setNext(*b);
	b->setNext(*c);
	c->setNext(*a);
	return a;
}
void performSimpleLinkedListDeleteTest(Node* ptr) {
	SWPPtr<Node*> head(ptr);
}
void performCircularLinkedListDeleteTest(Node *ptr) {
	SWPPtr<Node*> head(ptr);
}
void testSimpleLLDeleteTest() {
	Node *ll = simpleLinkedList();
	performSimpleLinkedListDeleteTest(ll);
}
void testCircularLinkedList() {
	Node *cl = circularLinkedList();
	performCircularLinkedListDeleteTest(cl);
}
void performRecoveryCircularLinkedList(Node *ptr) {
	SWPPtr<Node*> head(ptr);
}
void testCircularLinkedListRecovery() {
	Node *a = new Node(1);
	Node *b = new Node(2);
	Node *c = new Node(3);
	Node *d = new Node(4);
	cout<<a<<","<<b<<","<<c<<","<<d<<std::endl;
	a->setNext(*b);
	b->setNext(*c);
	c->setNext(*d);
	d->setNext(*a);
	Node *e = new Node(5);
	SWPPtr<Node*> support(e);
	e->setNext(*c);
	performRecoveryCircularLinkedList(a); 
}
 
int mains() {
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
int main() {
// 	cout<<"Simple Linked List test"<<std::endl;
//	testSimpleLLDeleteTest();
	cout<<"Circular Linked List test"<<std::endl;
//	testCircularLinkedList();
	testCircularLinkedListRecovery();
	return 0;

}
int smain() {
	mains();
	cout<<"After mains----------------------"<<std::endl;
//	n->printNode();
//	p->printNode();
	return 0;
}
