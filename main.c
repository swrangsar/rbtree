#include "rbtree.h"

int main(const int argc, const char* argv[])
{
    printf("main: rbtree test\n");
    
    rbnode *rn = rbnodeNew(NULL);
    if (isLeaf(rn)) {
        printf("main: rn is leaf. Yay.\n");
    } else {
        printf("main: rn is not as expected :(");
    }

    rbnodeDel(rn);
    return 0;
}
