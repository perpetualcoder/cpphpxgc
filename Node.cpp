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
		next = Ptr<Node*>(*this, &n, next.index);
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
 
int main() {
 	cout<<"Simple Linked List Delete test"<<std::endl;
	testSimpleLLDeleteTest();
	cout<<"---------------------------------------"<<std::endl;
	cout<<"Circular Linked List Delete test"<<std::endl;
	testCircularLinkedList();
	cout<<"----------------------------------------"<<std::endl;
	cout<<"Circular Linked List Recovery and Delete test"<<std::endl;	
	testCircularLinkedListRecovery();
	cout<<"-----------------------------------------"<<std::endl;
	return 0;

}
