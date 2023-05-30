#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid = getpid();
    printf("Before FORK, PID = %d\n", pid);
    int ret = fork();
    if (ret == 0)
    {
        printf("Child process.\n");
    }
    else
    {
        printf("Main process.\n");
    }
    pid = getpid();
    printf("After FORK, ret = %d, PID = %d\n", ret, pid);
    return 0;
}