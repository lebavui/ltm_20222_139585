#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int cid = fork();
    if (cid == 0)
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
    printf("Main process done.\n");
    kill(cid, SIGKILL);
    return 0;
}