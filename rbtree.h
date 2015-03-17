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


typedef struct rbtree {
    rbnode *root;
    rbnode *sentinel;
} rbtree;

rbtree *rbtreeNew(void);
void rbtreeDel(rbtree *);

#endif
