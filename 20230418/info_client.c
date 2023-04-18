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

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }
        
    // Nhap du lieu va tao buffer chua du lieu can gui
    char buf[256];
    int pos = 0;

    char computer_name[64];
    printf("Nhap ten may tinh: ");
    scanf("%s", computer_name);

    strcpy(buf, computer_name);
    pos += strlen(computer_name);
    buf[pos] = 0;
    pos++;

    int num_drives;
    printf("Nhap so o dia: ");
    scanf("%d", &num_drives);
    getchar();

    char drive_letter;
    unsigned short drive_size;

    for (int i = 0; i < num_drives; i++)
    {
        printf("Nhap ky tu: ");
        scanf("%c", &drive_letter);

        buf[pos] = drive_letter;
        pos++;

        printf("Nhap kich thuoc: ");
        scanf("%hd", &drive_size);
        getchar();

        memcpy(buf + pos, &drive_size, sizeof(drive_size));
        pos += sizeof(drive_size);
    }

    printf("Buffer size: %d\n", pos);

    send(client, buf, pos, 0);

    // Ket thuc, dong socket
    close(client);

    return 0;
}