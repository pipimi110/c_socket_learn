// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
int a()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }
    return fd;
}
void just_c(int fd,int port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    // int pt;
    // printf("please input your port:");
    // scanf("%d",&pt);
    // char yourip[128];
    // printf("please input your ip:");
    // scanf("%s",yourip);

    addr.sin_port = htons(port); // 大端端口
    // inet_pton(AF_INET, yourip , &addr.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect");
        exit(0);
    }
}

void send_num(int fd, char *np, int num1, int num2)
// void send_num(int fd, int num1, int num2)
{
    char buf[1024];
    // sprintf(buf, "add,%d,%d", num1, num2);
    // sprintf(buf, "add,%d,%d,", num1, num2);
    sprintf(buf, "%s,%d,%d,",np, num1, num2);
    int len = write(fd, buf, sizeof(buf));
    if (len == -1)
    {
        perror("write");
    }
    else
    {
        printf("write %d,%d\n", num1, num2);
    }
}
int recv_res(int cfd)
{
    char buf[1024];
    int len = recv(cfd, buf, sizeof(buf), 0);
    if (len > 0)
    {
        printf("recv res: %s\n", buf);
        // printf("recv res:");
        return 0;
    }
    else if (len == 0)
    {
        printf("server-side close...\n");
        return -1;
    }
    else
    {
        perror("recv");
        return -1;
    }
}

int main(int argc,char *argv[])
{

    srand((unsigned)time(NULL));
    int num1, num2;
    num1 = rand() % 50;
    // srand(num1%50);
    num2 = rand() % 50;
    printf("we have two number %d and %d\n", num1, num2);

    // 1. 创建通信的套接字
    int fd = a();
    // 2. 连接服务器
    printf("port: %d\n", atoi(argv[1]));
    just_c(fd, atoi(argv[1]));
    // 3. 和服务器端通信
    char *np1 = "add";
    send_num(fd,np1, num1, num2);
    // send_num(fd, num1, num2);
    recv_res(fd);
    close(fd);

    // 1. 创建通信的套接字
    int fd1 = a();
    // 2. 连接服务器
    just_c(fd1, atoi(argv[2]));
    // 3. 和服务器端通信
    sleep(1);
    char *np2 = "dec";
    send_num(fd1,np2, num1, num2);
    //printf("send_ook");
    recv_res(fd);
    close(fd1);

    // get_twice(num1,num2);

    return 0;
}
