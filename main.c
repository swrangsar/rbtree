#include "rbtree.h"

#define INIT_VAL    0

int compare(const void *, const void *);
void destroy(void *);
void show(void *);

int compare(const void *a, const void *b)
{
    int ia = *(int *)a;
    int ib = *(int *)b;
    if (ia == ib) {
        return 0;
    } else if (ia < ib) {
        return -1;
    } else {
        return 1;
    }
}

void destroy(void *data)
{
    if (data) {
        printf("freeing int* data.. %d\n", *(int *)data);
        free((int *)data);
    } else {
        errcheck(data, "data is null in destroy()!");
    }
}

void show(void *data)
{
    if (data) {
        printf("rbnode data:  %d\n", *(int *)data);
    } else {
        printf("data is null in show()!\n");
    }
}

int main(const int argc, const char* argv[])
{
    printf("main: rbtree test\n");
    
    rbtreeClass *k = rbtreeClassNew((rbcmpf)compare, (rbdstf)destroy, (rbshwf)show);

    if (k) printf("main: rbtreeClass created.\n");
    rbtree *t = rbtreeNew(k);

    if (t) {
        printf("main: rbtree exists!\n");
    } else {
        printf("main: rbtree does not exist :(");
        return -1;
    }

    int iDel = 0;
    int i, h;
    for (i=0, h=INIT_VAL; i < 10; i++) {
        int *ndata = malloc(sizeof(int));
        memcheck(ndata);
        *ndata = (h++) % 10;
        if (i == 5) iDel = *ndata;
        h ^= h << 3;
        h ^= h >> 5;
        printf("h = %d\n", *ndata);
        rbtreeInsert(t, (void *)ndata);
    }
    printf("i = %d\n", i);

    if (t->root) {
        printf("root data = %d\n", *(int *)t->root->data);
    }

    printf("constructing data out of iDel...\n");
    int *delData = malloc(sizeof(int));
    memcheck(delData);
    *delData = iDel;
    printf("*delData is %d\n", *delData);
    rbtreeRemove(t, delData);

    free(delData);
    rbtreeDel(t);
    return 0;
}
