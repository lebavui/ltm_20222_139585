#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    // http://api.weatherapi.com/v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo *info;
    int ret = getaddrinfo("api.weatherapi.com", "http", NULL, &info);
    if (ret != 0 || info == NULL)
    {
        printf("Khong phan giai duoc ten mien.\n");
        exit(1);
    }

    if (connect(client, info->ai_addr, info->ai_addrlen)) {
        perror("connect() failed");
        return 1;
    }
        
    // Gui lenh GET len server
    char *request = "GET /v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no HTTP/1.1\r\nHost: api.weatherapi.com\r\n\r\n";
    send(client, request, strlen(request), 0);

    // Nhan phan hoi tu server
    char buf[2048];
    int len = recv(client, buf, sizeof(buf), 0);
    buf[len] = 0;

    puts(buf);

    char *pos1 = strstr(buf, "temp_c");
    pos1 = strchr(pos1, ':') + 1;
    char *pos2 = strchr(pos1, ',');

    int value_length = pos2 - pos1;
    char value_str[64];
    memcpy(value_str, pos1, value_length);
    value_str[value_length] = 0;
    double temp = atof(value_str);

    printf("Temp: %f\n", temp);

    // Ket thuc, dong socket
    close(client);

    return 0;
}