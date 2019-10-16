#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "/home/pavel/CLionProjects/untitled2/FW_1.3.1_Lin64/fwBase.h"
#include "/home/pavel/CLionProjects/untitled2/FW_1.3.1_Lin64/fwSignal.h"


int main(int argc, char *argv[]) {

    int N;
    int K = 1;
    int A = 315;
    struct timeval T1, T2;
    long delta_ms;


    double E = M_E;
    N = atoi(argv[1]);

    if (argc > 2) {
        K = atoi(argv[2]);
    }
    fwSetNumThreads(K);

    gettimeofday(&T1, NULL);
    for (unsigned int i = 0; i < 50; i++) {
        srand(i); /*инициализировать начальное значение ГСЧ*/
        /*Заполнить массив исходных данных размером NxN*/
        double *M2_prep = malloc(sizeof(double)* N / 2)
        double *M1 = malloc(sizeof(double)*N);
        double *M2 = malloc(sizeof(double)* N / 2);


        double s;
//        var 6
        unsigned int seed = i;
        for (int k = 0; k < N; k++) {
            M1[k] = 1. + rand_r(&seed) / (RAND_MAX / (A - 1.));

        }
//        var 2
        for (int j = 0; j < N / 2; j++) {
            M2[j] = A + rand_r(&seed) / (RAND_MAX / (10. * A - A));

        }

        /* Решить поставленную задачу, заполнить массив с результатами*/
//        var 4
        //Корень кубический
        fwsDivC_64f_I(E, M1, N);
        fwsCbrt_64f_A50(M1, M1, N);

        //M2[0] = fabs(cos(M2[0]));
        fwsCos_64f_A50(&M2[0], &M2[0], 1);
        fwsAbs_64f(&M2[0], &M2[0], 1);
        /* for (int j = 1; j < N / 2; j++) {
             M2[j] = fabs(cos(M2[j] + M2[j - 1]));
         }*/
        for (int j = 1; j < N / 2; j++) {
            M2_prep[j] = M2[j] + M2[j - 1];
            fwsCos_64f_A50(&M2_prep[j], &M2[j], 1);
            fwsAbs_64f(&M2[j], &M2[j], 1);
        }


        for (int j = 0; j < N / 2; j++) {
            M2[j] = M1[j] >= M2[j] ? M1[j] : M2[j];
        }

        /*Отсортировать массив с результатами указанным методом*/
//       var 6
        int k, j;
        double key;
        for (k = 1; k < N / 2; k++) {
            key = M2[k];
            j = k - 1;
            while (j >= 0 && M2[j] > key) {
                M2[j + 1] = M2[j];
                j = j - 1;
            }
            M2[j + 1] = key;
        }
//      reduce
        double X = 0.;
        for (int k = 0; k < N / 2; k++) {
            int int_part = M2[k] / M2[0];
            if (!(int_part % 2)) {
                X += sin(M2[k]);
            }
        }
        printf("%lf\n", X);

    }
    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN = %d. Milliseconds passed: %1ld\n\n", N, delta_ms);
    return 0;
}
