#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_THREADS 5

typedef struct{
    int threadId;
}threadParm_t;

int sum=0;
sem_t counter;
sem_t barrier;

// 利用信号量实现
void *threadFuncSemaphore(void *parm) {
    threadParm_t *p = (threadParm_t *) parm;
    fprintf(stdout, "Thread %d has entered step 1.\n", p->threadId);
    sem_wait(&counter);
    if(sum==NUM_THREADS-1){
        for(int i=0;i<NUM_THREADS;i++){
            sem_post(&barrier);
        }
        sem_post(&counter);
    }
    else{
        sum++;
        sem_post(&counter);
    }
    sem_wait(&barrier);
    fprintf(stdout, "Thread %d has entered step 2.\n", p->threadId);
    pthread_exit(NULL);
}

// 利用信号量实现
int main(int argc, char *argv[]){
    pthread_t thread[NUM_THREADS];
    threadParm_t threadParm[NUM_THREADS];
    sem_init(&counter, 0, 1);
    sem_init(&barrier, 0, 0);
    int i;
    for (i=0; i<NUM_THREADS; i++){
        threadParm[i].threadId = i;
        pthread_create(&thread[i], NULL, threadFuncSemaphore, (void *)&threadParm[i]);
    }
    for (i=0; i<NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    sem_destroy(&counter);
    sem_destroy(&barrier);
    return 0;
}
