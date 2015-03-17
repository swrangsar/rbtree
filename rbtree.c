#include "rbtree.h"
#include "debug.h"


static rbnode *rbnodeNew(void *);
static int isLeaf(rbnode *);
static void rbnodeDel(rbnode *);
static void _rbtreeDel(rbnode *);
static void _rbtreeInsert(rbtree *, rbnode *, rbnode *);

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

rbtreeClass *rbtreeClassNew(rbcompf fa)
{
    rbtreeClass *k = malloc(sizeof(rbtreeClass));
    memcheck(k);
    k->compare = fa;
    return k;
}

rbtree *rbtreeNew(rbtreeClass *k)
{
    rbtree *rt = malloc(sizeof(rbtree));
    memcheck(rt);
    rt->root = NULL;
    rt->sentinel = rbnodeNew(NULL);
    rt->klass = k;
    return rt;
}

void rbtreeDel(rbtree *t)
{
    if (t->root) _rbtreeDel(t->root);
    rbnodeDel(t->sentinel);
    free(t->klass);
    free(t);
    t = NULL;
}

static void _rbtreeDel(rbnode *n)
{
    if (n->left) _rbtreeDel(n->left);
    if (n->right) _rbtreeDel(n->right);
    rbnodeDel(n);
}

void rbtreeInsert(rbtree *t, void *data)
{
    rbnode *n = rbnodeNew((void *)data);
    if (!t || !data) {
        printf("rbtreeInsert: either the tree or the data is null!\n");
        return;
    }
    if (!t->root) {
        t->root = n;
    } else {
        _rbtreeInsert(t, t->root, n);
    }
}

static void _rbtreeInsert(rbtree *t, rbnode *r, rbnode *n)
{
    int res = t->klass->compare(n->data, r->data);
    if (res < 0) {
        if (r->left) {
            _rbtreeInsert(t, r->left, n);
        } else {
            r->left = n;
            n->parent = r;
        }
    } else if (res > 0) {
        if (r->right) {
            _rbtreeInsert(t, r->right, n);
        } else {
            r->right = n;
            n->parent = r;
        }
    } else {
        printf("_rbtreeInsert: rbnode already exist!\n");
        return;
    }
}
