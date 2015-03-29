/**
 * Matrix multiplication with an NDRange with one Work Item.
 * Both A, B and C must have MS width and height.
 * 
 * @param *A Pointer to matrix A
 * @param *B Pointer to matrix B
 * @param *C Pointer to matrix C where the result will be stored
 * @param MS Matrix size
 */

float doAPoint(int x, int y, __global float* A, __global float *B, const int sizeAX, const int sizeBX) {
    float result = 0;
    int posA, posB;

    for ( int step = 0 ; step < sizeAX ; step++ ) {
        posA = ( y * sizeAX ) + step;
        posB = ( step * sizeBX ) + x;

        result = result + A[posA] * B[posB];
    }
    
    return result;
}

__kernel void MatMulKernel(__global float *A, __global float *B, __global float *C, int MS ) {
    int localId = get_local_id(0);
    int globalId = get_global_id(0);
    int globalSizeX = get_global_size(0);

    int x = ( globalSizeX * globalId ) + localId;

    for ( int y = 0 ; y < MS ; y++ ) {
        C[( y * MS ) + x] = doAPoint(x, y, A, B, MS, MS);
    }
}
