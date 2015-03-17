#ifndef _RBTREE_H
#define _RBTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    BLACK,
    RED
} Color;

typedef struct rbnode {
    Color color;
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
    rbnode *sentinel;
    rbtreeClass *klass;
} rbtree;

rbtreeClass *rbtreeClassNew(rbcompf);
rbtree *rbtreeNew(rbtreeClass *);
void rbtreeDel(rbtree *);
void rbtreeInsert(rbtree *, void *);

#endif
