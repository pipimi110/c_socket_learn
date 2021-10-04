// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<time.h>
int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);   // 大端端口
    // inet_pton(AF_INET, "8.129.107.19", &addr.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }
    //连接建立成功，打印客户端和本地IP与端口信息
    char ip[24] = {0};

    printf( "sever IP: %s,端口： %d\n",
    inet_ntop(AF_INET,&addr.sin_addr.s_addr , ip, sizeof(ip)),
    ntohs(addr.sin_port));
    // 3. 和服务器端通信
    int number = 0;
    while(1)
    {
        // 发送数据
        char buf[4096];
        sprintf(buf, "请求服务器time...\n");
        write(fd, buf, strlen(buf)+1);
        
        // 接收数据
        memset(buf, 0, sizeof(buf));
        int len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            printf("服务器say: %s\n", buf);
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
        sleep(1);   // 每隔1s发送一条数据
    }
//关闭文件描述符
    close(fd);

    return 0;
}