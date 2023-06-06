#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_proc(void *);

int main()
{
    pthread_t thread_id1;
    int param = 10;
    if (pthread_create(&thread_id1, NULL, thread_proc, &param))
    {
        printf("pthread_create() failed.\n");
        return 1;
    }

    pthread_join(thread_id1, NULL);
    printf("Main thread done.\n");
    
    return 0;
}

void *thread_proc(void *param)
{
    int *pn = (int *)param;

    printf("Child thread %ld started.\n", pthread_self());
    for (int i = 0; i < *pn; i++)
    {
        printf("Child thread %ld running.\n", pthread_self());
        sleep(1);
    }
    printf("Child thread %ld done.\n", pthread_self());
    return NULL;
}