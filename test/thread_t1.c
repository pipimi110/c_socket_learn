// #include <stdio.h>
// #include <pthread.h>
// pthread_t thread;
// void fn(void *arg)
// {
//     int i = *(int *)arg;
//     printf("i= %d\n",i);
//     return ((void *)0);
// }
// int main()
// {
//     int err1;
//     int i = 10;
//     err1 = pthread_create(&thread, NULL, fn, &i);
//     pthread_join(thread, NULL);
// }
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *thread_function(void *arg)
{
    int i = *(int *)arg;
    for (; i < 8; i++)
    {
        printf("Thread working...! %d \n", i);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t t1,t2;
    int i = 0;
    int j = 6;
    pthread_create(&t1, NULL, thread_function, (void*)&i);
    pthread_create(&t2, NULL, thread_function, (void*)&j);
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    // pthread_detach(t1);
    // pthread_detach(t2);
    printf("thread done! \n");
    sleep(8);
    exit(0);
    return 0;
}