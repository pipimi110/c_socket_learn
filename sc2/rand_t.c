#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    int a;
    srandom(time(NULL));
    a = random() % 51 + 13;
    printf("%d\n", a);
    char buf[1024];
    sprintf(buf,"%d",a);
    int value = atoi(buf);
    printf("value: %d\n", value);
    return 0;
}