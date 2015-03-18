#ifndef _RBTREE_H
#define _RBTREE_H

#include <stdio.h>
#include <stdlib.h>

#define BLACK   (unsigned char)0
#define RED     (unsigned char)1

typedef struct rbnode {
    unsigned char color;
    void *data;
    struct rbnode *left;
    struct rbnode *right;
    struct rbnode *parent;
} rbnode;

typedef int (*rbcompf) (void *, void *);

typedef struct rbtreeClass {
    rbcompf compare;
} rbtreeClass;

typedef struct rbtree {
    rbnode *root;
    rbtreeClass *klass;
} rbtree;

rbtreeClass *rbtreeClassNew(rbcompf);
rbtree *rbtreeNew(rbtreeClass *);
void rbtreeDel(rbtree *);
void rbtreeInsert(rbtree *, void *);

#endif
