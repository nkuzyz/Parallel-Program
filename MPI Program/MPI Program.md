<div class="cover" style="page-break-after:always;font-family:方正公文仿宋;width:100%;height:100%;border:none;margin: 0 auto;text-align:center;">
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:20%;">
        </br>
        <img src="https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/%E5%8D%97%E5%BC%80%E5%A4%A7%E5%AD%A6logo.jpeg" alt="校名" style="width:80%;"/>
    </div>
    </br></br></br></br></br>
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:30%;">
        <img src="https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/%E5%8D%97%E5%BC%80%E5%A4%A7%E5%AD%A6%E6%A0%A1%E5%BE%BDlogo.jpg" alt="校徽" style="width:50%;"/>
	</div>
    </br></br></br></br></br></br></br></br>
    <span style="font-family:华文黑体Bold;text-align:center;font-size:20pt;margin: 10pt auto;line-height:30pt;">《MPI 编程练习实验报告》</span>
    <p style="text-align:center;font-size:14pt;margin: 0 auto">并行第05次作业 </p>
    </br>
    </br>
    <table style="border:none;text-align:center;width:72%;font-family:仿宋;font-size:14px; margin: 0 auto;">
    <tbody style="font-family:方正公文仿宋;font-size:12pt;">
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">题　　目</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> MPI 编程练习实验报告</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">上课时间</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 周一下午</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">授课教师</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">孙永谦 </td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">姓　　名</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 张怡桢</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">学　　号</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2013747 </td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">年　　级</td>
    		<td style="width:%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 2020级本科生</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">日　　期</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2022/12/14</td>     </tr>
    </tbody>              
    </table>
</div>




<!-- 注释语句：导出PDF时会在这里分页 -->

# MPI 编程练习实验报告



<center><div style='height:2mm;'></div><div style="font-family:华文楷体;font-size:14pt;">张怡桢，2013747</div></center>
<center><span style="font-family:华文楷体;font-size:9pt;line-height:9mm">南开大学软件学院</span>
</center>
<div>
<div style="width:52px;float:left; font-family:方正公文黑体;">摘　要：</div> 
<div style="overflow:hidden; font-family:华文楷体;">  Massage Passing Interface:是消息传递函数库的标准规范，由MPI论坛开发。
定义：1. 一种新的库描述，不是一种语言。共有上百个函数调用接口，提供与C和Fortran语言的绑定。
2. MPI是一种标准或规范的代表，而不是特指某一个对它的具体实现。
3. MPI是一种消息传递编程模型，并成为这种编程模型的代表和事实上的标准。
  </div>
</div>

<div>
<div style="width:52px;float:left; font-family:方正公文黑体;">关键词：</div> 
<div style="overflow:hidden; font-family:华文楷体;">MPI</div>
</div>




## 实验内容

1. 实现第5章课件中的梯形积分法的MPI编程熟悉并掌握MPI编程方法，规模自行设定，可探讨不同规模对不同实现方式的影响。
2. 对于课件中“多个数组排序”的任务不均衡案例进行MPI编程实现，规模可自己设定、调整。
3.  附加：实现高斯消去法解线性方程组的MPI编程，规模自己设定。





## 实验环境

MPI实验在华为鲲鹏云服务器三台上实现

OpenMP以及Pthread使用M1芯片的MacOS实现



## 梯形积分法

### 实验内容

参见ppt上的内容

对于下图中给出的梯形积分法，实现并行编程。 每个线程或进程计算 a 到 b 区间中的某一段的梯形面积，最后求取全局和得到结果。



<img src="https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221204130708072.png" alt="image-20221204130708072" style="zoom: 33%;" />

本部分实现第5章课件中的梯形积分法的MPI编程，并设置不同的数据规模进行对照实验，同时讨论不同规模对不同实现方式（包含MPI，Pthread 和 OpenMP）的影响。

### 算法设计

#### MPI实现

实验共用到了 3 台主机的进程进行计算。每个进程根据自己的 my_rank 进程号得到自身的计算任务，完成局部和的计算后，（除 0 号进程外）使用 MPI_Send 将结果发送至 0 号进程； 0 号进程使用 MPI_Recv 阻塞式地接受其他进程传回的结果，并计算全局和。同时，0 号进程 还完成计时、输出结果的任务。具体的实现代码如下：

`mph_trap.cpp`

```c++
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

```

`makefile`

makefile文件进行编译

