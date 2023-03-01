#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <pthread.h>

using namespace std;

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
double local_trap(double a,double b,int n);
void pthread_trap(double a,double b,int n,double*global_result_p,int thread_count);
void* pthread_trap(void* param);

double f(double x){
    return (x*x*x);
}

int main(int argc, char* argv[]){
    double global_result=0.0;
    double a,b;
    int n;
    int thread_count=10.0;

    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

//    //1:最普通的OpenMP版本
//    cout<<"Using Common OpenMP"<<endl;
//#pragma omp parallel num_threads(thread_count)
//    trap(a,b,n,&global_result);


//    //2:使用递归的OpenMP版本
//    cout<<"Using recurrence OpenMP"<<endl;
//    #pragma omp parallel num_threads(thread_count)
//    {
//        double my_result=0.0;
//        my_result+=local_trap(a,b,n);
//        #pragma omp critical
//        global_result+=my_result;
//    }
//
//    //3:使用归约简化的递归OpenMP版本
//    cout<<"Using reduction OpenMP"<<endl;
//    #pragma omp parallel num_threads(thread_count)\
//        reduction(+: global_result)
//    global_result+=local_trap(a,b,n);

    //4:pthread编程
    global_result=0.0;
    cout<<"Using Pthread"<<endl;
    pthread_trap(a,b,n,&global_result,thread_count);


    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);

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
//返回局部和
double local_trap(double a,double b,int n){
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
    //局部求和，避免过多通信
    for(int i=1;i<=local_n-1;i++){
        x=local_a+i*h;
        my_result+=f(x);
    }
    my_result=my_result*h;
    return my_result;
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
