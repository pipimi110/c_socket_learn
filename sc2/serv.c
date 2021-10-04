#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

int read_num(int fd)
{
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));
    if (len > 0)
    {
        // int value = atoi(buf);
        // printf("read_num: %d\n", value);
        printf("read something");
        return 0;
    }
    else if (len == 0)
    {
        printf("connection close...\n");
        return -1;
    }
    else
    {
        perror("read");
        return -1;
    }
}

int create_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }
    printf("fd: %d\n", fd);
    return fd;
}
int bind_and_listen(int fd, int port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET,"0.0.0.0", &addr.sin_addr.s_addr);
    int ret = bind(fd, (struct sockaddr *)&addr,sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }
    printf("ret: %d\n", ret);

    ret = listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }
    return fd;
}

int wait_cli(int fd)
{
    //4.阻塞等待客户端连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    //已连接套接口
    int cfd = accept(fd, (struct sockaddr *)&caddr, &addrlen);
    if (cfd == -1)
    {
        perror("accept");
        return -1;
        // break;
    }
    // //连接建立成功，打印客户端和本地IP与端口信息
    char ip[24] = {0};
    char lip[24] = {0};
    printf("客户端IP: %s,端口： %d\n",
           inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(caddr.sin_port));
    // printf("loaclhost IP: %s,port： %d\n",
    //        inet_ntop(AF_INET, &saddr.sin_addr.s_addr, lip, sizeof(lip)),
    //        ntohs(saddr.sin_port));
    //接受数据
    // char buf[4096];
    // char judge[4096];
    // sprintf(judge, "请求服务器time...\n");
    // int len = recv(cfd, buf, sizeof(buf), 0);
    int ret = read_num(cfd);
    // ret = read_num(cfd);
    //关闭已连接套接口
    close(cfd);
    return 0;
}

int main(){
    int fd = create_socket();
    fd = bind_and_listen(fd,7777);
    fd = wait_cli(fd);
    close(fd);
    return 0;
}