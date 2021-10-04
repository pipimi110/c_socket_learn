// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

void get_twice(int num1,int num2)
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
    inet_pton(AF_INET, "127.0.0.1" , &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }

    // 3. 和服务器端通信
    
	char buf[1024];

	sprintf(buf,"%d",num1);
	
	write(fd,buf,strlen(buf)+1);//发送第一个数字
	//printf("send_ook");

	memset(buf,0,sizeof(buf));

	int len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            printf("服务器say: %s\n", buf);
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
        }
        else
        {
            perror("read");
        }


	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",num2);
	write(fd,buf,strlen(buf)+1);//发送第二个数字

	memset(buf,0,sizeof(buf));

	/*len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            printf("服务器say: %s\n", buf);
			
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
        }
        else
        {
            perror("read");
        }
	*/
	len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            printf("our answer is : %s\n", buf);
			
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
        }
        else
        {
            perror("read");
        }   
   
	close(fd);

}

int main()
{
		
	srand((unsigned)time(NULL));
	int num1,num2;
	num1=rand()%23333;
	srand(num1%23333);
	num2=rand()%23333;
	printf("we have two number %d and %d\n",num1,num2);

	get_twice(num1,num2);
	get_twice(num1,num2);

    

    return 0;
}


