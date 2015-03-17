#include "rbtree.h"

int compare(void *, void *);

int compare(void *a, void *b)
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
    }

    rbtreeDel(t);
    return 0;
}
