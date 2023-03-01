#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>
#include <algorithm>
#include <sys/time.h>

using namespace std;

struct timeval tpstart,tpend;
typedef struct{
    double a;
    double b;
    int n;
    double* global_result_p;
    int thread_count;
    int my_rank;
    pthread_mutex_t* amutex;
}pthread_param;

void trap(double a,double b,int n,double*global_result_p);
void pthread_trap(double a,double b,int n,double*global_result_p,int thread_count);
void* pthread_trap(void* param);

double f(double x){
    return x*x;
}

int main(int argc, char* argv[]){
    double global_result=0.0;
    double a=0,b=5000;
    int n=5000;
    int thread_count=10.0;

    //1:最普通的OpenMP版本
    cout<<"Using OpenMP"<<endl;
    gettimeofday(&tpstart,NULL);
#pragma omp parallel num_threads(thread_count)
    trap(a,b,n,&global_result);
    gettimeofday(&tpend,NULL);
    //time=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
    cout<<"cost time: "<<(1000000.0*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000.0<<"ms"<<endl;

    //2:pthread编程
    global_result=0.0;
    cout<<"Using Pthread"<<endl;
    gettimeofday(&tpstart,NULL);
    pthread_trap(a,b,n,&global_result,thread_count);
    gettimeofday(&tpend,NULL);
    //time=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
    cout<<"cost time: "<<(1000000.0*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000.0<<"ms"<<endl;
    return 0;
}

//每个线程执行的求和程序
void trap(double a,double b,int n,double*global_result_p){
    double h,x,my_result;
    double local_a,local_b;
    int local_n;
    int my_rank=omp_get_thread_num();
    int thread_count=omp_get_num_threads();
    h=(b-a)/n;
    local_n=n/thread_count;
    local_a=a+my_rank*local_n*h;
    local_b=local_a+local_n*h;
    my_result=(f(local_a)+f(local_b))/2.0;
    //先局部求和，避免过多通信
    for(int i=1;i<=local_n-1;i++){
        x=local_a+i*h;
        my_result+=f(x);
    }
    my_result=my_result*h;
    //用临界区保证全局求和正确性
#pragma omp critical
    *global_result_p+=my_result;
}
//pthread编程线程控制程序
void pthread_trap(double a,double b,int n,double*global_result_p,int thread_count){
    pthread_param params[thread_count];
    pthread_t threads[thread_count];
    pthread_mutex_t amutex;
    pthread_mutex_init(&amutex,NULL);
    for(int i=0;i<thread_count;i++){
        //为每个线程参数赋值
        params[i].a=a;
        params[i].b=b;
        params[i].n=n;
        params[i].global_result_p=global_result_p;
        params[i].thread_count=thread_count;
        params[i].my_rank=i;
        params[i].amutex=&amutex;
        //创建线程
        pthread_create(&threads[i],NULL,pthread_trap,(void*)&params[i]);
    }
    //销毁线程
    for(int i=0;i<thread_count;i++){
        pthread_join(threads[i],NULL);
    }
    //销毁互斥量锁
    pthread_mutex_destroy(&amutex);
}
//pthread中每个线程调用的程序
void* pthread_trap(void* p){
    //恢复获取参数
    pthread_param* params=(pthread_param*) p;
    double h,x,my_result;
    double local_a,local_b;
    int local_n;
    int my_rank=params->my_rank;
    int thread_count=params->thread_count;
    h=(params->b-params->a)/params->n;
    local_n=params->n/thread_count;
    local_a=params->a+my_rank*local_n*h;
    local_b=local_a+local_n*h;
    my_result=(f(local_a)+f(local_b))/2.0;
    //先局部求和，避免过多通信
    for(int i=1;i<=local_n-1;i++){
        x=local_a+i*h;
        my_result+=f(x);
    }
    my_result=my_result*h;
    //使用互斥量 amutex 上锁保证全局求和正确性
    pthread_mutex_lock(params->amutex);
    *params->global_result_p+=my_result;
    pthread_mutex_unlock(params->amutex);
}
