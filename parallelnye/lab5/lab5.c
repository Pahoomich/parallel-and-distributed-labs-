#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct {
    double sum;
    int veclen_m1, veclen_m2;
    double percent;
//----------arrays----------
    double *M1;
    double *M2;
//----------addition for fill----------
    double *M1_pre;
    double *M2_pre;
//----------addition for short----------
    double *M2_1;
    double *M2_2;
} DOTDATA;

/* Define globally accessible variables and a mutex */

#define NUMTHRDS 4
#define A 315
#define chunk_percent 0.5
DOTDATA dotstr;
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;

void start_end_schedule(long thred_num, int veclen, int *start, int *end) {
    *start = thred_num * (veclen / (NUMTHRDS - 1));
    if (veclen % (NUMTHRDS - 1) != 0 & thred_num == (NUMTHRDS - 1) - 1) {
        *end = *start + veclen / (NUMTHRDS - 1) + veclen % (NUMTHRDS - 1);//последнему добавочку
    } else
        *end = *start + veclen / (NUMTHRDS - 1);
}

void *initArray(void *arg) {
    int iForSeed = (intptr_t) arg;
    unsigned int seed = iForSeed;
    dotstr.percent += chunk_percent;
    for (int k = 0; k < dotstr.veclen_m1; k++) {
        dotstr.M1_pre[k] = rand_r(&seed);
    }
    for (int k = 0; k < dotstr.veclen_m1 / 2; k++) {
        dotstr.M2_pre[k] = rand_r(&seed);
    }
}

void *fillArrayM1(void *arg) {
    int start, end;
    long offset;
    offset = (long) arg;
    start_end_schedule(offset, dotstr.veclen_m1, &start, &end);
    for (unsigned int k = start; k < end; k++) {
        dotstr.M1[k] = 1. + dotstr.M1_pre[k] / (RAND_MAX / (A - 1.));
    }
}

void *fillArrayM2(void *arg) {
    int start, end;
    long offset;
    offset = (long) arg;
    start_end_schedule(offset, dotstr.veclen_m2, &start, &end);
    if (dotstr.veclen_m2 / (NUMTHRDS - 1) > 1) {
        for (unsigned int k = start; k < end; k++) {
            dotstr.M2[k] = A + dotstr.M2_pre[k] / (RAND_MAX / (10. * A - A));
        }
    } else if (offset == 0) {
        for (unsigned int k = 0; k < dotstr.veclen_m2; k++) {
            dotstr.M2[k] = A + dotstr.M2_pre[k] / (RAND_MAX / (10. * A - A));
        }
    }
    pthread_exit((void *) 0);
}

void *mapM1(void *arg) {
    int start, end;
    long offset;
    offset = (long) arg;
    start_end_schedule(offset, dotstr.veclen_m1, &start, &end);
    for (int k = start; k < end; k++) {
        dotstr.M1[k] = pow(dotstr.M1[k] / M_E, 1. / 3.);
    }
    pthread_exit((void *) 0);
}

void *mapM2(void *arg) {
    for (int j = 1; j < dotstr.veclen_m2; j++) {
        dotstr.M2[j] = fabs(cos(dotstr.M2[j] + dotstr.M2[j - 1]));
    }
    dotstr.M2[0] = fabs(cos(dotstr.M2[0]));
    dotstr.percent += chunk_percent;
    pthread_exit((void *) 0);
}

void *mergeArray(void *arg) {
    int start, end;
    long offset;
    offset = (long) arg;
    start_end_schedule(offset, dotstr.veclen_m2, &start, &end);
    if (dotstr.veclen_m2 / (NUMTHRDS - 1) > 1) {
        for (int j = start; j < end; j++) {
            dotstr.M2[j] = dotstr.M1[j] >= dotstr.M2[j] ? dotstr.M1[j] : dotstr.M2[j];
        }
    } else if (offset == 0) {
        for (int j = 0; j < dotstr.veclen_m2; j++) {
            dotstr.M2[j] = dotstr.M1[j] >= dotstr.M2[j] ? dotstr.M1[j] : dotstr.M2[j];
        }
    }
    pthread_exit((void *) 0);
}

void *creatSubSortArr(void *arg) {
    int start, end;
    long offset;
    offset = (long) arg;
    start_end_schedule(offset, dotstr.veclen_m1 / 4, &start, &end);
    for (int k = start; k < end; k++) {
        dotstr.M2_1[k] = dotstr.M2[k];
        dotstr.M2_2[k] = dotstr.M2[k + dotstr.veclen_m1 / 4];
    }
    pthread_exit((void *) 0);
}

void *boobleSort(void *arg) {
    long offset;
    offset = (long) arg;
    double key;
    int l = 0, j = 0;
    if (offset == 0) {
        for (int k = 1; k < dotstr.veclen_m1 / 4; k++) {
            key = dotstr.M2_1[k];
            j = k - 1;
            while (j >= 0 && dotstr.M2_1[j] > key) {
                dotstr.M2_1[j + 1] = dotstr.M2_1[j];
                j = j - 1;
            }
            dotstr.M2_1[j + 1] = key;
        }
    } else {
        for (int k = 1; k < dotstr.veclen_m1 / 4; k++) {
            key = dotstr.M2_2[k];
            j = k - 1;
            while (j >= 0 && dotstr.M2_2[j] > key) {
                dotstr.M2_2[j + 1] = dotstr.M2_2[j];
                j = j - 1;
            }
            dotstr.M2_2[j + 1] = key;
        }
    }
    pthread_exit((void *) 0);
}

