#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

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

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }
    printf("New client connected: %d\n", client);

    // Truyen nhan du lieu
    char buf[256];
    char mssv[10];
    char hoten[64];
    char ns[12];
    float dtb;

    FILE *f = fopen("sv_log.txt", "a");

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        // Xu ly du lieu nhan duoc
        buf[ret] = 0;

        // Tach cac thong tin tu xau ky tu
        memcpy(mssv, buf, 8);
        mssv[8] = 0;

        int hoten_len = ret - 25;
        memcpy(hoten, buf + 9, hoten_len);
        hoten[hoten_len] = 0;

        memcpy(ns, buf + hoten_len + 10, 10);
        ns[10] = 0;

        dtb = atof(buf + hoten_len + 21);

        printf("MSSV: %s\nHo ten: %s\nNgay sinh: %s\nDiem TB: %.2f\n", 
            mssv, hoten, ns, dtb);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        fprintf(f, "%s %d-%d-%d %d:%d:%d %s\n",
            inet_ntoa(clientAddr.sin_addr),
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            buf);
    }

    fclose(f);

    close(client);  
    close(listener);    

    return 0;
}