#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int i;
    char lo[] = "127.0.0.1";
    struct in_addr netAddr;
    netAddr.s_addr = inet_addr(lo);
    printf("NetIP: 0x%x\n", netAddr.s_addr);
    char *strAddr = inet_ntoa(netAddr);
    printf("StrIP: %s\n", strAddr);
    int ret = inet_aton(strAddr, &netAddr);
    printf("NetIP: 0x%x\n", netAddr.s_addr);
    return 0;
}