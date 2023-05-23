#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    saddr.sin_port = htons(atoi(argv[2]));

    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_addr.s_addr = INADDR_ANY;
    raddr.sin_port = htons(atoi(argv[3]));

    bind(receiver, (struct sockaddr *)&raddr, sizeof(raddr));

    fd_set fdread, fdtest;
    FD_ZERO(&fdread);
    FD_SET(STDIN_FILENO, &fdread);
    FD_SET(receiver, &fdread);

    char buf[256];

    while (1)
    {
        fdtest = fdread;
        int ret = select(receiver + 1, &fdtest, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select() failed");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &fdtest))
        {
            fgets(buf, sizeof(buf), stdin);
            sendto(sender, buf, strlen(buf), 0, 
                (struct sockaddr *)&saddr, sizeof(saddr));
        }

        if (FD_ISSET(receiver, &fdtest))
        {
            ret = recvfrom(receiver, buf, sizeof(buf), 0, NULL, NULL);
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }
    }

    close(sender);
    close(receiver);
}