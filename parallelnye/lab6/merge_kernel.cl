/ OpenCL kernel. Each work item takes care of one element of c
__kernel void merge_kernel(__global double *M1,
                           __global double *M2,
                           const unsigned int n) {
    //Get our global thread ID
    int id = get_global_id(0);

    //Make sure we do not go out of bounds
    if (id < n) {
        M2[id] = M1[id] >= M2[id] ? M1[id] : M2[id];
    }
}
