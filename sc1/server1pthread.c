// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<time.h>
#include <pthread.h>

//定义要传入working的结构体
struct SockInfo
{
    struct sockaddr_in addr;
    int fd;
    int num;
};
struct SockInfo infos[512];

void* working(void* arg);

int main()
{

    // 1. 创建监听的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        return -1;
    }
    //2.绑定本地IP
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7777);
    //saddr.sin_addr.s_addr = INADDR_ANY; 0 = 0.0.0.0 读取本地IP绑定
    inet_pton(AF_INET, "0.0.0.0", &saddr.sin_addr.s_addr);
    int ret = bind (fd, (struct  sockaddr*)&saddr,sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    //3.设置监听
    ret =listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    //初始化结构体数组
    int max =  sizeof(infos) / sizeof(infos[0]);
    for(int i= 0;i<max;i++)
    {
        bzero(&infos[i], sizeof(infos[i])); //全部置0
        infos[i].fd = -1;//空闲
    }
    //打印服务器IP与端口
    char lip[24] = {0};
    printf( "loaclhost IP: %s,port： %d\n",
    inet_ntop(AF_INET,&saddr.sin_addr.s_addr , lip, sizeof(lip)),
    ntohs(saddr.sin_port));

    //4.阻塞等待客户端连接,
    int addrlen = sizeof(struct sockaddr_in);
    while (1)
    {
        struct SockInfo* pinfo;
        for(int i =0;i<max;i++)
        {
            infos[i].num = i ;
            if(infos[i].fd == -1)
            {
                pinfo = &infos[i];
                break;
            }
        }

        int cfd = accept(fd,(struct  sockaddr*)&pinfo->addr ,&addrlen);
        pinfo->fd =cfd;
        if (cfd == -1)
            {
                perror("accept");
                return -1;
            }  
        //创建子线程
        pthread_t  tid;
        pthread_create(&tid, NULL ,working , pinfo );
        pthread_detach(tid);//资源回收
    }
    
    close(fd);
    return 0;
}   
//working
void* working(void* arg)
{
    struct SockInfo* pinfo = (struct SockInfo*)arg;

    //连接建立成功，打印客户端IP与端口信息
    char ip[24] = {0};
    printf( "客户端IP: %s,端口： %d\n",
        inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr , ip, sizeof(ip)),
    ntohs(pinfo->addr.sin_port));

    //5.通信
    while(1)
    {
        //接受数据
        char buf[4096];
        char judge[4096];
        sprintf(judge, "请求服务器time...\n");//设置判别字符串
        int len = recv(pinfo->fd,buf,sizeof(buf),0);
        if(len > 0)
        {
            printf("客户端 %d say: %s\n", pinfo->num,buf);
            int flag=strcmp(buf,judge);
            if(flag == 0)
            {
                time_t curtime;
                time(&curtime);
                write(pinfo->fd, ctime(&curtime), len);
            }
        }
        else if(len  == 0)
        {
            printf("客户端%d断开了连接...\n",pinfo->num);
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }


    close(pinfo->fd);
    pinfo->fd =  -1;//重置
    return NULL;
}
