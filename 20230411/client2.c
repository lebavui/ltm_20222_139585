#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Sai tham so.\n");
        return 1;
    }

    // Xu ly xau argv[1]
    char *pos = strchr(argv[1] + 7, '/');
    char *domain = NULL;
    char *path = NULL;

    if (pos != NULL)
    {
        // Co path (/)
        int domain_len = (pos - argv[1]) - 7;
        domain = malloc(domain_len + 1);
        memcpy(domain, argv[1] + 7, domain_len);
        domain[domain_len] = 0;
        int path_len = strlen(argv[1]) - domain_len - 7;
        path = malloc(path_len + 1);
        memcpy(path, argv[1] + domain_len + 7, path_len);
        path[path_len] = 0;
    }
    else
    {
        // Khong co path (/)
        int domain_len = strlen(argv[1]) - 7;
        domain = malloc(domain_len + 1);
        memcpy(domain, argv[1] + 7, domain_len);
        domain[domain_len] = 0;
        int path_len = 1;
        path = malloc(path_len + 1);
        path[0] = '/';
        path[1] = 0;
    }

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo *result;
    int ret = getaddrinfo(domain, "http", NULL, &result);
    if (ret != 0 || result == NULL)
    {
        printf("getaddrinfo() failed.\n");
        return 1;
    }

    ret = connect(client, result->ai_addr, result->ai_addrlen);
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    char msg[256];
    sprintf(msg, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, domain);
    send(client, msg, strlen(msg), 0);

    char buf[256];
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
    }
    
    close(client);
}