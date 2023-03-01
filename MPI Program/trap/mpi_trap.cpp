#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <mpi.h>

//待积分的f(x)
double f(double x) {
    return x*x;
}

//计算局部和
double Trap(double a, double b, int count, double h) {
    double my_result, x;
    int i;
    my_result = (f(a) + f(b)) / 2.0;
    //局部求和，避免过多通信
    for (i = 1; i < count; i++) {
        x = a + i * h;
        my_result += f(x);
    }
    my_result *= h;
    return my_result;
}

//主函数
int main()
{
    int my_rank, comm_sz, name_len, n = 1000;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double a = 0, b = 5000;
    double h = (b - a) / n;
    double global_result;
    //clock_t start, end;
  	//计时
		struct timeval tpstart,tpend;
    double time;

    MPI_Init(NULL, NULL);
    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    // Get the name of the processor
    MPI_Get_processor_name(processor_name, &name_len);
    // Print off a hello world message
    printf("Trap Caculation from processor %s, rank %d out of %d processors\n",
    processor_name, my_rank, comm_sz);

    //0号进程开始计时
    if (my_rank == 0) {
        //start = clock();
      gettimeofday(&tpstart,NULL);
    }
    int local_n = n / comm_sz;
    int local_a = a + my_rank * local_n * h;
    int local_b = local_a + local_n * h;
    //将未除尽的任务数分配给最后一个进程
    if (my_rank == comm_sz - 1) {
        local_n = n - (comm_sz - 1) * local_n;
        local_b = b;
    }
    //每个进程计算自身分配的任务的局部和
    double local_result = Trap(local_a, local_b, local_n, h);
    //其他进程将局部和发送给0号进程
    if (my_rank != 0) {
        MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    //0号进程收集局部和并求全局和
    else {
        global_result = local_result;
        for (int source = 1; source < comm_sz; source++)
        {
            MPI_Recv(&local_result, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_result += local_result;
        }
        //完成计算，0号进程停止计时
        //end = clock();
        //time = (end - start) / CLOCKS_PER_SEC;
      gettimeofday(&tpend,NULL);
    	time=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;

    }
    //0号进程打印出结果
    if (my_rank == 0) {
        printf("划分成小梯形的块数: %d\n", n);
        printf("计算结果是: %.15e\n", global_result);
        printf("总共耗时: %f\n\nms", time);
    }

    MPI_Finalize();
    return 0;
}
