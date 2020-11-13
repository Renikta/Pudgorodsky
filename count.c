#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

long long count = 0;
long long algorithm = 0;
long long num_pth = 0;
long long num_count = 0;

enum algorithms {
    A = 1,      //dumb
    B = 2,      //flaive
    C = 3,      //mide
    D = 4

};

void* counter_A(void* pointer);
void* counter_B(void* pointer);
void* counter_C(void* pointer);
void* counter_D(void* pointer);

int main(int argc, char* argv[])
{
    num_count = atoi(argv[1]);  //count
    num_pth = atoi(argv[2]);  //pthread
    algorithm = atoi(argv[3]);
    void* (*function) (void*);

    switch (algorithm)
    {
        case A:
            function = &counter_A;
            break;
        case B:
            function = &counter_B;
            break;
        case C:
            function = &counter_C;
            break;
        case D:
            function = &counter_D;
            break;
        default:
            printf("Can't be so!\n");
            break;
    }

    struct timeval start, finish;
    struct timezone tz;

    pthread_t* pthread = (pthread_t*) calloc((size_t) num_pth, sizeof(pthread[0]));
    pthread_mutex_t pmt;
    int ret_value = 0;
    void* pointer;


    pthread_mutex_init(&pmt, NULL);

    gettimeofday(&start, &tz);

    for(long long i = 0; i <= num_pth; i++)
    {
        errno = 0;
        ret_value = 0;
        ret_value = pthread_create(pthread + i, NULL, function, &pmt);
        if (ret_value != 0)
        {
            perror("Error: pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    for(long long j = 0; j < num_pth; j++)
    {
        errno = 0;
        ret_value = 0;
        ret_value = pthread_join(pthread[j], NULL);
        if (ret_value != 0) {
            perror("Error: pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&finish, &tz);
    printf("time: %f\n", finish.tv_sec + 1e-6 * finish.tv_usec - start.tv_sec - 1e-6 * start.tv_usec);

    printf("Count = %lld\n", count);
    free(pthread);
    return 0;
}

void* counter_A(void* pointer)
{
    for(long long i; i < num_count/num_pth; i++)
        count++;
    return NULL;
}

void* counter_B(void* pointer)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*) pointer;

    for(long long i; i < num_count/num_pth; i++)
    {
        errno = 0;
        if(pthread_mutex_lock(pmt) != 0)
        {
            perror("Error: pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }

        count++;

        errno = 0;
        if(pthread_mutex_unlock(pmt) != 0)
        {
            perror("Error: pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}
void* counter_C(void* pointer)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*) pointer;

    errno = 0;
    if(pthread_mutex_lock(pmt) != 0)
    {
        perror("Error: pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    for(long long i; i < num_count/num_pth; i++)
        count++;
    pthread_mutex_unlock(pmt);

    return NULL;
}

void* counter_D(void* pointer)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*) pointer;

    long long another_count = 0;
    for(long long i; i < num_count/num_pth; i++)
        another_count++;

    errno = 0;
    if(pthread_mutex_lock(pmt) != 0)
    {
        perror("Error: pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }
    count += another_count;
    
    pthread_mutex_unlock(pmt);

    return NULL;
}
