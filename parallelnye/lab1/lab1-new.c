#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define A 819
#define DBL_MAX __DBL_MAX__

int main(int argc, char *argv[]) {
    const int N = atoi(argv[1]);
    const int N2 = N / 2;
    const int N2m1 = N2 - 1;
    const int randIndM1 = RAND_MAX / A;
    const int randIndM2 = RAND_MAX / (9 * A);
    const int CountTest = 50;
    struct timeval TimeStar, TimeEnd;
    long delta_ms;
    gettimeofday(&TimeStar, NULL);
    for (unsigned int numTest = 0; numTest < CountTest; ++numTest) {
// Stage 1 begin
        double M1[N];
        double M2[N2];
        unsigned int myseed = numTest;
        for (int i = 0; i < N; i++) {
            M1[i] = rand_r(&myseed) / randIndM1;
        }
        for (int i = 0; i < N2; i++) {
            M2[i] = A + (rand_r(&myseed) / randIndM2);
        }
// Stage 2#7 begin
        for (int i = 0; i < N; i++) {
            M1[i] = exp(sqrt(M1[i]));
        }
        double prev = 0.0;
        for (int i = 0; i < N2; i++) {
            double cur = M2[i];
            M2[i] = log(fabs(tan(prev + cur)));
            prev = cur;
        }
// Stage 3 (merging) begin
        for (int i = 0; i < N2; i++) {
            M2[i] *= M1[i];
        }
//Stage 4 (sorting) begin
        for (int i = 0; i < N2m1; i++) {
            int indexMin = i;
            for (int j = i + 1; j < N2; j++) {
                if (M2[indexMin] > M2[j])
                    indexMin = j;
            }
            if (indexMin != i) {
                double swap = M2[i];
                M2[i] = M2[indexMin];
                M2[indexMin] = swap;
            }
        }
//Stage 5 (getting X)
        double min = DBL_MAX;
        for (int i = 0; i < N2; i++) {
            double current = M2[i];
            if (current < min && current > 0) {
                min = current;
            }
        }
        double X = 0.0;
        for (int i = 0, countSummedElement = 0; i < N2 && countSummedElement < 3; i++) {
            if ((int) (M2[i] / min) % 2 == 0) {
                X += sin(M2[i]);
                countSummedElement++;
            }
        }
    }
    gettimeofday(&TimeEnd, NULL);
    delta_ms = 1000 * (TimeEnd.tv_sec - TimeStar.tv_sec) + (TimeEnd.tv_usec -
                                                            TimeStar.tv_usec) / 1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms);
    return 0;
}

