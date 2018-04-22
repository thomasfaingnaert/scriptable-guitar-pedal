#include <pthread.h>
#include <stdio.h>

void* func(void* arg)
{
    while (1) ;
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
}

