// #include <ntsid.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    pid_t pid;

    int loop;
    //获得当前进程ID
    printf("Current Process Id = %d \n", getpid());

    for (loop = 0; loop < 2; loop++)
    {

        //fork创建子进程
        if ((pid = fork()) < 0)
        {
            printf("退出程序\n");
            exit(1);
        }
        else if (pid == 0)
        {

            //创建子进程后，子进程返回0，通过getPid得到当前进程，getPPid得到父进程
            printf("Child Process  loop = %d,  Current Pid = %d , Parent Pid = %d\n", loop, getpid(), getppid());
        }
        else
        {
            //创建子进程后，pid就是子进程ID
            printf("Child Process  loop = %d, Pid = %d,Parent Pid = %d\n", loop, pid, getpid());
            sleep(5);
        }
    }
    return 0;
}