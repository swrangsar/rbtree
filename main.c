#include "rbtree.h"

int main(const int argc, const char* argv[])
{
    printf("main: rbtree test\n");
    
    rbtree *t = rbtreeNew();

    if (t) {
        printf("main: rbtree exists!\n");
    } else {
        printf("main: rbtree does not exist :(");
    }

    rbtreeDel(t);
    return 0;
}
