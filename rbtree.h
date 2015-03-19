#ifndef _RBTREE_H
#define _RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

#define BLACK   (unsigned char)0
#define RED     (unsigned char)1

typedef struct rbnode {
    unsigned char color;
    void *data;
    struct rbnode *left;
    struct rbnode *right;
    struct rbnode *parent;
} rbnode;

typedef int (*rbcmpf) (void *, void *);
typedef void (*rbdstf) (void *);

typedef struct rbtreeClass {
    rbcmpf cmp;
    rbdstf dst;
} rbtreeClass;

typedef struct rbtree {
    rbnode *root;
    rbtreeClass *klass;
} rbtree;

rbtreeClass *rbtreeClassNew(const rbcmpf, const rbdstf);
rbtree *rbtreeNew(rbtreeClass *);
void rbtreeDel(rbtree *);
void rbtreeInsert(rbtree *, const void *);

#endif
