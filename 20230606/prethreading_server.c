#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

void *thread_proc(void *);

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

    int num_threads = 8;
    pthread_t thread_id;
    for (int i = 0; i < num_threads; i++)
        pthread_create(&thread_id, NULL, thread_proc, &listener);
    
    pthread_join(thread_id, NULL);

    close(listener);    
    return 0;
}

void *thread_proc(void *param)
{
    int listener = *(int *)param;
    char buf[256];
    while (1)
    {
        int client = accept(listener, NULL, NULL);
        printf("new client accepted: %d\n", client);
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            continue;
        
        buf[ret] = 0;
        printf("Received: %s\n", buf);

        strcpy(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>");
        send(client, buf, strlen(buf), 0);

        close(client);
    }
}