#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Sai tham so\n");
        return 1;
    }

    int n = atoi(argv[1]);
    for (int i = 1; i <= n; i++)
        printf("%d\n", i);

    return 0;
}