void *mergeSort(void *arg) {
    int l = 0;
    int j = 0;
    for (int k = 0; k < dotstr.veclen_m2; k++) {
        if (l > dotstr.veclen_m2 / 2 - 1) {
            double a = dotstr.M2_2[j];
            dotstr.M2[k] = a;
            j++;
        } else if (j > dotstr.veclen_m2 / 2 - 1) {
            double a = dotstr.M2_1[l];
            dotstr.M2[k] = a;
            l++;
        } else if (dotstr.M2_1[l] < dotstr.M2_2[j]) {
            double a = dotstr.M2_1[l];
            dotstr.M2[k] = a;
            l++;
        } else {
            double b = dotstr.M2_2[j];
            dotstr.M2[k] = b;
            j++;
        }
    }
    pthread_exit((void *) 0);
}

void *sumX(void *arg) {
    dotstr.sum = 0;
    int int_part;
    for (int k = 0; k < dotstr.veclen_m2; k++) {
        int_part = dotstr.M2[k] / dotstr.M2[0];
        if (!(int_part % 2)) {
            dotstr.sum += sin(dotstr.M2[k]);
        }
    }
    dotstr.percent += chunk_percent * 2;
    pthread_exit((void *) 0);
}

void *printX(void *arg) {
    printf("%f\n", dotstr.sum);
    pthread_exit((void *) 0);
}

void *procBar(void *arg) {
    while (1) {
        printf("​ \r​ %f%% \n", dotstr.percent);
        fflush(stdout);
        sleep(1);
        if (dotstr.percent == 100) {
            printf("​ \r​ %f%% \n", dotstr.percent);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    long i;
    double *M1, *M2, *M1_pre, *M2_pre, *M2_1, *M2_2;
    void *status;
    struct timeval T1, T2;
    long delta_ms;
    pthread_attr_t attr;

/* Assign storage and initialize values */

    dotstr.veclen_m1 = atoi(argv[1]);
    dotstr.veclen_m2 = dotstr.veclen_m1 / 2;

    M1 = (double *) malloc(dotstr.veclen_m1 * sizeof(double));
    M2 = (double *) malloc(dotstr.veclen_m1 / 2 * sizeof(double));
    M1_pre = (double *) malloc(dotstr.veclen_m1 * sizeof(double));
    M2_pre = (double *) malloc(dotstr.veclen_m1 / 2 * sizeof(double));
    M2_1 = (double *) malloc(dotstr.veclen_m1 / 4 * sizeof(double));
    M2_2 = (double *) malloc(dotstr.veclen_m1 / 4 * sizeof(double));
    dotstr.M1 = M1;
    dotstr.M2 = M2;
    dotstr.M1_pre = M1_pre;
    dotstr.M2_pre = M2_pre;
    dotstr.M2_1 = M2_1;
    dotstr.M2_2 = M2_2;
    dotstr.percent = 0.;

    pthread_mutex_init(&mutexsum, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&callThd[0], &attr, procBar, (void *) NULL);

    gettimeofday(&T1, NULL);

    for (int iterNum = 0; iterNum < 50; iterNum++) {

        pthread_create(&callThd[1], &attr, initArray, (void *) (intptr_t) iterNum);

        pthread_join(callThd[1], &status);

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_create(&callThd[i + 1], &attr, fillArrayM1, (void *) i);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_join(callThd[i + 1], &status);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_create(&callThd[i + 1], &attr, fillArrayM2, (void *) i);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_join(callThd[i + 1], &status);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_create(&callThd[i + 1], &attr, mapM1, (void *) i);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_join(callThd[i + 1], &status);
        }

        pthread_create(&callThd[1], &attr, mapM2, (void *) NULL);

        pthread_join(callThd[1], &status);

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_create(&callThd[i + 1], &attr, mergeArray, (void *) i);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_join(callThd[i + 1], &status);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_create(&callThd[i + 1], &attr, creatSubSortArr, (void *) i);
        }

        for (i = 0; i < NUMTHRDS - 1; i++) {
            pthread_join(callThd[i + 1], &status);
        }
        pthread_create(&callThd[1], &attr, boobleSort, (void *) 0);
        pthread_create(&callThd[2], &attr, boobleSort, (void *) 1);
        pthread_join(callThd[1], &status);
        pthread_join(callThd[2], &status);

        pthread_create(&callThd[1], &attr, mergeSort, (void *) NULL);
        pthread_join(callThd[1], &status);
        pthread_create(&callThd[1], &attr, sumX, (void *) NULL);
        pthread_join(callThd[1], &status);
        pthread_create(&callThd[1], &attr, printX, (void *) NULL);
        pthread_join(callThd[1], &status);

    }
    gettimeofday(&T2, NULL);
    pthread_join(callThd[0], &status);

    pthread_attr_destroy(&attr);

    free(M1);
    free(M2);
    free(M1_pre);
    free(M2_pre);
    free(M2_1);
    free(M2_2);
    pthread_mutex_destroy(&mutexsum);

    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN = %d. Milliseconds passed: %1ld\n\n", dotstr.veclen_m1, delta_ms);
    pthread_exit(NULL);
}
