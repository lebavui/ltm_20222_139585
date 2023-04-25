#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000); 

    // if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
    //     perror("connect() failed");
    //     return 1;
    // }
        
    fd_set fdread, fdtest;
    
    FD_ZERO(&fdread);
    FD_SET(STDIN_FILENO, &fdread);

    client = 65;

    FD_SET(client, &fdread);

    char buf[256];

    while (1)
    {
        fdtest = fdread;

        int ret = select(client + 1, &fdtest, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &fdtest))
        {
            fgets(buf, sizeof(buf), stdin);
            send(client, buf, strlen(buf), 0);
        }

        if (FD_ISSET(client, &fdtest))
        {
            ret = recv(client, buf, sizeof(buf), 0);
            if (ret <= 0)
                break;
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }
    }

    // Ket thuc, dong socket
    close(client);

    return 0;
}