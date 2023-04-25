#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    unsigned long ul = 1;
    ioctl(listener, FIONBIO, &ul);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }

    int clients[64];
    int num_clients = 0;
    char buf[256];

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // Loi do thao tac vao ra chua hoan tat
                // Van dang cho ket noi
                // Bo qua khong xu ly
            }
            else
            {
                perror("accept() failed");
                break;
            }
        }
        else
        {
            printf("New client connected: %d\n", client);
            clients[num_clients++] = client;
            unsigned long ul = 1;
            ioctl(client, FIONBIO, &ul);
        }

        for (int i = 0; i < num_clients; i++)
        {
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if (ret == -1)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    // Loi do thao tac vao ra chua hoan tat
                    // Van dang doi du lieu
                    // Bo qua chuyen sang socket khac
                }
                else
                {
                    perror("recv() failed");
                    continue;
                }
            }
            else if (ret == 0)
            {
                // Ket noi bi dong;
                // TODO: Xoa socket client ra khoi mang
                continue;
            }
            else
            {
                buf[ret] = 0;
                printf("Received from %d: %s\n", clients[i], buf);
            }            
        }
    }
    
    close(listener);    

    return 0;
}