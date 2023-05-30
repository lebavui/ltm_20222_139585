#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid = getpid();
    printf("Before FORK, PID = %d\n", pid);

    for (int i = 0; i < 5; i++)
        if (fork() == 0)
        {
            printf("Child process.\n");
            // Thuc hien cong viec trong tien trinh con
            exit(0);
        }
    
    printf("Main process.\n");
    
    return 0;
}