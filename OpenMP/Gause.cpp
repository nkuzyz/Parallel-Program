//
// Created by 张哈哈 on 2022/12/1.
//
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sys/time.h>
#include <pmmintrin.h>
#include <omp.h>


using namespace std;

//规定矩阵最大规模
const int maxN=2048;
//矩阵实际规模
int n=6;
//线程数 等于1时表示串行实现
const int THREAD_NUM=10;
//线程分配任务粗细粒度
const int TASK_NUM=10;
//计时
struct timeval tpstart,tpend;

class parameters{
public:
    float a[maxN][maxN];  //未消元的原始增广矩阵
    float b[maxN][maxN];  //消元后得到的上三角增广矩阵
    float x[maxN];  //线性方程组的解
    //打印原始增广矩阵
    void printA(){
        cout<<"待求解的增广矩阵:"<<endl;
        for(int i=0;i<n;i++){
            for(int j=0;j<n+1;j++){
                cout<<setw(10)<<setfill(' ')<<a[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    //打印消元后得到的上三角矩阵
    void printB(){
        cout<<"完成消元后得到的上三角矩阵:"<<endl;
        for(int i=0;i<n;i++){
            for(int j=0;j<n+1;j++){
                cout<<setw(10)<<setfill(' ')<<b[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    //打印方程组的解
    void printX(){
        cout<<"方程组的解:"<<endl;
        for(int i=0;i<n;i++){
            cout<<setw(10)<<setfill(' ')<<x[i]<<" ";
        }
        cout<<endl<<endl;
    }
};

//初始化矩阵
void init(parameters* p);
//消元
void omp_sse_elimination_static(parameters* p);  //静态分配
void omp_sse_elimination_dynamic(parameters* p,int task_size);  //动态分配
void omp_sse_elimination_guided(parameters* p);  //guided分配
//回代求解
void omp_sse_back(parameters* p);

int main()
{


    gettimeofday(&tpstart,NULL);
    parameters* p=new parameters();
    init(p);
    p->printA();
    omp_sse_elimination_static(p);
    p->printB();
    omp_sse_back(p);
    p->printX();
    gettimeofday(&tpend,NULL);
    cout<<"执行时间为:"<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;


//以下注释部分为实验测试获取数据的代码
/*
    parameters* p=new parameters[50];
    for(int i=0;i<50;i++)init(&p[i]);
    gettimeofday(&tpstart,NULL);
    for(int m=0;m<50;m++){
        //静态分配消元
        omp_sse_elimination_static(p);
        omp_sse_back(p);
    }
    gettimeofday(&tpend,NULL);
    //cout<<"static 执行时间为:"<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    for(int i=0;i<50;i++)init(&p[i]);
    gettimeofday(&tpstart,NULL);
    for(int m=0;m<50;m++){
        //动态分配消元（细粒度）
        omp_sse_elimination_dynamic(p,1);
        omp_sse_back(p);
    }
    gettimeofday(&tpend,NULL);
    //cout<<"dynamic(细粒度) 执行时间为:"<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    for(int i=0;i<50;i++)init(&p[i]);
    gettimeofday(&tpstart,NULL);
    for(int m=0;m<50;m++){
        //动态分配消元（粗粒度）
        omp_sse_elimination_dynamic(p,TASK_NUM);
        omp_sse_back(p);
    }
    gettimeofday(&tpend,NULL);
    //cout<<"dynamic(粗粒度) 执行时间为:"<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;

    for(int i=0;i<50;i++)init(&p[i]);
    gettimeofday(&tpstart,NULL);
    for(int m=0;m<50;m++){
        //guided分配
        omp_sse_elimination_guided(p);
        omp_sse_back(p);
    }
    gettimeofday(&tpend,NULL);
    //cout<<"guided 执行时间为:"<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<" ms"<<endl;
    cout<<(1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec)/1000<<endl;
*/
}

//初始化矩阵
void init(parameters* p){
    srand(static_cast <unsigned> (time(0)));
    for(int i=0;i<n;i++){
        for(int j=0;j<n+1;j++){
            p->a[i][j]=static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (1000 - 1)));
            p->b[i][j]=p->a[i][j];
        }
    }
}
//静态分配任务消元
void omp_sse_elimination_static(parameters* p){
    __m128 t1,t2,t3,t4;
    float tmp;
    for(int k=0;k<n-1;k++){
        int i,j;
# pragma omp parallel for num_threads(THREAD_NUM) \
            schedule(static) \
                default(none) shared(p,k,n) private(i,j,tmp,t1,t2,t3,t4)
        for(i=k+1;i<n+1;i++){
            tmp=p->b[i][k]/p->b[k][k];
            t3=_mm_load1_ps(&tmp);
            for(j=n+1-4;j>=k;j-=4){
                t1=_mm_loadu_ps(p->b[i]+j);
                t2=_mm_loadu_ps(p->b[k]+j);
                t3=_mm_mul_ps(t2,t3);
                t4=_mm_sub_ps(t1,t3);
                _mm_storeu_ps(p->b[i]+j,t4);
            }
            //解决不能被4整除的部分
            for (j=((n-k+1)%4)+k-1;j>= k;--j) {
                p->b[i][j] =p->b[i][j]-tmp*p->b[k][j];
            }
            p->b[i][k]=0;
        }
    }
}
//动态分配任务消元
void omp_sse_elimination_dynamic(parameters* p,int task_size){
    __m128 t1,t2,t3,t4;
    float tmp;
    for(int k=0;k<n-1;k++){
        int i,j;
# pragma omp parallel for num_threads(THREAD_NUM) \
            schedule(dynamic,task_size) \
                default(none) shared(p,k,n,task_size) private(i,j,tmp,t1,t2,t3,t4)
        for(i=k+1;i<n+1;i++){
            tmp=p->b[i][k]/p->b[k][k];
            t3=_mm_load1_ps(&tmp);
            for(j=n+1-4;j>=k;j-=4){
                t1=_mm_loadu_ps(p->b[i]+j);
                t2=_mm_loadu_ps(p->b[k]+j);
                t3=_mm_mul_ps(t2,t3);
                t4=_mm_sub_ps(t1,t3);
                _mm_storeu_ps(p->b[i]+j,t4);
            }
            //解决不能被4整除的部分
            for (j=((n-k+1)%4)+k-1;j>= k;--j) {
                p->b[i][j] =p->b[i][j]-tmp*p->b[k][j];
            }
            p->b[i][k]=0;
        }
    }
}
//guided分配任务消元
void omp_sse_elimination_guided(parameters* p){
    __m128 t1,t2,t3,t4;
    float tmp;
    for(int k=0;k<n-1;k++){
        int i,j;
# pragma omp parallel for num_threads(THREAD_NUM) \
            schedule(guided) \
                default(none) shared(p,k,n) private(i,j,tmp,t1,t2,t3,t4)
        for(i=k+1;i<n+1;i++){
            tmp=p->b[i][k]/p->b[k][k];
            t3=_mm_load1_ps(&tmp);
            for(j=n+1-4;j>=k;j-=4){
                t1=_mm_loadu_ps(p->b[i]+j);
                t2=_mm_loadu_ps(p->b[k]+j);
                t3=_mm_mul_ps(t2,t3);
                t4=_mm_sub_ps(t1,t3);
                _mm_storeu_ps(p->b[i]+j,t4);
            }
            //解决不能被4整除的部分
            for (j=((n-k+1)%4)+k-1;j>= k;--j) {
                p->b[i][j] =p->b[i][j]-tmp*p->b[k][j];
            }
            p->b[i][k]=0;
        }
    }
}
//回代求解
void omp_sse_back(parameters* p) {
    __m128 t1, t2, sumSSE;
    float sum;
    //先处理后四个解，以使sse并行计算能够启动
    for(int i=n-1;i>=0;i--){
        if((n-1-i)==4)break;
        float sum = 0;
        for(int j=n-1;j>i;j--){
            sum+=p->b[i][j]*p->x[j];
        }
        p->x[i]=(p->b[i][n]-sum)/p->b[i][i];
    }
    //OpenMP结合SSE计算
    for(int i=n-5;i>=0;i--){
        sumSSE=_mm_setzero_ps();
        sum=0;
        int forSerial=(n-i-1)%4;
        int j;
# pragma omp parallel for num_threads(THREAD_NUM) \
            schedule(dynamic,TASK_NUM) \
                default(none) shared(p,i,n,sumSSE,forSerial) private(j,t1,t2)
        for(j=i+forSerial+1;j<=n-4;j+=4){
            t1=_mm_loadu_ps(p->b[i]+j);
            t2=_mm_loadu_ps(p->x+j);
            t1=_mm_mul_ps(t1,t2);
            sumSSE=_mm_add_ps(sumSSE,t1);
        }
        sumSSE=_mm_hadd_ps(sumSSE,sumSSE);
        sumSSE=_mm_hadd_ps(sumSSE,sumSSE);
        _mm_store_ss(&sum,sumSSE);
        //解决不能被4整除的部分
        for(int j=i+1;j<=i+forSerial;j++) {
            sum+=p->b[i][j]*p->x[j];
        }
        p->x[i]=(p->b[i][n]-sum)/p->b[i][i];
    }
}

