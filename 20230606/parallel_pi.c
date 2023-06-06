#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 8

static long num_steps = 1000000000; 
double step;
double local_sum[NUM_THREADS];

void *thread_proc(void *);

int main()
{
    step = 1.0 / (double)num_steps;
    for (int i = 0; i < NUM_THREADS; i++)
        local_sum[i] = 0;
    
    pthread_t thread_ids[NUM_THREADS];
    int params[NUM_THREADS];

    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        params[i] = i;
        pthread_create(&thread_ids[i], NULL, thread_proc, &params[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(thread_ids[i], NULL);

    double sum = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        sum = sum + local_sum[i];

    clock_gettime(CLOCK_REALTIME, &stop);

    double etime = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;
    printf("Elapsed time: %f us\n", etime);

    double pi = sum * step;
    printf("PI = %.10f\n", pi);

    return 0;
}

void *thread_proc(void *param)
{
    int idx = *(int *)param;

    for (int i = idx; i < num_steps; i += NUM_THREADS)
    {
        double x = (i + 0.5) * step;
        local_sum[idx] += 4.0 / (1.0 + x * x);
    }
    
    return NULL;
}