```makefile
EXECS=mpi_trap
MPICC?=mpicc
all: ${EXECS}
mpi_trap: mpi_trap.cpp
	${MPICC} -o mpi_trap mpi_trap.cpp
clean:
	rm -f ${EXECS}

```

`config`

给每台主机分配了2个进程（在 config 文件中设置），总共有6个进程共同执行计算任务。

```config
ecs-zyz-0001:2
ecs-zyz-0002:2
ecs-zyz-0003:2
```

`执行语句`

```bash
mpiexec -n 6 -f /home/zhangyizhen/trap/config /home/zhangyizhen/trap/mpi_trap
```

#### Pthread实现

对于每个线程，先求局部和最后全局求和，避免过多通信。同时注意给全局和加锁保证结果的正确性。

```cpp
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

```

其中每个线程执行的函数如下：

```cpp
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

```



#### OpenMP实现

```cpp
//1:最普通的OpenMP版本
    cout<<"Using OpenMP"<<endl;
    gettimeofday(&tpstart,NULL);
#pragma omp parallel num_threads(thread_count)
    trap(a,b,n,&global_result);
    gettimeofday(&tpend,NULL);
    time=(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000;
```



```cpp
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
```



#### OpenMP与Pthread在MacOS上的makefile配置

```makefile
cmake_minimum_required(VERSION 3.23)
project(pthread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#set(CMAKE_C_COMPILER "/usr/bin/gcc") #这里写你的gcc路径
#set(CMAKE_CXX_COMPILER "/usr/bin/g++") #这里写你的g++路径
set(OPENMP_LIBRARIES "/opt/homebrew/Cellar/libomp/15.0.4/lib") #这里写你的libomp路径, 通过brew安装的默认地址
set(OPENMP_INCLUDES "/opt/homebrew/Cellar/libomp/15.0.4/include")#这里写你的libomp路径

OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)

# Find OpenMP
if(APPLE AND USE_OpenMP)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
        #注意以上需要增加-Xpreprocessor 以及不能直接-llibomp 在这里不需要前缀lib只需要-lomp即可，下面相似的地方也是同个道理
        set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
        set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

if(USE_OpenMP)
    find_package(OpenMP REQUIRED)
endif(USE_OpenMP)


if (OPENMP_FOUND)
    include_directories("${OPENMP_INCLUDES}")
    link_directories("${OPENMP_LIBRARIES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

add_executable(pthread trap.cpp)

```





### 实验设计与数据分析

实验计算的梯形面积为——函数:f(x)=x*x，a=0，b=5000的面积部分。

通过调整梯形积分 法划分成小梯形的个数规模，来比较不同编程方式的异同。

下面对三种编程模式设置规模n=100，1000，2000，5000这几个参数来实现计算并计算时间进行对比。

#### mpi实现

`规模：n=100`

![image-20221218145906542](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218145906542.png)

`规模：n=1000`

![image-20221214174125916](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221214174125916.png)

`规模：n=2000`

![image-20221218150218474](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218150218474.png)

`规模：n=5000`

![image-20221218150423144](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218150423144.png)



#### Pthread实现

`规模：n=100`

![image-20221218200931299](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218200931299.png)

`规模：n=1000`

![image-20221218200950175](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218200950175.png)

`规模：n=2000`

![image-20221218201016430](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218201016430.png)

`规模：n=5000`

![image-20221218201115062](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218201115062.png)

#### OpenMP实现

`规模：n=100`

![image-20221218200922621](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218200922621.png)

`规模：n=1000`

![image-20221218201000184](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218201000184.png)

`规模：n=2000`

![image-20221218201027086](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218201027086.png)

`规模：n=5000`

![image-20221218201103343](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218201103343.png)

#### 对比分析

| 规模 | MPI  | OpenMP | Pthread |
| ---- | ---- | ------ | ------- |
| 100  | 5    | 0.475  | 0.235   |
| 1000 | 10   | 0.26   | 0.319   |
| 2000 | 8    | 0.248  | 0.347   |
| 5000 | 4    | 0.329  | 0.33    |

![image-20221218202733474](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218202733474.png)

根据上述图表可以看出，几种实现方式的性能差异较为明显。

可能是因为OpenMP以及Pthread使用的是本地的电脑完成，所以处理速度快，效率高；

而MPI 方法的执行时间出现较长的现象，MPI使用远程服务器以及三个服务器之间的通信完成，存在网络延迟问题。

可见，在网络较差的情况下，使用MPI算法，并不能得到更大的优势。

并且耗时并没有随着梯形划分规模n的增大而增大，推测应该是数据规模太小了，程序多线程处理以及网络延时的耗时影响完全覆盖了规模问题。

