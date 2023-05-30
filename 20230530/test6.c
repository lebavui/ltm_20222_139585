#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    for (int i = 0; i < 5; i++)
        if (fork() == 0)
        {
            printf("Child process %d started\n", getpid());
            while (1)
            {
                sleep(1);
                printf("Child process %d running\n", getpid());
            }
            printf("Child process %d done\n", getpid());
            exit(0);
        }

    // Main process
    printf("Main process started.\n");
    sleep(5);
    killpg(0, SIGKILL);
    printf("Main process done.\n");
    return 0;
}