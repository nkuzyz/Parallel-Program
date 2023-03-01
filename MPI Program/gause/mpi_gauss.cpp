#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <sys/time.h>
#include <algorithm>


const int n = 512;//�̶������ģ�����Ʊ���
const int maxN = n + 1; // ��������ֵ
float a[maxN][maxN];
float temp[maxN][maxN];//������ʱ�洢a�����еı��������Ʊ���Ψһ
int next_task = 0;
int seg;
int line = 0;//��¼��ǰ������������
struct timeval startTime, stopTime;// timers


/**
 * ���ݵ�i�е�Ԫ�أ�����j�е�Ԫ��
 * @param i ���ݵ�����
 * @param j Ҫ��Ԫ������
 */
void OMP_elimination(int i, int j) {
    //�������
    float temp = a[j][i] / a[i][i];
    //������һ�е�����ֵ����i�������ֵ���μ�ȥ���Ӧ��ֵ���Ա���
    for (int k = i + 1; k <= n; ++k) {
        a[j][k] -= a[i][k] * temp;
    }
    //��i��Ϊ0
    a[j][i] = 0.00;
}


//���ھ���ı���ֵ,Ϊ��ֹ�������,�����������Ϊ100���ڵĸ�����
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
            //�ȴ������߳�
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
