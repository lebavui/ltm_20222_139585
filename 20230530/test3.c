#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    if (fork() == 0)
    {
        printf("Child process %d started\n", getpid());
        sleep(10);
        printf("Child process %d done\n", getpid());
        exit(0);
    }

    // Main process
    printf("Waiting for child process finished.\n");
    int cid = wait(NULL);
    printf("Child process %d finished.\n", cid);
    printf("Main process done.\n");
    return 0;
}