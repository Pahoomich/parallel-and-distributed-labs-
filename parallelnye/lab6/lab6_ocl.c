#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

size_t get_global_size(size_t arr_size, size_t local_size) {
    return local_size * ceil((double) arr_size / local_size);
}

int main(int argc, char *argv[]) {
    int N;
    int A = 315;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]);
    int N2 = N / 2;

    double *M1_pre = malloc(sizeof(double) * N);
    double *M2_pre = malloc(sizeof(double) * N / 2);
    double *M1 = malloc(sizeof(double) * N);
    double *M2 = malloc(sizeof(double) * N / 2);

    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("merge_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *) malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    cl_mem d_M1; // buffer object for M1
    cl_mem d_M2; // buffer object for M2

    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
    cl_int err;                       //err from opencl api


    size_t global_size_merge;
    size_t group_size_merge;

// Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);

    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

    // Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                                        (const char **) &source_str, (const size_t *) &source_size, &err);
    // Build the program executable
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "merge_kernel", &err);

    // Create the input and output arrays in device memory for our calculation
    d_M1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(double) * N, NULL, NULL);
    d_M2 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(double) * (N / 2), NULL, NULL);

    // Get the maximum work group size for executing the kernel on the
    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(group_size_merge), &group_size_merge,
                             NULL);
    global_size_merge = get_global_size((N / 2), group_size_merge);

    gettimeofday(&T1, NULL);
    for (unsigned int i = 0; i < 50; i++) {
        unsigned int seed = i;
        for (int k = 0; k < N; k++) {
            M1_pre[k] = rand_r(&seed);
        }
        for (int k = 0; k < N / 2; k++) {
            M2_pre[k] = rand_r(&seed);
        }

//----------Этап Generate----------
        for (int k = 0; k < N; k++) {
            M1[k] = 1. + M1_pre[k] / (RAND_MAX / (A - 1.));
        }

        for (int j = 0; j < N / 2; j++) {
            M2[j] = A + M2_pre[j] / (RAND_MAX / (10. * A - A));
        }

//----------Этап Map----------
//var 6
        for (int k = 0; k < N; k++) {
            M1[k] = pow(M1[k] / M_E, 1. / 3.);
        }

//var 2
        for (int j = 1; j < N / 2; j++) {
            M2[j] = fabs(cos(M2[j] + M2[j - 1]));
        }
        M2[0] = fabs(cos(M2[0]));
//----------Этап Merge----------
//var 4
// Write our data set into the input array in device memory
        err = clEnqueueWriteBuffer(queue, d_M1, CL_TRUE, 0,
                                   sizeof(double) * N, M1, 0, NULL, NULL);
        err = clEnqueueWriteBuffer(queue, d_M2, CL_TRUE, 0,
                                    sizeof(double) * (N / 2), M2, 0, NULL, NULL);
// Set the arguments to our compute kernel
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_M1);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_M2);
        err = clSetKernelArg(kernel, 2, sizeof(unsigned int), &N2);

        // Execute the kernel over the entire range of the data set
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size_merge, &group_size_merge,
                                     0, NULL, NULL);

        // Wait for the command queue to get serviced before reading back results
        clFinish(queue);


        // Read the results from the device
        clEnqueueReadBuffer(queue, d_M2, CL_TRUE, 0,
                            sizeof(double) * (N / 2), M2, 0, NULL, NULL);

        /*Отсортировать массив с результатами указанным методом*/
//      Sort         var 6
        int j;
        double key;
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
        for (int k = 0; k < N / 2; k++) {
            int_part = M2[k] / M2[0];
            if (!(int_part % 2)) {
                X += sin(M2[k]);
            }
        }
        printf("%lf\n", X);
    }

    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN = %d. Milliseconds passed: %1ld\n", N, delta_ms);

    free(M1_pre);
    free(M2_pre);
    free(M1);
    free(M2);

    // release OpenCL resources
    clReleaseMemObject(d_M1);
    clReleaseMemObject(d_M2);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return 0;
}
