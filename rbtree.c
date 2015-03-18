#include "rbtree.h"


static rbnode *rbnodeNew(const void *);
static int isLeaf(const rbtree *, const rbnode *);
static void rbtreeClassDel(rbtreeClass *);
static void rbnodeDel(rbnode *);
static void _rbtreeDel(rbtree *, rbnode *);
static void _rbtreeInsert(const rbtree *, rbnode *, rbnode *);
static rbnode *grandparent(const rbnode *);
static rbnode *uncle(const rbnode *);
static void rotate_left(rbtree *, rbnode *);
static void rotate_right(rbtree *, rbnode *);
static void insert_case1(rbtree *, rbnode *);
static void insert_case2(rbtree *, rbnode *);
static void insert_case3(rbtree *, rbnode *);
static void insert_case4(rbtree *, rbnode *);
static void insert_case5(rbtree *, rbnode *);

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
    if (n->data) free((void *)n->data);
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
    k->cmp = fa;
    return k;
}

static void rbtreeClassDel(rbtreeClass *k)
{
    k->cmp = NULL;
    free(k);
}

rbtree *rbtreeNew(rbtreeClass *k)
{
    rbtree *rt = malloc(sizeof(rbtree));
    memcheck(rt);
    rt->root = NULL;
    rt->leaf = rbnodeNew(NULL);
    errcheck(rt->leaf, "leaf of tree shouldn't be NULL here!");
    rt->leaf->color = BLACK;
    rt->klass = k;
    return rt;
}

void rbtreeDel(rbtree *t)
{
    if (t->root) _rbtreeDel(t, t->root);
    if (t->leaf) rbnodeDel(t->leaf);
    if (t->klass) rbtreeClassDel(t->klass);
    t->root = NULL;
    t->leaf = NULL;
    t->klass = NULL;
    free(t);
    t = NULL;
}

static void _rbtreeDel(rbtree *t, rbnode *n)
{
    if (!isLeaf(t, n->left)) _rbtreeDel(t, n->left);
    if (!isLeaf(t, n->right)) _rbtreeDel(t, n->right);
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
    if (!t->root || isLeaf(t, t->root)) {
        t->root = n;
        insert_case1(t, n);
    } else {
        _rbtreeInsert(t, t->root, n);
        insert_case2(t, n);
    }
}

static void _rbtreeInsert(const rbtree *t, rbnode *r, rbnode *n)
{
    int res = t->klass->cmp(n->data, r->data);
    if (res < 0) {
        if (!isLeaf(t, r->left)) {
            _rbtreeInsert(t, r->left, n);
        } else {
            r->left = n;
            n->parent = r;
        }
    } else if (res > 0) {
        if (!isLeaf(t, r->right)) {
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

static void rotate_left(rbtree *t, rbnode *n)
{
    errcheck(n, "rotate_left: n is null!");
    rbnode *p = n->parent;
    rbnode *r = n->right;
    errcheck(!isLeaf(t, r), "rotate_left: r is null!");
    if (p) p->left = r;
    n->right = r->left;
    r->left = n;
    n->parent = r;
    r->parent = p;
}

static void rotate_right(rbtree *t, rbnode *n)
{
    errcheck(n, "rotate_right: n is null!");
    rbnode *p = n->parent;
    rbnode *l = n->left;
    errcheck(!isLeaf(t, l), "rotate_right: l is null!");
    if (p) p->right = l;
    n->left = l->right;
    l->right = n;
    n->parent = l;
    l->parent = p;
}
    

static void insert_case1(rbtree *t, rbnode *n)
{
    if (!(n->parent)) {
        n->color = BLACK;
    } else {
        insert_case2(t, n);
    }
}

static void insert_case2(rbtree *t, rbnode *n)
{
    rbnode *p = n->parent;
    if (p->color == BLACK) return;
    insert_case3(t, n);
}

static void insert_case3(rbtree *t, rbnode *n)
{
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    rbnode *u = uncle(n);
    errcheck(p, "parent does not exist!");
    errcheck(u, "uncle does not exist!");
    errcheck(g, "grandparent does not exist!");
    
    if (RED == u->color) {
        p->color = BLACK;
        u->color = BLACK;
        insert_case1(t, g);
    } else {
        insert_case4(t, n);
    }
}

static void insert_case4(rbtree *t, rbnode *n)
{
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    errcheck(p, "parent does not exist!");
    errcheck(g, "grandparent does not exist!");
    
    if (n == p->right && p == g->left) {
        rotate_left(t, p);
        insert_case5(t, p);
    } else if (n == p->left && p == g->right) {
        rotate_right(t, p);
        insert_case5(t, p);
    } else {
        insert_case5(t, n);
    }
}

static void insert_case5(rbtree *t, rbnode *n)
{
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    errcheck(p, "parent does not exist!");
    errcheck(g, "grandparent does not exist!");
    p->color = BLACK;
    g->color = RED;
    (n == p->left)?rotate_right(t, g):rotate_left(t, g);
    if (!(p->parent)) {
        t->root = p;
    }
}
