#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

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

    printf("Khoi tao thanh cong!!!\n");

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int client = accept(listener, 
        (struct sockaddr *)&client_addr, 
        &client_addr_len);

    printf("Accepted socket %d from IP: %s:%d\n", 
        client,
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));

    char buf[256];
    int ret;

    char *request = NULL;
    int size = 0;

    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        request = realloc(request, size + ret);
        memcpy(request + size, buf, ret);
        size += ret;

        if (strstr(request, "\r\n\r\n") != NULL)
            break;
    }    

    printf("%d bytes from client: %s", size, request);
    free(request);

    strcpy(buf, "HTTP/1.1 200 OK\r\nContentType: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>");
    send(client, buf, strlen(buf), 0);

    close(client);
    close(listener);
}