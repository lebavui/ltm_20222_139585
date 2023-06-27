#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

void *client_thread(void *);

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

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            perror("accept() failed");
            continue;
        }
        printf("New client connected: %d\n", client);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_thread, &client);
        pthread_detach(thread_id);
    }
    
    close(listener);    

    return 0;
}

void *client_thread(void *param)
{
    int client = *(int *)param;
    char buf[2048];

    int ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
    
    char method[16], uri[256];
    sscanf(buf, "%s%s", method, uri);

    if (strcmp(uri, "/web") == 0)
    {
        char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Xin chao.</h1></body></html>";
        send(client, response, strlen(response), 0);
    }
    else if (strcmp(uri, "/text") == 0)
    {
        char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
        send(client, header, strlen(header), 0);

        FILE *f = fopen("test.txt", "rb");
        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(uri, "/image") == 0)
    {
        FILE *f = fopen("test.jpg", "rb");
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);

        char header[256];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: image/jpeg\r\n\r\n", size);
        send(client, header, strlen(header), 0);

        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else
    {
        char *response = "HTTP/1.1 404 File not found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Khong tim thay noi dung yeu cau.</h1></body></html>";
        send(client, response, strlen(response), 0);    
    }
    
    close(client);
}