#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

int create_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }
    printf("fd: %d\n",fd);
    return fd;
}

int get_rsocket(int fd,char* ip,int port){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // 大端端口
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
    int conn_fd = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (conn_fd == -1)
    {
        perror("connect");
        exit(0);
    }
    printf("conn_fd: %d\n",conn_fd);
    return conn_fd;
}

void write_num(int fd, int num)
{
    char buf[1024];
    // sprintf(buf, "%d", num);
    sprintf(buf, "test");
    // int len = write(fd, buf, strlen(buf) + 1); //发送第一个数字
    int len = write(fd, buf, strlen(buf)); //发送第一个数字
    if (len == -1)
    {
        perror("write");
    }
    else
    {
        printf("write %d\n",num);
        printf("writed %d\n", len);
    }
}

int read_num(int fd)
{
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));
    if (len > 0)
    {
        int value = atoi(buf);
        printf("read_num: %d\n",value);
        // tmp += value;
        // printf("client say: %s\nAnd we will add them\n", buf);
        // sprintf(buf, "we got the first num!!");
        // write(cfd, buf, sizeof(buf));

        // value = atoi(buf);
        // tmp += value;
        // printf("client say: %s\nAnd we get the answer", buf);
        // printf("\n");
        // memset(buf, 0, sizeof(buf));
        // sprintf(buf, "%d", tmp);
        // write(cfd, buf, strlen(buf) + 1);
        // //write(cfd, tmp, sizeof(tmp));
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

void calc(int num1, int num2, char *flag)
{
    int fd = create_socket();
    // 2. 连接服务器
    int conn_fd = get_rsocket(fd, "127.0.0.1", 7777);
    // read_num(ret);
    write_num(conn_fd, num1);
    // write_num(conn_fd, num2);
    close(conn_fd);
    // // 3. 和服务器端通信
    // char buf[1024];
    // sprintf(buf, "%d", num1);
    // write(fd, buf, strlen(buf) + 1); //发送第一个数字
    // //printf("send_ook");

    // memset(buf, 0, sizeof(buf));

    // int len = read(fd, buf, sizeof(buf));
    // if (len > 0)
    // {
    //     printf("服务器say: %s\n", buf);
    // }
    // else if (len == 0)
    // {
    //     printf("服务器断开了连接...\n");
    // }
    // else
    // {
    //     perror("read");
    // }

    // memset(buf, 0, sizeof(buf));
    // sprintf(buf, "%d", num2);
    // write(fd, buf, strlen(buf) + 1); //发送第二个数字

    // memset(buf, 0, sizeof(buf));

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

    close(fd);
}

int main()
{

    srandom(time(NULL));
    int num1, num2;
    num1 = random() % 12345 - 2345;
    num2 = random() % 12345 - 2345;
    printf("we have two number %d and %d\n", num1, num2);

    calc(num1, num2, "add");
    // calc(num1, num2, "dec");

    return 0;
}
