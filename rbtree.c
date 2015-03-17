#include "rbtree.h"
#include "debug.h"


static rbnode *rbnodeNew(void *);
static int isLeaf(rbnode *);
static void rbnodeDel(rbnode *);
static void rbtreeDelRec(rbnode *);

static rbnode *rbnodeNew(void *data)
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

static int isLeaf(rbnode *n)
{
    return (n->data)?0:1;
}

static void rbnodeDel(rbnode *n)
{
    if (!n) return;
    if (n->data) free(n->data);
    n->left = n->right = n->right = NULL;
    free(n);
    n = NULL;
}

rbtree *rbtreeNew(void)
{
    rbtree *rt = malloc(sizeof(rbtree));
    memcheck(rt);
    rt->root = NULL;
    rt->sentinel = rbnodeNew(NULL);
    return rt;
}

void rbtreeDel(rbtree *t)
{
    if (t->root) rbtreeDelRec(t->root);
    rbnodeDel(t->sentinel);
    free(t);
    t = NULL;
}

static void rbtreeDelRec(rbnode *n)
{
    if (n->left) rbtreeDelRec(n->left);
    if (n->right) rbtreeDelRec(n->right);
    rbnodeDel(n);
}
