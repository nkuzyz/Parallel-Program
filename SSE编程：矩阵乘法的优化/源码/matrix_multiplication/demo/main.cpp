#include <iostream>
#include <stdio.h>
#include <pmmintrin.h>
#include <stdlib.h>
#include <algorithm>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

const int maxN=1024;
const int T=64;
long long head, tail, freq;
int n;
float c[maxN][maxN];

class Matrix {
public:
    float a[maxN][maxN];
    float b[maxN][maxN];
};

// 随机生成矩阵
void init(Matrix* dataset) {
    srand(static_cast <unsigned> (time(0)));
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                dataset[i].a[j][k] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / 1000));
                dataset[i].b[j][k] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / 1000));
            }
        }
    }
}

// serial 串行
void serial(float a[maxN][maxN], float b[maxN][maxN]){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            c[i][j] = 0.0;
            for (int k = 0; k < n; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// cache 缓存优化
void cache(float a[maxN][maxN], float b[maxN][maxN]){
    // transposition
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            c[i][j] = 0.0;
            for (int k = 0; k < n; ++k) {
                c[i][j] += a[i][k] * b[j][k];
            }
        }
    }

    // transposition
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }
}

// SSE SSE编程
void sse(float a[maxN][maxN], float b[maxN][maxN]){
    __m128 t1, t2, sum;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            c[i][j] = 0.0;
            sum = _mm_setzero_ps();
            for (int k = n - 4; k >= 0; k -= 4){
                // sum every 4 elements
                t1 = _mm_loadu_ps(a[i] + k);
                t2 = _mm_loadu_ps(b[j] + k);
                t1 = _mm_mul_ps(t1, t2);
                sum = _mm_add_ps(sum, t1);
            }
            sum = _mm_hadd_ps(sum, sum);
            sum = _mm_hadd_ps(sum, sum);
            _mm_store_ss(c[i] + j, sum);
            for (int k = (n % 4) - 1; k >= 0; --k){
                // handle the last n%4 elements
                c[i][j] += a[i][k] * b[j][k];
            }
        }
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }
}

// tile 分片策略
void tile(float a[maxN][maxN], float b[maxN][maxN]){
    __m128 t1, t2, sum;
    float t;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }

    for (int r = 0; r < n / T; ++r)
        for (int q = 0; q < n / T; ++q){
            for (int i = 0; i < T; ++i)
                for (int j = 0; j < T; ++j){
                    c[r * T + i][q * T + j] = 0.0;
                }
            for (int p = 0; p < n / T; ++p){
                for (int i = 0; i < T; ++i)
                    for (int j = 0; j < T; ++j){
                        sum = _mm_setzero_ps();
                        for (int k = 0; k < T; k += 4){
                            //sum every 4th elements
                            t1 = _mm_loadu_ps(a[r * T + i] + p * T + k);
                            t2 = _mm_loadu_ps(b[q * T + j] + p * T + k);
                            t1 = _mm_mul_ps(t1, t2);
                            sum = _mm_add_ps(sum, t1);
                        }
                        sum = _mm_hadd_ps(sum, sum);
                        sum = _mm_hadd_ps(sum, sum);
                        _mm_store_ss(&t, sum);
                        c[r * T + i][q * T + j] += t;
                    }
            }
        }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j){
            float t=b[i][j];
            b[i][j]=b[j][i];
            b[j][i]=t;
        }
}

int main()
{
    double time[4][21];
    Matrix* dataset=new Matrix[50];

    for(n=0;n<=1000;n+=50){
        init(dataset);
        cout<<"------------------------------------"<<endl;
        cout <<"问题规模:" << n << endl;
        cout <<"四种算法执行时间如下:" << endl;
        QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

        QueryPerformanceCounter((LARGE_INTEGER *)&head);
        for(int i=0;i<50;i++){
            serial(dataset[i].a,dataset[i].b);
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&tail);
        time[0][n/50]=((tail-head)*1000.0/freq)/50;
        cout<<time[0][n/50]<<endl;

        QueryPerformanceCounter((LARGE_INTEGER *)&head);
        for(int i=0;i<50;i++){
            cache(dataset[i].a,dataset[i].b);
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&tail);
        time[1][n/50]=((tail-head)*1000.0/freq)/50;
        cout<<time[1][n/50]<<endl;

        QueryPerformanceCounter((LARGE_INTEGER *)&head);
        for(int i=0;i<50;i++){
            sse(dataset[i].a,dataset[i].b);
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&tail);
        time[2][n/50]=((tail-head)*1000.0/freq)/50;
        cout<<time[2][n/50]<<endl;

        QueryPerformanceCounter((LARGE_INTEGER *)&head);
        for(int i=0;i<50;i++){
            tile(dataset[i].a,dataset[i].b);
        }
        QueryPerformanceCounter((LARGE_INTEGER *)&tail);
        time[3][n/50]=((tail-head)*1000.0/freq)/50;
        cout<<time[3][n/50]<<endl;
        cout<<"------------------------------------"<<endl;
    }
    cout<<"四种算法在各规模下时间汇总:"<<endl;
    for(int i=0;i<4;i++){
        cout<<"------------------------------------"<<endl;
        for(int j=0;j<21;j++){
            cout<<time[i][j]<<endl;
        }
    }
    return 0;
}
