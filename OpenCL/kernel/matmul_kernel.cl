
__kernel void MatMulKernel(
    __global float *A,
    __global float *B,
    __global float *C,
    __local float *As,
    __local float *Bs,
    int MS) {
    
    // Work Group index
    int bx = get_group_id(0);
    int by = get_group_id(1);
 
    // Work Item index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    int blockSize = get_local_size(0);
 
    // Index of the first sub-matrix of A processed by the block
    int aBegin = MS * blockSize * by;
 
    // Index of the last sub-matrix of A processed by the block
    int aEnd   = aBegin + MS - 1;
 
    // Step size used to iterate through the sub-matrices of A
    int aStep  = blockSize;
 
    // Index of the first sub-matrix of B processed by the block
    int bBegin = blockSize * bx;
 
    // Step size used to iterate through the sub-matrices of B
    int bStep  = blockSize * MS;

    float Csub = 0.0;
 
    // Loop over all the sub-matrices of A and B
    // required to compute the block sub-matrix
    for (int a = aBegin, b = bBegin;
             a <= aEnd;
             a += aStep, b += bStep) {
 
        // Load the matrices from global memory to local memory; each work item
        // loads one element of each matrix
        As[(ty * blockSize) + tx] = A[a + MS * ty + tx];
        Bs[(ty * blockSize) + tx] = B[b + MS * ty + tx];
 
        // Synchronize to make sure the matrices are loaded
        barrier(CLK_LOCAL_MEM_FENCE);
 
        // Multiply the two matrices together;
        // each work item computes one element
        // of the work group sub-matrix
        for (int k = 0; k < blockSize; ++k) {
            //Csub += As[ty][k] * Bs[k][tx];
            Csub += As[(ty * blockSize) + k] * Bs[(k * blockSize) + tx];
        }
 
        // Synchronize to make sure that the preceding
        // computation is done before loading two new
        // sub-matrices of A and B in the next iteration
        barrier(CLK_LOCAL_MEM_FENCE);
 
    }
 
    // Write the work group sub-matrix to device memory;
    // each work item writes one element
    int c = MS * blockSize * by + blockSize * bx;
    C[c + MS * ty + tx] = Csub;

}
