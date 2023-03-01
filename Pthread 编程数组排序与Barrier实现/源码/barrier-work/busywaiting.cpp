#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 5

typedef struct{
    int threadId;
}threadParm_t;

int flag = 0;

// 利用忙等待实现
void *threadFuncBusywaiting(void *parm) {
    threadParm_t *p = (threadParm_t *) parm;
    fprintf(stdout, "Thread %d has entered step 1.\n", p->threadId);
    while(flag!=p->threadId)sleep(0);
    flag++;
    while(flag!=NUM_THREADS)sleep(0);
    fprintf(stdout, "Thread %d has entered step 2.\n", p->threadId);
    pthread_exit(NULL);
}

// 利用忙等待实现
int main(int argc, char *argv[]){
    pthread_t thread[NUM_THREADS];
    threadParm_t threadParm[NUM_THREADS];
    int i;
    for (i=0; i<NUM_THREADS; i++){
        threadParm[i].threadId = i;
        pthread_create(&thread[i], NULL, threadFuncBusywaiting, (void *)&threadParm[i]);
    }
    for (i=0; i<NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    return 0;
}
