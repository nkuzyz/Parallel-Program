//
// Created by 张哈哈 on 2022/11/18.
//


#include <iostream>
#include <pmmintrin.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/time.h> // for gettimeofday()
#include <time.h>
#include <cstdlib>
#include <pthread.h>

#include <vector>
#include <immintrin.h>

using namespace std;

struct timeval tpstart,tpend;
// 实现高斯消去法解线性方程组的Pthread多线程编程，可与SSE/AVX编程结合，并探索优化任务分配方法。

class Matrix;

// 规定矩阵最大规模
const int maxN = 1000;
// 测试矩阵的数量
const int maxCount = 50;
// 线程数
const int THREAD_NUM = 20;
// 矩阵实际规模
int n = 500;
// 动态划分线程任务
int nextTask = 1;
// 分配任务时上锁
pthread_mutex_t mutex_task;
pthread_mutex_t amutex;
// 一个线程一次分配处理的列数
const int columnGauss = 15;
const int columnBack = 15;
// 线程同步障碍
pthread_barrier_t barrier;

int flag;

typedef struct{
    int threadId;
} threadParm_t;

class Matrix{
public:
    float a[maxN][maxN];
    float atemp[maxN][maxN+1];
    float x[maxN];
    float b[maxN];
};

Matrix* dataset = new Matrix[maxCount];

// 初始化待测试矩阵
void init() {
    srand(static_cast <unsigned> (time(0)));
    for (int count = 0; count < maxCount; count++) {
        for (int j = 0; j < n; j++) {
            dataset[count].x[j] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / 1000));
            for (int k = 0; k < n; k++) {
                dataset[count].a[j][k] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (1000 - 1)));
                dataset[count].atemp[j][k] = dataset[count].a[j][k];
            }
        }
        for (int j = 0; j < n; j++) {
            dataset[count].b[j] = 0;
            for (int k = 0; k < n; k++) {
                dataset[count].b[j] += dataset[count].a[j][k] * dataset[count].x[k];
            }
            dataset[count].atemp[j][n]=dataset[count].b[j];
        }
    }
}


// 消元过程pthread编程
void* pthread_elimination(void* parm){
    threadParm_t* p=(threadParm_t*) parm;
    int startJ;
    int endJ;
    for(int k=0; k<n; k++){
        pthread_mutex_lock(&amutex);
        if(k>=n){
            pthread_mutex_unlock(&amutex);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&amutex);
        pthread_barrier_wait(&barrier);
        if(nextTask!=1)nextTask=1;
        pthread_barrier_wait(&barrier);
        while(1){
            pthread_mutex_lock(&mutex_task);
            startJ=k+nextTask;
            nextTask=nextTask+columnGauss;
            endJ=k+nextTask;
            pthread_mutex_unlock(&mutex_task);
            if(startJ>=n)break;
            for(int j=startJ; j<endJ; j++){
                for(int count=0; count<maxCount; count++){
                    dataset[count].atemp[k][j]=dataset[count].atemp[k][j]/dataset[count].atemp[k][k];
                    for(int i=k+1; i<n; i++){
                        dataset[count].atemp[i][j]=dataset[count].atemp[i][j]-dataset[count].atemp[i][k]*dataset[count].atemp[k][j];
                    }
                }
            }
        }
    }
}

// sse结合pthread消元过程
void* pthread_sse_elimination(void* parm){
    threadParm_t* p=(threadParm_t*) parm;
    int startJ;
    int endJ;
    for(int k=0; k<n; k++){
        pthread_mutex_lock(&amutex);
        if(k>=n){
            pthread_mutex_unlock(&amutex);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&amutex);
        pthread_barrier_wait(&barrier);
        if(nextTask!=1)nextTask=1;
        pthread_barrier_wait(&barrier);
        while(1){
            pthread_mutex_lock(&mutex_task);
            startJ=k+nextTask;
            nextTask=nextTask+columnGauss;
            endJ=k+nextTask;
            pthread_mutex_unlock(&mutex_task);
            if(startJ>=n+1)break;
            for(int j=startJ; j<endJ; j++){
                for(int count=0; count<maxCount; count++){
                    dataset[count].atemp[k][j]=dataset[count].atemp[k][j]/dataset[count].atemp[k][k];
                    // 先处理可以sse并行处理的部分
                    __m128 t1, t2, t3, t4;
                    int i;
                    for(i=k+1; i<n-3; i+=4){
                        float tmp1[4] = {dataset[count].atemp[k][j], dataset[count].atemp[k][j], dataset[count].atemp[k][j], dataset[count].atemp[k][j]};
                        t1 = _mm_loadu_ps(tmp1);
                        float tmp2[4] = {dataset[count].a[i][k], dataset[count].a[i+1][k], dataset[count].a[i+2][k], dataset[count].a[i+3][k]};
                        t2 = _mm_loadu_ps(tmp2);
                        float tmp3[4] = {dataset[count].a[i][j], dataset[count].a[i+1][j], dataset[count].a[i+2][j], dataset[count].a[i+3][j]};
                        t3 = _mm_loadu_ps(tmp3);
                        t4 = _mm_sub_ps(t3, _mm_mul_ps(t1, t2));
                        _mm_storeu_ps(tmp1, t4);
                        dataset[count].a[i][j]=tmp1[0];
                        dataset[count].a[i+1][j]=tmp1[1];
                        dataset[count].a[i+2][j]=tmp1[2];
                        dataset[count].a[i+3][j]=tmp1[3];
                    }
                    // 再处理不能被4整除的部分
                    for(; i<n; i++){
                        dataset[count].a[i][j]=dataset[count].a[i][j]-dataset[count].a[i][k]*dataset[count].atemp[k][j];
                    }
                    if(endJ>=n+1){
                        for(i=k+1; i<n; i++){
                            dataset[count].b[i]=dataset[count].b[i]-dataset[count].a[i][k]*(dataset[count].b[i]/dataset[count].a[k][k]);
                        }
                    }
                }
            }
        }
    }
}

