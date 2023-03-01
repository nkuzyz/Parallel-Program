//
// Created by 张哈哈 on 2022/12/4.
//
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <ctime>
#include <sys/time.h> // for gettimeofday()

using namespace std;

const int ARR_NUM=5000;
const int ARR_LEN=1000;
const int THREAD_NUM=3;
const int TASK_NUM=ARR_NUM/THREAD_NUM;
const int seg = ARR_NUM / THREAD_NUM;
vector<int> arr[ARR_NUM];
struct timeval tpstart,tpend;

//正常情况下的初始化待排序数组
void init_1(void){
    srand(unsigned(time(nullptr)));
    for (int i = 0; i < ARR_NUM; i++) {
        arr[i].resize(ARR_LEN);
        for (int j = 0; j < ARR_LEN; j++){
            arr[i][j] = rand();
        }
    }
}

// 初始化待排序数组，使得
// 第一段：完全升序
// 第二段：1/4逆序，3/4升序
// 第三段：1/2逆序，1/2升序
// 第四段：完全逆序
void init_2(void){
    int ratio;
    srand(unsigned(time(nullptr)));
    for (int i = 0; i < ARR_NUM; i++){
        arr[i].resize(ARR_LEN);
        if(i < seg)ratio = 0;
        else if(i < seg * 2)ratio = 32;
        else if(i < seg * 3)ratio = 64;
        else ratio = 128;
        if((rand() & 127) < ratio){
            for(int j = 0; j < ARR_LEN; j++){
                arr[i][j] = ARR_LEN - j;
            }
        }
        else{
            for(int j = 0; j < ARR_LEN; j++){
                arr[i][j] = j;
            }
        }
    }
}

int main()
{


    //1:串行方法
    init_2(); //初始化数组
    gettimeofday(&tpstart,NULL);
    for(int i=0;i<ARR_NUM;i++){
        stable_sort(arr[i].begin(),arr[i].end());
    }
    gettimeofday(&tpend,NULL);
//    cout<<"serial: "<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;
    //2:omp静态块划分
    init_2(); //初始化数组
    gettimeofday(&tpstart,NULL);
#pragma omp parallel for num_threads(THREAD_NUM)
    for(int i=0;i<ARR_NUM;i++){
        stable_sort(arr[i].begin(),arr[i].end());
    }
    gettimeofday(&tpend,NULL);
//    cout<<"omp static: "<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    //3:omp动态块划分（一次划分一个任务，细粒度）
    init_2(); //初始化数组
    gettimeofday(&tpstart,NULL);
#pragma omp parallel for num_threads(THREAD_NUM)\
    schedule(dynamic,1)
    for(int i=0;i<ARR_NUM;i++){
        stable_sort(arr[i].begin(),arr[i].end());
    }
    gettimeofday(&tpend,NULL);
//    cout<<"omp dynamic(TASK_NUM=1): "<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    //4:omp动态块划分（一次划分TASK_NUM个任务，粗粒度）
    init_2(); //初始化数组
    gettimeofday(&tpstart,NULL);
#pragma omp parallel for num_threads(THREAD_NUM)\
    schedule(dynamic,TASK_NUM)
    for(int i=0;i<ARR_NUM;i++){
        stable_sort(arr[i].begin(),arr[i].end());
    }
    gettimeofday(&tpend,NULL);
//    cout<<"omp dynamic(TASK_NUM): "<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    //5:omp使用guided调度
    init_2(); //初始化数组
    gettimeofday(&tpstart,NULL);
#pragma omp parallel for num_threads(THREAD_NUM)\
    schedule(guided)
    for(int i=0;i<ARR_NUM;i++){
        stable_sort(arr[i].begin(),arr[i].end());
    }
    gettimeofday(&tpend,NULL);
//    cout<<"omp guided dynamic: "<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

}
