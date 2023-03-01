#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 5

typedef struct{
    int threadId;
}threadParm_t;

int flag = 0;
pthread_mutex_t amutex;
pthread_cond_t cond;

// 利用互斥量(条件变量)实现
void *threadFuncMutex(void *parm) {
    threadParm_t *p = (threadParm_t *) parm;
    fprintf(stdout, "Thread %d has entered step 1.\n", p->threadId);
    pthread_mutex_lock(&amutex);
    flag++;
    if(flag==NUM_THREADS){
        pthread_cond_broadcast(&cond);
    }
    else{
        while(pthread_cond_wait(&cond, &amutex)!=0);
    }
    pthread_mutex_unlock(&amutex);
    fprintf(stdout, "Thread %d has entered step 2.\n", p->threadId);
    pthread_exit(NULL);
}

// 利用互斥量(条件变量)实现
int main(int argc, char *argv[]){
    pthread_t thread[NUM_THREADS];
    threadParm_t threadParm[NUM_THREADS];
    pthread_mutex_init(&amutex, NULL);
    pthread_cond_init(&cond, NULL);
    int i;
    for (i=0; i<NUM_THREADS; i++){
        threadParm[i].threadId = i;
        pthread_create(&thread[i], NULL, threadFuncMutex, (void *)&threadParm[i]);
    }
    for (i=0; i<NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_mutex_destroy(&amutex);
    pthread_cond_destroy(&cond);
    return 0;
}
