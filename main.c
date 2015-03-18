#include "rbtree.h"

int compare(void *, void *);

int compare(void *a, void *b)
{
    printf("compare: entered\n");
    int ia = *(int *)a;
    int ib = *(int *)b;
    printf("compare: after inits\n");
    if (ia == ib) {
        printf("compare: eq\n");
        return 0;
    } else if (ia < ib) {
        printf("compare: lt\n");
        return -1;
    } else {
        printf("compare: gt\n");
        return 1;
    }
}

int main(const int argc, const char* argv[])
{
    printf("main: rbtree test\n");
    
    rbtreeClass *k = rbtreeClassNew((rbcompf)compare);

    if (k) printf("main: rbtreeClass created.\n");
    rbtree *t = rbtreeNew(k);

    if (t) {
        printf("main: rbtree exists!\n");
    } else {
        printf("main: rbtree does not exist :(");
        return -1;
    }

    int i, h;
    for (i=0, h=0; i < 10; i++) {
        int *ndata = malloc(sizeof(int));
        memcheck(ndata);
        *ndata = (h++) % 1000;
        h ^= h << 3;
        rbtreeInsert(t, (void *)ndata);
        printf("h = %d\n", *ndata);
    }
    printf("i = %d\n", i);

    rbtreeDel(t);
    return 0;
}
