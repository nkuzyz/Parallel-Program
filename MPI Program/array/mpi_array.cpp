#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <sys/time.h> // for gettimeofday()
using namespace std;

//数组数量
const int ARR_NUM = 1000;
//每个数组的长度
const int ARR_LEN = 300;
//进程数量/主机数量
const int PROCESS_NUM = 3;
//每个进程处理的数组数量
const int seg = 50;
vector<int> arr[ARR_NUM];

struct timeval tpstart,tpend;

// 初始化待排序数组，使得
// 第一段：完全升序
// 第二段：1/4逆序，3/4升序
// 第三段：1/2逆序，1/2升序
// 第四段：完全逆序
void init(void){
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

void doTask(int begin) {
    for (int i = begin; i < min(begin + seg, ARR_NUM); ++i) {
        sort(arr[i].begin(), arr[i].end());
    }
}

//主函数
int main() {
    int my_rank, comm_sz,name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    //初始化数组
    init();

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    // Get the name of the processor
    MPI_Get_processor_name(processor_name, &name_len);
    // Print off a hello world message
    printf("From processor %s, rank %d out of %d processors\n",
    processor_name, my_rank, comm_sz);

    MPI_Status status;
    bool done = false;
    int current_task = 0;//当前的任务
    int ready;
    double time = 0;

    gettimeofday(&tpstart,NULL);
    //其他进程将本进程的运行时间发送给0号进程
    if (my_rank != 0) {
         while (!done) {
            int begin;
            MPI_Send(&ready, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&begin, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == 1) {
                break;
            }
            doTask(begin);
        }
    }
    //0号进程打印各个进程的运行时间
    if (my_rank == 0) {
         while (current_task < ARR_NUM) {
            //接受任何一个线程的状态
            MPI_Recv(&ready, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(&current_task, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            current_task += seg;
        }
        done = true;
        printf("数组数量：%d;数组长度:%d \n",ARR_NUM,ARR_LEN);
        cout << "all work done!!!!!" << endl;
        //唤醒所有线程
        for (int i = 1; i < comm_sz; ++i) {
            MPI_Recv(&ready, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            MPI_Send(&current_task, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        }
        gettimeofday(&tpend,NULL);
        time += (1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;
        cout <<" time: " << time << "ms" << endl;
        
    }
    

    MPI_Finalize();

    

    return 0;
}