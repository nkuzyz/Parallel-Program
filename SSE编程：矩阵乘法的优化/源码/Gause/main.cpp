#include <iostream>
#include <stdio.h>
#include <pmmintrin.h>
#include <stdlib.h>
#include <algorithm>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

const int maxN = 1024;
long long head, tail, freq;
int n;

class Matrix {
public:
    float a[maxN][maxN];
    float atemp[maxN][maxN];
    float x[maxN];
    float b[maxN];
};

void init(Matrix* dataset) {
    srand(static_cast <unsigned> (time(0)));
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < n; j++) {
            dataset[i].x[j] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / 1000));
            for (int k = 0; k < n; k++) {
                dataset[i].a[j][k] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (1000 - 1)));
                dataset[i].atemp[j][k] = dataset[i].a[j][k];
            }
        }
        for (int j = 0; j < n; j++) {
            dataset[i].b[j] = 0;
            for (int k = 0; k < n; k++) {
                dataset[i].b[j] += dataset[i].a[j][k] * dataset[i].x[k];
            }
        }
    }

}

// serial串行消元
void serialGauss(Matrix* dataset) {
    for (int count = 0; count < 50; count++) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                float temp = dataset[count].a[j][i] / dataset[count].a[i][i];
                for (int k = i; k < n; k++) {
                    dataset[count].a[j][k] = dataset[count].a[j][k] - temp * dataset[count].a[i][k];
                }
            }
        }
    }
}

// sse消元
void sseGauss(Matrix* dataset) {
    for (int count = 0; count < 50; count++) {
        __m128 t1, t2, t3, t4;
        for (int k = 0; k < n; k++) {
            float tmp[4] = { dataset[count].a[k][k], dataset[count].a[k][k], dataset[count].a[k][k], dataset[count].a[k][k] };
            t1 = _mm_loadu_ps(tmp);
            for (int j = n - 4; j >= k; j -= 4) {
                t2 = _mm_loadu_ps(dataset[count].a[k] + j);
                t3 = _mm_div_ps(t2, t1);
                _mm_storeu_ps(dataset[count].atemp[k] + j, t3);
            }
            if (k % 4 != (n % 4)) {
                for (int j = k; j % 4 != (n % 4); j++) {
                    dataset[count].atemp[k][j] = dataset[count].a[k][j] / tmp[0];
                }
            }
            for (int j = (n % 4) - 1; j >= 0; j--) {
                dataset[count].atemp[k][j] = dataset[count].a[k][j] / tmp[0];
            }
            for (int i = k + 1; i < n; i++) {
                float tmp[4] = { dataset[count].a[i][k], dataset[count].a[i][k], dataset[count].a[i][k], dataset[count].a[i][k] };
                t1 = _mm_loadu_ps(tmp);
                for (int j = n - 4; j > k; j -= 4) {
                    t2 = _mm_loadu_ps(dataset[count].a[i] + j);
                    t3 = _mm_loadu_ps(dataset[count].atemp[k] + j);
                    t4 = _mm_sub_ps(t2, _mm_mul_ps(t1, t3));
                    _mm_storeu_ps(dataset[count].a[i] + j, t4);
                }
                for (int j = k + 1; j % 4 != (n % 4); j++) {
                    dataset[count].a[i][j] = dataset[count].a[i][j] - dataset[count].a[i][k] * dataset[count].atemp[k][j];
                }
                dataset[count].a[i][k] = 0;
            }
        }
    }
}

// serial串行回代
void serialBack(Matrix* dataset) {
    for (int count = 0; count < 50; count++) {
        for (int i = n - 1; i >= 0; i--) {
            float sum = 0;
            for (int j = n - 1; j > i; j--) {
                sum += dataset[count].a[i][j] * dataset[count].x[j];
            }
            dataset[count].x[i] = (dataset[count].b[i] - sum) / dataset[count].a[i][i];
        }
    }
}

// sse回代
void sseBack(Matrix* dataset) {
    __m128 t1, t2, sumSSE;
    float sum;
    for (int count = 0; count < 50; count++) {
        //先处理后四个解，以使sse并行计算能够启动
        for (int i = n - 1; i >= 0; i--) {
            if ((n - 1 - i) == 4)break;
            float sum = 0;
            for (int j = n - 1; j > i; j--) {
                sum += dataset[count].a[i][j] * dataset[count].x[j];
            }
            dataset[count].x[i] = (dataset[count].b[i] - sum) / dataset[count].a[i][i];
        }
        //向量化计算
        for (int i = n - 5; i >= 0; i--) {
            sumSSE = _mm_setzero_ps();
            sum = 0;
            int forSerial = (n - i - 1) % 4;
            for (int j = i + forSerial + 1; j <= n - 4; j += 4) {
                t1 = _mm_loadu_ps(dataset[count].a[i] + j);
                t2 = _mm_loadu_ps(dataset[count].x + j);
                t1 = _mm_mul_ps(t1, t2);
                sumSSE = _mm_add_ps(sumSSE, t1);
            }
            sumSSE = _mm_hadd_ps(sumSSE, sumSSE);
            sumSSE = _mm_hadd_ps(sumSSE, sumSSE);
            _mm_store_ss(&sum, sumSSE);

            for (int j = i + 1; j <= i + forSerial; j++) {
                sum += dataset[count].a[i][j] * dataset[count].x[j];
            }

            dataset[count].x[i] = (dataset[count].b[i] - sum) / dataset[count].a[i][i];
        }
    }
}

int main()
{
    double time[4][21];
    Matrix* dataset = new Matrix[50];
    int times=3;

    while(times>0){
            cout<<"第"<<4-times<<"次测试"<<endl;

    for (n = 0; n <= 1000; n += 50) {
        init(dataset);

        cout << "------------------------------------" << endl;
        cout <<"问题规模:" << n << endl;
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

        cout <<"两种高斯消元方法运行时间:"<< endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        //普通的高斯方法消元
        serialGauss(dataset);
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        time[0][n/50] = ((tail - head) * 1000.0 / freq)/50;
        cout << time[0][n/50] << " ";

        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        //sse的高斯方法消元
        sseGauss(dataset);
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        time[1][n/50] = ((tail - head) * 1000.0 / freq)/50;
        cout << time[1][n/50] << " "<< endl;

        cout <<"两种回代求解方法运行时间:"<< endl;
        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        //普通的回代求解
        serialBack(dataset);
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        time[2][n/50] = ((tail - head) * 1000.0 / freq)/50;
        cout << time[2][n/50] << " ";

        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        //sse的回代求解
        sseBack(dataset);
        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        time[3][n/50] = ((tail - head) * 1000.0 / freq)/50;
        cout << time[3][n/50] << " "<< endl;

        cout << "------------------------------------" << endl;
    }
    for(int i=0;i<4;i++){
        cout<<"------------------------------------"<<endl;
        for(int j=0;j<21;j++){
            cout<<time[i][j]<<endl;
        }
    }
    times--;
    }

    return 0;
}
