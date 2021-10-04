#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
pthread_t thread;
void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s,
           (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}
void *thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *)0);
}
int main(void)
{
    int err;
    err = pthread_create(&thread, NULL, thr_fn, NULL);
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));
    sleep(1);
    printids("main thread:");
    sleep(1);
    exit(0);
}