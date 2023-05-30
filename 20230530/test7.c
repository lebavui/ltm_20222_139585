#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
}

int main()
{
    signal(SIGCHLD, signalHandler);
    if (fork() == 0)
    {
        printf("Child process %d started\n", getpid());
        sleep(10);
        printf("Child process %d done\n", getpid());
        exit(0);
    }

    // Main process
    getchar();
    printf("Main process done.\n");
    return 0;
}