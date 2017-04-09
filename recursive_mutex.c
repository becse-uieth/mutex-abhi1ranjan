#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
pthread_mutex_init(&mutex, &attr);

struct 
{
        int a;
        int b;
        int c;
} A;

void *f()
{
        pthread_mutex_lock(&mutex);
        A.a++;
        g();
        A.c = 0;
        pthread_mutex_unlock(&mutex);
}

void *g()
{
        pthread_mutex_lock(&mutex);
        A.b += A.a;
        pthread_mutex_unlock(&mutex);
}