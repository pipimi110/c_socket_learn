// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

void *working(void *arg) ;
int main()
{
    // 1. 创建监听的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0); //监听套接口
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }
    //2.绑定本地IP
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7777);
    //saddr.sin_addr.s_addr = INADDR_ANY; 0 = 0.0.0.0 读取本地IP绑定
    // inet_pton(AF_INET, "172.26.111.33", &saddr.sin_addr.s_addr);
    inet_pton(AF_INET, "0.0.0.0", &saddr.sin_addr.s_addr);
    int ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)); //赋予一个套接口
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    //3.设置监听
    ret = listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }

    //5.通信
    while (1)
    {
        // fork_test(fd);
        // pid = fork();
        // if (pid == -1)
        //     ERR_EXIT("fork error");
        // if (pid == 0)
        // {
        //     fork_test(fd);
        //     exit(EXIT_SUCCESS);
        // }
        // else
        //     break; //父进程
        int len=100;
        pthread_t t[len];
        for(int i=0;i<len;i++){
            pthread_create(&t[i], NULL, working, (void *)&fd);
        }
        for(int i=0;i<len;i++){
            pthread_join(t[i], NULL);
        }
        printf("create thread over");
        // pthread_detach(t1);
        // pthread_detach(t2);
    }
    close(fd);

    return 0;
}

void *working(void *arg)
{
    int fd = *(int *)arg;
    //4.阻塞等待客户端连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    //已连接套接口
    int cfd = accept(fd, (struct sockaddr *)&caddr, &addrlen);
    if (cfd == -1)
    {
        perror("accept");
        // return -1;
        // break;
    }
    //连接建立成功，打印客户端和本地IP与端口信息
    char ip[24] = {0};
    char lip[24] = {0};
    printf("客户端IP: %s,端口： %d\n",
           inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(caddr.sin_port));

    // printf("loaclhost IP: %s,port： %d\n",
    //        inet_ntop(AF_INET, &saddr.sin_addr.s_addr, lip, sizeof(lip)),
    //        ntohs(saddr.sin_port));
    //接受数据
    char buf[4096];
    char judge[4096];
    sprintf(judge, "请求服务器time...\n");
    int len = recv(cfd, buf, sizeof(buf), 0);
    if (len > 0)
    {
        printf("客户端say: %s", buf);
        printf("sleep 3 (thread test)\n\n");
        sleep(3);
        int flag = strcmp(buf, judge);
        if (flag == 0)
        {
            time_t curtime;
            time(&curtime);
            write(cfd, ctime(&curtime), len);
        }
    }
    else if (len == 0)
    {
        printf("客户端断开了连接...\n");
        // break;
    }
    else
    {
        perror("recv");
        // break;
    }
    //关闭已连接套接口
    close(cfd);
}

void fork_test(int arg)
{
    int fd = arg;
    //4.阻塞等待客户端连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    //已连接套接口
    int cfd = accept(fd, (struct sockaddr *)&caddr, &addrlen);
    if (cfd == -1)
    {
        perror("accept");
        // return -1;
        // break;
    }
    //连接建立成功，打印客户端和本地IP与端口信息
    char ip[24] = {0};
    char lip[24] = {0};
    printf("客户端IP: %s,端口： %d\n",
           inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(caddr.sin_port));

    // printf("loaclhost IP: %s,port： %d\n",
    //        inet_ntop(AF_INET, &saddr.sin_addr.s_addr, lip, sizeof(lip)),
    //        ntohs(saddr.sin_port));
    //接受数据
    char buf[4096];
    char judge[4096];
    sprintf(judge, "请求服务器time...\n");
    int len = recv(cfd, buf, sizeof(buf), 0);
    if (len > 0)
    {
        printf("客户端say: %s", buf);
        printf("sleep 3 (thread test)\n\n");
        sleep(3);
        int flag = strcmp(buf, judge);
        if (flag == 0)
        {
            time_t curtime;
            time(&curtime);
            write(cfd, ctime(&curtime), len);
        }
    }
    else if (len == 0)
    {
        printf("客户端断开了连接...\n");
        // break;
    }
    else
    {
        perror("recv");
        // break;
    }
    //关闭已连接套接口
    close(cfd);
}