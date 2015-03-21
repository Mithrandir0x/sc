#include "matmul.h"

#define CPU 0
#define GPU 1

void MatrixMultiplicationXAxisWorkItems(sclHard* hardware, char* module, char* kernel) {
    SimpleKernelDefinition d;

    int result;
    const int matrixSize = 4;

    double ellapsedTime;
    struct timespec start, end;

    /* Host data */
    float *A = NULL;     // Input array
    float *B = NULL;     // Input array
    float *C = NULL;     // Output array
    float *Ctest = NULL; // Test array

    // float diff;

    d.modulePath = module;
    d.kernelName = kernel;

    /* Size of the matrix */
    const int elements = matrixSize * matrixSize;

    /* Compute the size of the data */
    size_t datasize = sizeof(float) * elements;

    /* Allocate space for input/output data */
    A = (float*) malloc(datasize);
    B = (float*) malloc(datasize);
    C = (float*) malloc(datasize);
    Ctest = (float*) malloc(datasize);

    /* Initialize the input data */
    for ( int i = 0 ; i < elements ; i++ ) {
        A[i] = i * 0.00001;
        B[i] = 0.001;
    }
    
    printf("A:\n");
    printMatrix(A, matrixSize);
    printf("\n");
    
    printf("B:\n");
    printMatrix(B, matrixSize);
    printf("\n");

    /* Initialize test matrix */
    getTime(&start);
    calculateTestMatrix(A, B, Ctest, matrixSize);
    getTime(&end);
    
    printf("Ctest:\n");
    printMatrix(Ctest, matrixSize);
    printf("\n");

    ellapsedTime = getSeconds(&start, &end);
    printf("CPU Time [%lf]\n", ellapsedTime);

    // Define an NDRange size of 16x16 Work Items
    d.globalSize[0] = matrixSize;
    d.globalSize[1] = 1;

    // Define the Work Group size that will contain the Work Items
    d.localSize[0] = matrixSize;
    d.localSize[1] = 1;

    /* Compile the kernel to the target device */
    printf("(OpenCL/GPU) Loading kernel\n");
    getSoftware(hardware[GPU], &d);

    /* Execute the compiled kernel */
    printf("(OpenCL/GPU) Executing kernel\n");
    getTime(&start);
    sclManageArgsLaunchKernel(
        d.hardware, d.software,
        d.globalSize, d.localSize,
        "%r %r %w %a",
        datasize, A,
        datasize, B,
        datasize, C,
        sizeof(int), &matrixSize);
    getTime(&end);
    printf("\n");

    /* printf("C:\n");
    printMatrix(C, matrixSize);
    printf("\n"); */

    ellapsedTime = getSeconds(&start, &end);
    printf("(OpenCL/GPU) GPU Time [%lf]\n", ellapsedTime);

    /* Verify matrix multiplication */
    printf("(OpenCL/GPU) Verifying matrix multiplication\n");
    result = checkMatrixMultiplication(C, Ctest, MATRIX_SIZE, ALPHA);

    if ( result ) {
        printf("\n");
    } else {
        printf("All good\n");
    }

    /* Compile the kernel to the target device */
    printf("(OpenCL/CPU) Loading kernel\n");
    getSoftware(hardware[CPU], &d);

    /* Execute the compiled kernel */
    printf("(OpenCL/CPU) Executing kernel\n");
    getTime(&start);
    sclManageArgsLaunchKernel(
        d.hardware, d.software,
        d.globalSize, d.localSize,
        "%r %r %w %a",
        datasize, A,
        datasize, B,
        datasize, C,
        sizeof(int), &matrixSize);
    getTime(&end);
    printf("\n");

    /* printf("C:\n");
    printMatrix(C, matrixSize);
    printf("\n"); */

    ellapsedTime = getSeconds(&start, &end);
    printf("(OpenCL/CPU) CPU Time [%lf]\n", ellapsedTime);

    /* Verify matrix multiplication */
    printf("(OpenCL/CPU) Verifying matrix multiplication\n");
    result = checkMatrixMultiplication(C, Ctest, MATRIX_SIZE, ALPHA);

    if ( result ) {
        printf("\n");
    } else {
        printf("All good\n");
    }
}