### 实验出现的问题与解决办法

#### 计时问题

**精度太低**

`使用clock() 函数       `                 

头文件：<ctime>

clock()函数，返回“自程序启动到调用该函数，CPU时钟的计时单元数（clock tick）”

每过1ms，计数值+1

精度：1毫秒

```cpp
start = clock(); ......

end = clock(); time = (end - start) / CLOCKS_PER_SEC;
```

![image-20221214172425685](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221214172425685.png)

在实验过程中出现耗时为0的情况，该计时函数精度太低！！！

**解决办法**

`使用gettimeofday（）函数`

头文件：<sys/time.h>

精度：1us

```cpp
struct timeval tpstart,tpend;
gettimeofday(&tpstart,NULL);
gettimeofday(&tpend,NULL);
double timeuse = 1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
printf("used time:%fus\n",timeuse);
```

该精度可以满足实验的耗时要求。



### 实验总结

通过本实验，我熟悉并掌握 MPI 编程方法，在探讨不同规模对不同实现方式的影响，了解了多台主机共同完成同一计算任务的思想。本实验中 MPI耗时比Pthread、OpenMP方法的性能差异较为明显，这应该是设备性能问题以及网络延时问题带来的差异。



## 多个数组排序（MPI）

### 实验内容

对于课件中“多个数组排序”的任务不均衡案例进行MPI编程实现，规模可自己设定、调整。

对 ARR_NUM 个长度为 ARR_LEN 的一维数组进行排序，使用MPI编程。

### 算法设计

#### 数组不均衡初始化

初始化 ARR_NUM 行，ARR_LEN 列的数组。 

当行数 i 属于 0~2499 时, ratio = 0，此时 2500 行内完全升序排列；当

行数 i 属于 2500~4999 时, ratio = 32 此时随机有 2500 * 1/4 行升序， 2500 * 3/4 行降序;

当 行数 i 属于 5000~7499 时，ratio = 64，此时随机有 2500 * 1/2 行升序，2500*1/2 行降序；

当行数 i 属于 7500~9999 时，ratio = 128，此时 2500 行完全降序。此时数组可分为四块，数组负载不均衡。



```c++
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
```

#### mpi代码实现

实验共用到了 3 台主机的进程完成数组排序。根据进程号连续划分任务，每个进程完成排序之后，将自己执行任务的耗时传送给 0 号进程，最终 0 号进程接受各个进程完成排序任务的 时间并输出。具体的实现代码如下：

```cpp
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
```



#### 执行设置

`makefile`

makefile文件进行编译

```makefile
EXECS=mpi_array
MPICC?=mpicc
all: ${EXECS}
mpi_array: mpi_array.cpp
	${MPICC} -o mpi_array mpi_array.cpp -lstdc++
clean:
	rm -f ${EXECS}

```

`config`

给每台主机分配了2个进程（在 config 文件中设置），总共有6个进程共同执行计算任务。

```config
ecs-zyz-0001:2
ecs-zyz-0002:2
ecs-zyz-0003:2
```

`执行语句`

```bash
mpiexec -n 6 -f /home/zhangyizhen/array/config /home/zhangyizhen/array/mpi_array
```

### 实验设计与数据分析
1. 使用 init 函数生成分布不均的数组； 
2. 使用与实验一（梯形积分法）中相同的计时方法。 
3. 改变数组规模，分别处理 1000，1500，2000，2500，3000 个长度为 300 的数组， 比较在不同规模下各个进程的时间差异，从而理解任务负载不均衡对效率的影响。

`ARR_NUM=1000`

![image-20221218191541396](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218191541396.png)

`ARR_NUM=1500`

![image-20221218191439288](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218191439288.png)

`ARR_NUM=2000`

![image-20221218191259128](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218191259128.png)

`ARR_NUM=2500`

![image-20221218191155806](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218191155806.png)

`ARR_NUM=3000`

![image-20221218191100618](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218191100618.png)

### 实验总结

| arr_num | 耗时/ms |
| ------- | ------- |
| 1000    | 7       |
| 1500    | 11      |
| 2000    | 19      |
| 2500    | 20      |
| 3000    | 26      |

![image-20221218205915871](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218205915871.png)

由上面的数据可以看到，随着ARR_NUM的增大，MPI排序算法需要的用时也增大了，符合我的预期。

通过以上的实验，我对于array排序在多线程，多进程中的实现有了更深的了解，同时对于多服务器实现MPI进行了掌握。



## 高斯消去（MPI）

