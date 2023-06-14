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

int clients[64];
int num_clients = 0;

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

        clients[num_clients] = client;
        num_clients++;

        if (num_clients % 2 == 1)
        {
            char *msg = "Please wait for your partner.\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            char *msg = "You can send message to your partner.\n";
            send(client, msg, strlen(msg), 0);
        }

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
    char buf[256];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        buf[ret] = 0;
        printf("Received from %d: %s\n", client, buf);

        if (num_clients % 2 == 1 && clients[num_clients - 1] == client)
        {
            char *msg = "Please wait for your partner.\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            int i;
            for (i = 0; i < num_clients; i++)
                if (clients[i] == client)
                    break;
            
            int partner = i % 2 == 1 ? clients[i - 1] : clients[i + 1];
            send(partner, buf, strlen(buf), 0);
        }
    }

    close(client);
}