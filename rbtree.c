#include "rbtree.h"
#include "debug.h"


static rbnode *rbnodeNew(const void *);
static int isLeaf(const rbtree *, const rbnode *);
static void rbnodeDel(rbnode *);
static void _rbtreeDel(rbnode *);
static void _rbtreeInsert(const rbtree *, rbnode *, rbnode *);
static rbnode *grandparent(const rbnode *);
static rbnode *uncle(const rbnode *);
static void insert_case1(rbnode *);
static void insert_case2(rbnode *);
static void insert_case3(rbnode *);
static void insert_case4(rbnode *);

static rbnode *rbnodeNew(const void *data)
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

static int isLeaf(const rbtree *t, const rbnode *n)
{
    return (n == t->leaf)?1:0;
}

static void rbnodeDel(rbnode *n)
{
    if (!n) return;
    if (n->data) free(n->data);
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    free(n);
    n = NULL;
}

rbtreeClass *rbtreeClassNew(const rbcompf fa)
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
    rt->leaf = rbnodeNew(NULL);
    rt->klass = k;
    return rt;
}

void rbtreeDel(rbtree *t)
{
    if (t->root) _rbtreeDel(t->root);
    if (t->leaf) rbnodeDel(t->leaf);
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

void rbtreeInsert(rbtree *t, const void *data)
{
    rbnode *n = rbnodeNew((void *)data);
    n->left = t->leaf;
    n->right = t->leaf;
    if (!t || !data) {
        printf("rbtreeInsert: either the tree or the data is null!\n");
        return;
    }
    if (!t->root) {
        t->root = n;
        insert_case1(n);
    } else {
        _rbtreeInsert(t, t->root, n);
        insert_case2(n);
    }
}

static void _rbtreeInsert(const rbtree *t, rbnode *r, rbnode *n)
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

static rbnode *grandparent(const rbnode *n)
{
    rbnode *g;
    if (n && n->parent && (g = n->parent->parent)) return g;
    return NULL;
}

static rbnode *uncle(const rbnode *n)
{
    rbnode *p, *g;
    if (n && (p = n->parent) && (g = n->parent->parent)) {
        return (p == g->left)?(g->right):(g->left);
    } 
    return NULL;
}

static void insert_case1(rbnode *n)
{
    if (!(n->parent)) {
        n->color = BLACK;
    } else {
        insert_case2(n);
    }
}

static void insert_case2(rbnode *n)
{
    rbnode *p = n->parent;
    if (p->color == BLACK) return;
    insert_case3(n);
}

static void insert_case3(rbnode *n)
{
    rbnode *p = n->parent;
    rbnode *u = uncle(n);
    errcheck(p, "parent does not exist!");
    errcheck(u, "uncle does not exist!");
    
    if (RED == p->color && RED == u->color) {
        p->color = BLACK;
        u->color = BLACK;
    } else {
        insert_case4(n);
    }
}

static void insert_case4(rbnode *n)
{
}