// 转置矩阵
void transpose(){
    for(int count=0; count<maxCount; count++){
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                float temp=dataset[count].a[i][j];
                dataset[count].a[i][j]=dataset[count].a[j][i];
                dataset[count].a[j][i]=temp;
            }
        }
    }
}

// sse结合pthread回代过程
void* pthread_sse_back(void* parm){
    threadParm_t* p=(threadParm_t*) parm;
    int startJ;
    int endJ;
    for(int count=0; count<maxCount; count++){
        for(int k=n-1; k>=0; k--){
            // 等待所有线程完成列计算，再进入下一个k
            pthread_barrier_wait(&barrier);
            if(flag!=1)flag=1;
            if(nextTask!=0)nextTask=0;
            pthread_mutex_lock(&amutex);
            if(k<0)pthread_exit(NULL);
            if(flag==1){
                dataset[count].x[k]=dataset[count].b[k]/dataset[count].a[k][k];
                flag=0;
            }
            pthread_mutex_unlock(&amutex);
            pthread_barrier_wait(&barrier);
            while(1){
                // 动态获取任务
                pthread_mutex_lock(&mutex_task);
                startJ=nextTask;
                endJ=nextTask+columnBack;
                nextTask=endJ;
                if(endJ>=k)endJ=k;
                pthread_mutex_unlock(&mutex_task);

                // 完成任务则退出循环进入下一个k
                if(startJ>=k)break;
                // 回代过程
                int j=startJ;
                // 先处理可以sse并行处理的部分
                __m128 t1, t2, t3, t4;
                for(; j<endJ-3; j+=4){
                    float tmp[4]={dataset[count].x[k],dataset[count].x[k],dataset[count].x[k],dataset[count].x[k]};
                    t1=_mm_loadu_ps(tmp);
                    t2=_mm_loadu_ps(dataset[count].a[k]+j);
                    t3=_mm_loadu_ps(dataset[count].b+j);
                    _mm_storeu_ps(tmp, _mm_sub_ps(t3, _mm_mul_ps(t1,t2)));
                    dataset[count].b[j]=tmp[0];
                    dataset[count].b[j+1]=tmp[1];
                    dataset[count].b[j+2]=tmp[2];
                    dataset[count].b[j+3]=tmp[3];
                }
                // 再处理不能被4整除的部分
                for(; j<endJ; j++){
                    dataset[count].b[j]=dataset[count].b[j]-dataset[count].x[k]*dataset[count].a[k][j];
                }
            }
        }
    }
}

void elimination(void){
    // 计时器
    double timeuse;
    pthread_t thread[THREAD_NUM];
    threadParm_t threadParm[THREAD_NUM];
    //消元过程
    gettimeofday(&tpstart,NULL);
    for(int i=0; i<THREAD_NUM; i++){
        threadParm[i].threadId=i;
        pthread_create(&thread[i], nullptr, pthread_elimination, (void*)&threadParm[i]);
    }
    for(int i=0; i<THREAD_NUM; i++){
        pthread_join(thread[i], NULL);
    }
    for(int count=0; count<maxCount; count++){
        for(int i=1; i<n; i++){
            for(int j=0; j<i; j++){
                dataset[count].a[i][j]=0;
            }
        }
    }
    gettimeofday(&tpend,NULL);

    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;//注意，秒的读数和微秒的读数都应计算在内
    cout<<"pthread 消元过程:"<<timeuse<<"ms"<<endl;
}

void sse_elimination(void){
    // 计时器
    double timeuse;
    pthread_t thread[THREAD_NUM];
    threadParm_t threadParm[THREAD_NUM];
    //消元过程
    gettimeofday(&tpstart,NULL);
    for(int i=0; i<THREAD_NUM; i++){
        threadParm[i].threadId=i;
        pthread_create(&thread[i], nullptr, pthread_sse_elimination, (void*)&threadParm[i]);
    }
    for(int i=0; i<THREAD_NUM; i++){
        pthread_join(thread[i], NULL);
    }
    for(int count=0; count<maxCount; count++){
        for(int i=1; i<n; i++){
            for(int j=0; j<i; j++){
                dataset[count].a[i][j]=0;
            }
        }
    }
    gettimeofday(&tpend,NULL);

    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;//注意，秒的读数和微秒的读数都应计算在内
    // cout<<"pthread+sse 消元过程:"<<timeuse<<"ms"<<endl;
    cout<<timeuse<<endl;
}
void sse_back(void){
    // 计时器
    double timeuse;
    pthread_t thread[THREAD_NUM];
    threadParm_t threadParm[THREAD_NUM];
    gettimeofday(&tpstart,NULL);
    transpose();
    for(int i=0; i<THREAD_NUM; i++){
        threadParm[i].threadId=i;
        pthread_create(&thread[i], nullptr, pthread_sse_back, (void*)&threadParm[i]);
    }
    transpose();
    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;//注意，秒的读数和微秒的读数都应计算在内

    // cout<<"pthread+sse 回代过程:"<<timeuse<<"ms"<<endl;
    cout<<timeuse<<endl;

}

int main(){

    init();
    pthread_barrier_init(&barrier, NULL, THREAD_NUM);
    sse_elimination();
    sse_back();



    //回代求解过程

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex_task);
}
