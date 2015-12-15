# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all:node

node: PtrHolder.o Node.o
	$(CC) PtrHolder.o Node.o -o node

PtrHolder.o: PtrHolder.cpp
	$(CC) $(CFLAGS) PtrHolder.cpp

Node.o:
	$(CC) $(CFLAGS) Node.cpp

clean:
	rm *o node
