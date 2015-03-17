CC=gcc

all: rbtree

rbtree: main.c rbtree.o

rbtree.o: rbtree.h

clean:
	rm -rf rbtree.o rbtree
