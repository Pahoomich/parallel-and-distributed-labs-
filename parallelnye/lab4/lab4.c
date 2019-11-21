
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifdef _OPENMP
    #include <omp.h>
#else
    void omp_set_num_threads(int  num) {
        return​ ;
    }
    void omp_set_max_active_levels(int  num) {
        return​ ;
    }
    double omp_get_wtime() {
        struct  timeval T11;
        gettimeofday(&T11, NULL);
        return  (T11.tv_sec) *  1000 + (T11.tv_usec) /  1000;
    }

#endif
int main(int argc, char *argv[]) {
    int N;
    int A = 315;
    double T1, T2;
    double percent = 0.;
    long delta_ms;
    unsigned int i=0;
    N = atoi(argv[1]);
    double *M1 = malloc(sizeof(double) * N);
    double *M2 = malloc(sizeof(double) * N / 2);
    double *M1_pre = malloc(sizeof(double) * N);
    double *M2_pre = malloc(sizeof(double) * N / 2);
    double *M2_1 = malloc(sizeof(double) * N / 4);
    double *M2_2 = malloc(sizeof(double) * N / 4);

    T1 = omp_get_wtime();
    omp_set_max_active_levels(2);
#pragma omp parallel sections
    {
#pragma omp section
        {
            while  ( 1) {
                printf( "​ \r​ %f%% \n", percent);
                fflush(stdout);
                sleep(1);
                if (percent ==  100 ) {
                    printf( "​ \r​ %f%% \n", percent);
                    break;
                }
            }
        }
#pragma omp section
        {
            omp_set_num_threads( 3);
            for ( i = 0; i < 50; i++) {
                /*GENERATE*Заполнить массив исходных данных размером NxN*/
              //  printf("t3_count= %d\n",omp_get_num_threads());
                //printf("nameThead3= %d\n",omp_get_thread_num());
              //  printf("i= %d\n",i );
                    unsigned int seed =i;
                    percent+=0.5;
                    for (int k = 0; k < N; k++) {
                        M1_pre[k] = rand_r(&seed);
                    }
                    for (int k = 0; k < N/2; k++) {
                        M2_pre[k] = rand_r(&seed);
                    }
              //  printf("nameThead4= %d\n",omp_get_thread_num());
               // printf("t4_count= %d\n",omp_get_num_threads());
#pragma omp parallel for shared (M1_pre,A)
                for (int k = 0; k < N; k++) {
                    M1[k] = 1. + M1_pre[k]  / (RAND_MAX / (A - 1.));
                }
              //  printf("t4_count= %d\n",omp_get_num_threads());
#pragma omp parallel for shared(M2_pre, A)
                for (int j = 0; j < N / 2; j++) {
                    M2[j] = A + M2_pre[j] / (RAND_MAX / (10. * A - A));

                }
                // }
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
                    percent+=0.5;
                }

//      Merge         var 4
#pragma omp parallel for default(none) shared(M1, M2, N)
                for (int j = 0; j < N / 2; j++) {
                    M2[j] = M1[j] >= M2[j] ? M1[j] : M2[j];
                }

                double key;
//#pragma omp parallel for default(none) shared(M2, N) private(key, j)
                int l = 0, j = 0;
#pragma omp parallel shared(M2, M2_1, M2_2) private(key, l, j)
                {
#pragma omp for
                    for (int k = 0; k < N / 4; k++) {
                        M2_1[k] = M2[k];
                        M2_2[k] = M2[k + N / 4];
                    }
#pragma omp sections
                    {
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
                    }
                }
                l = 0;
                j = 0;
                for (int k = 0; k < N / 2; k++) {
                    if (l > N / 4 - 1) {
                        double a = M2_2[j];
                        M2[k] = a;
                        j++;
                    } else if (j > N / 4 - 1) {
                        double a = M2_1[l];
                        M2[k] = a;
                        l++;
                    } else if (M2_1[l] < M2_2[j]) {
                        double a = M2_1[l];
                        M2[k] = a;
                        l++;
                    } else {
                        double b = M2_2[j];
                        M2[k] = b;
                        j++;
                    }
                }
                double X = 0.;
                int int_part;
                for (int k = 0; k < N / 2; k++) {
                    int_part = M2[k] / M2[0];
                    if (!(int_part % 2)) {
                        X += sin(M2[k]);
                    }
                }
                percent+=1.;
                printf("%lf\n", X);


            }
        }
    }
    T2 = omp_get_wtime();
    printf("T1= %f\n", T1);
    printf("T2= %f\n", T2);
    delta_ms = (T2 - T1) * 1000;
    printf("\nN = %d. Milliseconds passed: %1ld\n\n", N, delta_ms);
    return 0;
        };
