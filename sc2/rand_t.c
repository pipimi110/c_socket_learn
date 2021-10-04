#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    int a;
    srandom(time(NULL));
    a = random() % 51 + 13;
    printf("%d\n", a);
    return 0;
}