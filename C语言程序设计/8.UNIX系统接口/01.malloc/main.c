#include "malloc.h"

int main()
{
    int *a;
    a = (int *)malloc2(sizeof(int));
    *a = 10;
    printf("%d\n", *a);
    return 0;
}
