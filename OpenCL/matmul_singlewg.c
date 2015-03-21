#include "matmul.h"

void MatrixMultiplicationSingleWorkItem(sclHard hardware, char* module, char* kernel) {
    SimpleKernelDefinition d;

    const int matrixSize = 4;
    const int localSize = 1;
    int result;

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
    calculateTestMatrix(A, B, Ctest, matrixSize);
    
    printf("Ctest:\n");
    printMatrix(Ctest, matrixSize);
    printf("\n");

    // Define an NDRange size of 16x16 Work Items
    d.globalSize[0] = 1;
    d.globalSize[1] = 1;

    // Define the Work Group size that will contain the Work Items
    d.localSize[0] = 1;
    d.localSize[1] = 1;

    /* Compile the kernel to the target device */
    printf("Loading kernel\n");
    getSoftware(hardware, &d);

    /* Execute the compiled kernel */
    printf("Executing kernel\n");
    sclManageArgsLaunchKernel(
        d.hardware, d.software,
        d.globalSize, d.localSize,
        "%r %r %w %a",
        datasize, A,
        datasize, B,
        datasize, C,
        sizeof(int), &matrixSize);
    printf("\n");

    printf("C:\n");
    printMatrix(C, matrixSize);
    printf("\n");

    /* Verify matrix multiplication */
    printf("Verifying matrix multiplication\n");
    result = checkMatrixMultiplication(C, Ctest, MATRIX_SIZE, ALPHA);

    if ( result ) {
        printf("\n");
    } else {
        printf("All good\n");
    }
}
