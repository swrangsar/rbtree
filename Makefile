CC=gcc
#CFLAGS= -g -Wall -D__DEBUG__
CFLAGS= -g -Wall

all: rbtree

rbtree: main.c rbtree.o

rbtree.o: rbtree.h

clean:
	rm -rf rbtree.o rbtree
