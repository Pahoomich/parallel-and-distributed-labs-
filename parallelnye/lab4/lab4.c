#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int N;
    int A = 315;
    double T1, T2;
    long delta_ms;
    N = atoi(argv[1]);

    T1 =omp_get_wtime();
    for (unsigned int i = 0; i < 50; i++) {
        /*GENERATE*Заполнить массив исходных данных размером NxN*/
        double *M1 = malloc(sizeof(double) * N);
        double *M2 = malloc(sizeof(double) * N / 2);
       // printf("i= %d\n",i );
#pragma omp parallel /*for default(none)  shared(M1, N, A)*/
        {
            unsigned int seed = i + omp_get_thread_num();
         
            #pragma omp parallel for default(none)   shared(M1,seed, N, A)
            for (int k = 0; k < N; k++) {
                M1[k] = 1. + rand_r(&seed) / (RAND_MAX / (A - 1.));
            }

            #pragma omp parallel for default(none) shared(M2,seed, N, A)
             for (int j = 0; j < N / 2; j++) {
            M2[j] = A + rand_r(&seed) / (RAND_MAX / (10. * A - A));

            }
        }

        /* Решить поставленную задачу, заполнить массив с результатами*/
//       MAP M1       var 6
#pragma omp parallel for default(none) shared(M1,N)
        for (int k = 0; k < N; k++) {
            M1[k] = pow(M1[k] / M_E, 1. / 3.);
        }

//      MAP M2        var 2
#pragma omp critical/* parallel for default(none) shared(M2, N)*/
        {
            for (int j = 1; j < N / 2; j++) {
                M2[j] = fabs(cos(M2[j] + M2[j - 1]));
            }
            M2[0] = fabs(cos(M2[0]));
        }

//      Merge         var 4
#pragma omp parallel for default(none) shared(M1, M2, N)
        for (int j = 0; j < N / 2; j++) {
            M2[j] = M1[j] >= M2[j] ? M1[j] : M2[j];
        }

        /*Отсортировать массив с результатами указанным методом*/
//      Sort         var 6
        int j;
        double key;
//#pragma omp parallel for default(none) shared(M2, N) private(key, j)
        for (int k = 1; k < N / 2; k++) {
            key = M2[k];
            j = k - 1;
            while (j >= 0 && M2[j] > key) {
                M2[j + 1] = M2[j];
                j = j - 1;
            }
            M2[j + 1] = key;
        }

      
//      Reduce
        double X = 0.;
        int int_part;
//#pragma omp parallel for default(none) shared(M2, N) private(int_part) reduction(+:X)
        for (int k = 0; k < N / 2; k++) {
            int_part = M2[k] / M2[0];
            if (!(int_part % 2)) {
                X += sin(M2[k]);
            }
        }
        printf("X = %lf\n", X);

    }
    T2 = omp_get_wtime();
    printf("T1= %f\n", T1);
    printf("T2= %f\n", T2);
    delta_ms = (T2 - T1)*1000;
    printf("\nN = %d. Milliseconds passed: %1ld\n\n", N, delta_ms);
    return 0;
}

