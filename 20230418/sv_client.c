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
        
    char mssv[10];
    char hoten[64];
    char ns[12];
    float dtb;
    char buf[256];

    while (1)
    {
        printf("Nhap thong tin sinh vien.\n");
        printf("Nhap MSSV: ");
        scanf("%s", mssv);

        if (strncmp(mssv, "0000", 4) == 0)
            break;

        getchar();

        printf("Nhap ho ten: ");
        fgets(hoten, sizeof(hoten), stdin);

        hoten[strlen(hoten) - 1] = 0;

        printf("Nhap ngay sinh: ");
        scanf("%s", ns);

        getchar();

        printf("Nhap diem TB: ");
        scanf("%f", &dtb);

        getchar();

        sprintf(buf, "%s %s %s %.2f", mssv, hoten, ns, dtb);
        printf("%s\n", buf);

        send(client, buf, strlen(buf), 0);
    }

    // Ket thuc, dong socket
    close(client);

    return 0;
}