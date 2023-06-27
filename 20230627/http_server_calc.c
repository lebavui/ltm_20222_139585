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

    char *pos = strchr(uri, '?');
    if (pos == NULL)
    {
        char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Thieu tham so</h1>";
        send(client, msg, strlen(msg), 0);
    }
    else
    {
        double a, b;
        int cmd = 0;
        char *p = strtok(pos + 1, "&");
        while (p != NULL)
        {
            if (strncmp(p, "a=", 2) == 0)
            {
                a = atof(p + 2);
            }
            else if (strncmp(p, "b=", 2) == 0)
            {
                b = atof(p + 2);
            }
            else if (strncmp(p, "cmd=", 4) == 0)
            {
                if (strncmp(p + 4, "add", 3) == 0)
                    cmd = 1;
                else if (strncmp(p + 4, "sub", 3) == 0)
                    cmd = 2;
                else if (strncmp(p + 4, "mul", 3) == 0)
                    cmd = 3;
                else if (strncmp(p + 4, "div", 3) == 0)
                    cmd = 4;
            }

            p = strtok(NULL, "&");
        }

        if (cmd == 1)
        {
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>%f+%f=%f</h1>", a, b, a + b);
        }
        else if (cmd == 2)
        {
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>%f-%f=%f</h1>", a, b, a - b);
        }
        else if (cmd == 3)
        {
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>%f*%f=%f</h1>", a, b, a * b);
        }
        else if (cmd == 4)
        {
            if (b == 0)
                strcpy(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Khong thuc hien duoc phep chia.</h1>");
            else
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>%f/%f=%f</h1>", a, b, a / b);
        }
        else
            strcpy(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Khong thuc hien duoc phep tinh.</h1>");

        send(client, buf, strlen(buf), 0);
    }
    
    close(client);
}