### 实验内容

附加：实现高斯消去法解线性方程组的MPI编程。

对于给定的线性方程组 Ax=b，使用 MPI 编程，完成对于线性方程组的高斯消元。



### 算法设计

#### mpi实现

每个进程完成部分行的消元计算任务，然后使用 barrier 功能，使得所有进程均完成第 k 行 的校园后再计算下一行的消元。程序实现的具体代码如下：

```cpp
#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <sys/time.h>
#include <algorithm>


const int n = 1024;//固定矩阵规模，控制变量
const int maxN = n + 1; // 矩阵的最大值
float a[maxN][maxN];
float temp[maxN][maxN];//用于暂时存储a数组中的变量，控制变量唯一
int next_task = 0;
int seg;
int line = 0;//记录当前所依赖的行数
struct timeval startTime, stopTime;// timers


/**
 * 根据第i行的元素，消除j行的元素
 * @param i 根据的行数
 * @param j 要消元的行数
 */
void OMP_elimination(int i, int j) {
    //求出相差倍数
    float temp = a[j][i] / a[i][i];
    //遍历这一行的所有值，将i后面的数值依次减去相对应的值乘以倍数
    for (int k = i + 1; k <= n; ++k) {
        a[j][k] -= a[i][k] * temp;
    }
    //第i个为0
    a[j][i] = 0.00;
}


//用于矩阵改变数值,为防止数据溢出,随机数的区间为100以内的浮点数
void change() {
    srand((unsigned) time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            a[i][j] = (float) (rand() % 10000) / 100.00;
        }
    }
}


int main(int argc, char *argv[]) {
    change();
    int rank, thread_num;
    gettimeofday(&startTime, NULL);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &thread_num);
    MPI_Status status;
    int ready;
    bool done = false;
    if (rank == 0) {
        printf("size : %d\n", n);
        for (line = 0; line < n - 1; ++line) {
            next_task = line + 1;
            seg = (n - next_task) / (thread_num - 1) + 1;
            for (int i = 1; i < thread_num; i++) {
                int task = (i - 1) * seg + next_task;
                MPI_Send(&task, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            //等待所有线程
            for (int j = 1; j < thread_num; ++j) {
                MPI_Recv(&ready, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            }
        }
        printf("all work done!!!!!\n");
        done = true;
        gettimeofday(&stopTime, NULL);
        double trans_mul_time =
                (stopTime.tv_sec - startTime.tv_sec) * 1000 + (stopTime.tv_usec - startTime.tv_usec) * 0.001;
        printf("time: %lf ms\n", trans_mul_time);
    } else {
        while (!done) {
            int task;
            MPI_Recv(&task, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            int min = task + seg < n ? task + seg : n;
            for (int i = task; i < min; ++i) {
                OMP_elimination(line, i);
            }
            MPI_Send(&ready, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}

```



#### 执行设置

`makefile`

makefile文件进行编译

```makefile
EXECS=mpi_gause
MPICC?=mpicc
all: ${EXECS}
mpi_gause: mpi_gause.cpp
	${MPICC} -o mpi_gause mpi_gause.cpp -lstdc++
clean:
	rm -f ${EXECS}

```

`config`

给每台主机分配了2个进程（在 config 文件中设置），总共有6个进程共同执行计算任务。

```config
ecs-zyz-0001:2
ecs-zyz-0002:2
ecs-zyz-0003:2
```

`执行语句`

```bash
mpiexec -n 6 -f /home/zhangyizhen/gause/config /home/zhangyizhen/gause/mpi_gause
```



### 实验设计与数据分析

改变矩阵规模，分别处理矩阵规模 n=512，1024，2048的高斯消去， 比较在不同规模下各个进程的时间差异，从而理解任务负载不均衡对效率的影响。

![image-20221218193902540](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218193902540.png)

![image-20221218193615331](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218193615331.png)

![image-20221218193804345](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218193804345.png)

### 实验总结

size从512到1024，再到2048，随着矩阵规模的增大，高斯消去的用时也随着增加，符合正常的实验预期。

通过这次实验，我对于高斯消去以及MPI的用法更加了解熟悉了。



## 注意

每次重启华为云服务器后，在配置文件中均会出现一行新的主机配置， 需要将其注释掉，不然会报如下错。

![image-20221218145719124](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218145719124.png)

**解决方法**

![img](data:image/png;base64,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)

![image-20221218145703469](https://zyzstc-1303973796.cos.ap-beijing.myqcloud.com/uPic/image-20221218145703469.png)
