#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    char buf[256];
    while (1)
    {
        printf("Nhap xau: ");
        fgets(buf, sizeof(buf), stdin);
        int ret = sendto(sender, buf, strlen(buf), 0,
            (struct sockaddr *)&addr, sizeof(addr));
        printf("%d bytes sent\n", ret);
        
        if (strncmp(buf, "exit", 4) == 0)
            break;
    }
}