#include "rbtree.h"


static rbnode *rbnodeNew(const void *);
static void rbtreeClassDel(rbtreeClass *);
static void rbnodeDel(rbtree *, rbnode *);
static void _rbtreeDel(rbtree *, rbnode *);
static void _rbtreeInsert(rbtree *, rbnode *, rbnode *);
static rbnode *grandparent(const rbnode *);
static rbnode *uncle(const rbnode *);
static void rotate_left(rbnode *);
static void rotate_right(rbnode *);
static void insert_case1(rbtree *, rbnode *);
static void insert_case2(rbtree *, rbnode *);
static void insert_case3(rbtree *, rbnode *);
static void insert_case4(rbtree *, rbnode *);
static void insert_case5(rbtree *, rbnode *);

rbtreeClass *rbtreeClassNew(const rbcmpf fa, const rbdstf fb)
{
    rbtreeClass *k = malloc(sizeof(rbtreeClass));
    memcheck(k);
    k->cmp = fa;
    k->dst = fb;
    return k;
}

rbtree *rbtreeNew(rbtreeClass *k)
{
    rbtree *t = malloc(sizeof(rbtree));
    memcheck(t);
    t->root = NULL;
    t->klass = k;
    return t;
}

void rbtreeDel(rbtree *t)
{
    errcheck(t, "rbtree is null!");
    if (t->root) _rbtreeDel(t, t->root);
    if (t->klass) rbtreeClassDel(t->klass);
    t->root = NULL;
    t->klass = NULL;
    free(t);
    t = NULL;
}

void rbtreeInsert(rbtree *t, const void *data)
{
    rbnode *n = rbnodeNew((void *)data);
    errcheck(t, "rbtreeInsert: tree is null!");
    errcheck(data, "rbtreeInsert: data is null!");

    if (!t->root) {
        t->root = n;
        insert_case1(t, n);
    } else {
        _rbtreeInsert(t, t->root, n);
        insert_case2(t, n);
    }
}


// ******************************************
//      Static functions
// ******************************************

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


static void rbnodeDel(rbtree *t, rbnode *n)
{
    if (!n) return;
    errcheck(t, "rbtree is null!");
    if (n->data) t->klass->dst(n->data);
    n->data = NULL;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    free(n);
    n = NULL;
}


static void rbtreeClassDel(rbtreeClass *k)
{
    k->cmp = NULL;
    k->dst = NULL;
    free(k);
    k = NULL;
}

static void _rbtreeDel(rbtree *t, rbnode *n)
{
    errcheck(t, "rbtree is null!");
    if (n->left) _rbtreeDel(t, n->left);
    if (n->right) _rbtreeDel(t, n->right);
    rbnodeDel(t, n);
}

static void _rbtreeInsert(rbtree *t, rbnode *r, rbnode *n)
{
    errcheck(r, "rbnode *r is null!");
    errcheck(n, "rbnode *n is null!");
    int res = t->klass->cmp(n->data, r->data);
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
        rbnodeDel(t, n);
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

static void rotate_left(rbnode *n)
{
    errcheck(n, "rotate_left: n is null!");
    rbnode *p = n->parent;
    rbnode *r = n->right;
    errcheck(r, "rotate_left: r is null!");
    if (p) {
        if (n == p->left) {
            p->left = r;
        } else {
            p->right = r;
        }
    }
    n->right = r->left;
    r->left = n;
    n->parent = r;
    r->parent = p;
    if (n->right) n->right->parent = n;
}

static void rotate_right(rbnode *n)
{
    errcheck(n, "rotate_right: n is null!");
    rbnode *p = n->parent;
    rbnode *l = n->left;
    errcheck(l, "rotate_right: l is null!");
    if (p) {
        if (n == p->left) {
            p->left = l;
        } else {
            p->right = l;
        }
    }
    n->left = l->right;
    l->right = n;
    n->parent = l;
    l->parent = p;
    if (n->left) n->left->parent = n;
}
    

static void insert_case1(rbtree *t, rbnode *n)
{
    printf("insert case 1\n");
    if (!(n->parent)) {
        n->color = BLACK;
    } else {
        insert_case2(t, n);
    }
}

static void insert_case2(rbtree *t, rbnode *n)
{
    printf("insert case 2\n");
    rbnode *p = n->parent;
    errcheck(p, "p is null!");
    if (p->color == BLACK) return;
    insert_case3(t, n);
}

static void insert_case3(rbtree *t, rbnode *n)
{
    printf("insert case 3\n");
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    rbnode *u = uncle(n);
    errcheck(p, "parent does not exist!");
    errcheck(g, "grandparent does not exist!");
    
    if (u && RED == u->color) {
        p->color = BLACK;
        u->color = BLACK;
        g->color = RED;
        insert_case1(t, g);
    } else {
        insert_case4(t, n);
    }
}

static void insert_case4(rbtree *t, rbnode *n)
{
    printf("insert case 4\n");
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    errcheck(p, "parent does not exist!");
    errcheck(g, "grandparent does not exist!");
    
    if (n == p->right && p == g->left) {
        rotate_left(p);
        insert_case5(t, p);
    } else if (n == p->left && p == g->right) {
        rotate_right(p);
        insert_case5(t, p);
    } else {
        insert_case5(t, n);
    }
}

static void insert_case5(rbtree *t, rbnode *n)
{
    printf("insert case 5\n");
    rbnode *p = n->parent;
    rbnode *g = grandparent(n);
    errcheck(p, "parent does not exist!");
    errcheck(g, "grandparent does not exist!");
    p->color = BLACK;
    g->color = RED;
    (n == p->left)?rotate_right(g):rotate_left(g);
    if (!(p->parent)) {
        printf("insert_case5: p parent is null!\n");
        t->root = p;
    }
}
