#include "rbtree.h"
#include "debug.h"


//static rbnode *rbnodeNew(void *);
//static int isLeaf(rbnode *);


rbnode *rbnodeNew(void *data)
{
    rbnode *n = malloc(sizeof(rbnode));
    memcheck(n);
    n->data = (void *)data;
    n->color = RED;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}

int isLeaf(rbnode *n)
{
    return (n->data)?0:1;
}

void rbnodeDel(rbnode *n)
{
    if (!n) return;
    if (n->data) free(n->data);
    n->left = n->right = n->right = NULL;
    free(n);
}
