#include <stdio.h>
#include <pthread.h>

#define NLOOP 100
int counter;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *doit(void *);

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, doit, NULL);
    pthread_create(&t2, NULL, doit, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}

void *doit(void *param)
{
    for (int i = 0; i < NLOOP; i++)
    {
        pthread_mutex_lock(&counter_mutex);
        int val = counter;
        printf("Thread %ld: %d\n", pthread_self(), val + 1);
        counter = val + 1;
        pthread_mutex_unlock(&counter_mutex);
    }
}