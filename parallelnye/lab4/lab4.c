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
        double *M1_pre = malloc(sizeof(double) * N);
        double *M2_pre = malloc(sizeof(double) * N / 2);
        double *M2_1 = malloc(sizeof(double) * N / 4);
        double *M2_2 = malloc(sizeof(double) * N / 4);
       // printf("i= %d\n",i );
        unsigned int seed =i;
           for (int k = 0; k < N; k++) {
            
            M1_pre[k] = rand_r(&seed);
           }
            for (int k = 0; k < N/2; k++) {
            M2_pre[k] = rand_r(&seed);
           }

#pragma omp parallel /*for default(none)  shared(M1, N, A)*/
        {
            srand(i);
            unsigned int seed =i;
            
            #pragma omp parallel for default(none)   shared(M1,M1_pre,seed, N, A)
            for (int k = 0; k < N; k++) {
                M1[k] = 1. + M1_pre[k]  / (RAND_MAX / (A - 1.));
            }

            #pragma omp parallel for default(none) shared(M2,M2_pre,seed, N, A)
             for (int j = 0; j < N / 2; j++) {
            M2[j] = A + M2_pre[j] / (RAND_MAX / (10. * A - A));

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
       
        double key;
//#pragma omp parallel for default(none) shared(M2, N) private(key, j)
        int i=0, j=0;
        #pragma omp parallel sections
        {    
            #pragma omp section
            {
                for (int k = 0; k <N/4; k++)
                {
                    M2_1[k] = M2[k];
                    M2_2[k] = M2[k+N/4];
                }
            }

            #pragma omp section
            {
                for (int k = 1; k < N / 4; k++) {
                    key = M2_1[k];
                    j = k - 1;
                    while (j >= 0 && M2_1[j] > key) {
                        M2_1[j + 1] = M2_1[j];
                        j = j - 1;
                    }
                    M2_1[j + 1] = key;
                }
            }

         

            #pragma omp section
            {
                for (int k = 1; k < N / 4; k++) {
                    key = M2_2[k];
                    j = k - 1;
                    while (j >= 0 && M2_2[j] > key) {
                        M2_2[j + 1] = M2_2[j];
                        j = j - 1;
                    }
                    M2_2[j + 1] = key;
                }
            }

            #pragma omp section
            {
                i=0, j=0;
                for (int k=0; k<N/2; k++) {

                    if (i > N/4-1) {
                        double a = M2_2[j];
                        M2[k] = a;
                        j++;
                    }
                    else if (j > N/4-1) {
                        double a = M2_1[i];
                        M2[k] = a;
                        i++;
                    }
                    else if (M2_1[i] < M2_2[j]) {
                        double a = M2_1[i]; 
                        M2[k] = a;
                        i++;
                    }
                    else {
                        double b = M2_2[j];
                        M2[k] = b;
                        j++;
                    }
                }
            }

        }
   /*     printf("%s\n","-------------" );
           for ( int k=0;k<N/4; k++){
                printf("M2_1= %f\n  ", M2_1[k]);
            }
        printf("%s\n","-------------" );
           for ( int k=0;k<N/4; k++){
                printf("M2_2= %f\n  ", M2_2[k]);
            }

        for ( int k=0;k<N/2; k++){
            printf("M2 = %f\n ", M2[k]);
        }*/
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

