// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int split_and_add(char str[], char delims[])
{
    char *result = NULL;
    result = strtok(str, delims);
    int res = 0;
    int flag = 0;
    int i = 0;
    int nums[4];
    char judge[1024];
    sprintf(judge, "add");
    printf("%s %s\n",judge,result);
    if (strcmp(judge,result)==0){
        flag = 1;
    }
    printf("flag: %d\n",flag);
    // while (result != NULL)
    while (i<3)
    {
        nums[i] = atoi(result);
        printf("result is \"%s\"\n", result);
        // res += atoi(result);
        result = strtok(NULL, delims);
        i++;
    }
    if (flag) {
        res = nums[1]+nums[2];
    }else {
        res = nums[1]-nums[2];
    }
    return res;
}
void send_num(int fd, int res)
{
    char buf[1024];
    sprintf(buf, "%d", res);
    // mistake1
    // int len = write(fd, buf, strlen(buf)); //发送第一个数字
    int len = write(fd, buf, sizeof(buf)); //发送第一个数字
    if (len == -1)
    {
        perror("write");
    }
    else
    {
        printf("write %d\n", res);
    }
}
int recv_and_calc_and_send(int cfd)
{
    char buf[1024];
    char judge[1024];
    sprintf(judge, "请求服务器time...\n");
    int len = recv(cfd, buf, sizeof(buf), 0);
    if (len > 0)
    {
        // printf("客户端say: %s", buf);
        // // printf("sleep 3 (thread test)\n\n");
        // // sleep(3);
        // int flag = strcmp(buf, judge);
        // if (flag == 0)
        // {
        //     time_t curtime;
        //     time(&curtime);
        //     write(cfd, ctime(&curtime), len);
        // }
        char delims[] = ",";
        int res = split_and_add(buf, delims);
        printf("send res: %d\n",res);
        send_num(cfd, res);
        return 0;
    }
    else if (len == 0)
    {
        printf("client-side close...\n");
        return -1;
    }
    else
    {
        perror("recv");
        return -1;
    }
}
int create_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0); //监听套接口
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }
    return fd;
}

int bind_and_listen(int fd, int port)
{
    //2.绑定本地IP
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    //saddr.sin_addr.s_addr = INADDR_ANY; 0 = 0.0.0.0 读取本地IP绑定
    // inet_pton(AF_INET, "172.26.111.33", &saddr.sin_addr.s_addr);
    inet_pton(AF_INET, "0.0.0.0", &saddr.sin_addr.s_addr);
    int ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)); //赋予一个套接口
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }
    //3.设置监听
    ret = listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }
    return fd;
}
int d(int fd){
    //4.阻塞等待客户端连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    //已连接套接口
    int cfd = accept(fd, (struct sockaddr *)&caddr, &addrlen);
    if (cfd == -1)
    {
        perror("accept");
        return -1;
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
    recv_and_calc_and_send(cfd);

    //关闭已连接套接口
    close(cfd);
    return 0;
}
int main(int argc, char *argv[])
{
    int fd = create_socket();
    printf("port: %d\n", atoi(argv[1]));
    bind_and_listen(fd, atoi(argv[1]));

    //5.通信
    while (1)
    {
        d(fd);
    }
    close(fd);

    return 0;
}
