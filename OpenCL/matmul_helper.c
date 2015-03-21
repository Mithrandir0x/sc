#include "matmul.h"

void getSoftware(sclHard hardware, SimpleKernelDefinition* def) {
    def->hardware = hardware;
    def->software = sclGetCLSoftware(
        def->modulePath,
        def->kernelName,
        hardware);
}

void printHexChar(char* s, int l) {
    for ( int i = 0 ; i < l ; i++ ) {
        printf(" %c (0x%02x)", s[i], s[i]);
    }
    printf(" ");
}

void printMatrix(float* M, int matrixSize) {
    for ( int y = 0 ; y < matrixSize ; y++ ) {
        for ( int x = 0 ; x < matrixSize ; x++ ) {
            printf("%f\t", M[y * matrixSize + x]);
        }
        printf("\n");
    }
}

/* Matrix multiplication - Host code */
/* Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
float doAPoint(int x, int y, float* A, float *B, const int sizeAX, const int sizeBX) {
    /* We assume sizeAX == sizeBY */
    float result = 0;
    int posA, posB;

    for (int step = 0; step<sizeAX; step++){

        posA = (y*sizeAX) + step;
        posB = (step*sizeBX) + x;

        result = result + A[posA] * B[posB];

    }
    return result;
}

void calculateTestMatrix(float* A, float* B, float* Ctest, int matrixSize) {
    // printf("\nCalculant resultats a CPU\n");
    for (int y = 0; y < matrixSize; y++) {
        for (int x = 0; x < matrixSize; x++) {
            Ctest[(y*matrixSize) + x] = doAPoint(x, y, A, B, matrixSize, matrixSize);
            // printf("%f\t", Ctest[(y*MATRIX_SIZE) + x]);
        }
        // printf("\n");
    }
}

int checkMatrixMultiplication(float* C, float* Ctest, int elements, float alpha) {
    float diff = 0.0;
    int count = 0;
    int result = 0;
    for ( int i = 0 ; i < elements ; i++ ) {
        diff = C[i] - Ctest[i];

        if ( diff > alpha ) {
            count++;
            printf("\nError a la posicio %d de C. Valor de C = %f. Valor de Ctest= %f.", i, C[i], Ctest[i]);
            result = 1;
        }
    }

    return result;
}
