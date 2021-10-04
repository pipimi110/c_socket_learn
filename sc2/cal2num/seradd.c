// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    int pt = 9999;
    // printf("please input your port:");
    // scanf("%d",&pt);
    addr.sin_port = htons(pt);

    //addr.sin_port = htons(10000);   // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY;  // 这个宏的值为0 == 0.0.0.0
//    inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. 设置监听
    ret = listen(lfd, 128);
    if(ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // 4. 阻塞等待并接受客户端连接
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
    if(cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(cliaddr.sin_port));

    // 5. 和客户端通信
	int flag=0;
	int value=0;
	int tmp=0;
    while(1)
    {
        // 接收数据
        char buf[1024];
	
        memset(buf, 0, sizeof(buf));
        int len = read(cfd, buf, sizeof(buf));
		
		//printf("%s",buf);
		
		//printf("the value from client is %d",value);
        if(len > 0)
        {
			printf("the flag is %d\n",flag);
			if(flag==0){
			value=atoi(buf);
			tmp+=value;
            printf("client say: %s\nAnd we will add them\n", buf);
			sprintf(buf,"we got the first num!!");
            write(cfd, buf, sizeof(buf));
				}
			else
			if(flag==1){
			value=atoi(buf);
			tmp+=value;
            printf("client say: %s\nAnd we get the answer", buf);
			printf("\n");
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d",tmp);
			write(cfd,buf,strlen(buf)+1);
            //write(cfd, tmp, sizeof(tmp));
				}
			flag++;
			if(flag==2) break;
        }
        else if(len  == 0)
        {
            printf("客户端断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    close(cfd);
    close(lfd);

    return 0;
}


