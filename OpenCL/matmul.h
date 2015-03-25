#ifndef _MATMUL_H_
#define _MATMUL_H_

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "simple-opencl/simpleCL.h"

#define BLOCK_SIZE_H 1
#define BLOCK_SIZE_V 1
#define ALPHA 0.0001
#define MATRIX_SIZE 4

/* Matrices are stored in row-major order: */
/* M(row, col) = *(M.elements + row * M.width + col) */

typedef struct {
    sclHard hardware;
    sclSoft software;
    size_t globalSize[2];
    size_t localSize[2];
    char* modulePath;
    char* kernelName;
} SimpleKernelDefinition;

/* KERNEL EXECUTORS */
void MatrixMultiplicationSingleWorkItem(sclHard* hardware, char* module, char* kernel);
void MatrixMultiplicationXAxisWorkItems(sclHard* hardware, char* module, char* kernel);

/* MISCELLANEOUS */
void getSoftware(sclHard hardware, SimpleKernelDefinition* def);
void printHexChar(char* s, int l);
void printMatrix(float* M, int matrixSize);
void getTime(timespec *ts);
double getSeconds(timespec* start, timespec* end);

/* MATRIX HELPERS */
float doAPoint(int x, int y, float* A, float *B, const int sizeAX, const int sizeBX);
void calculateTestMatrix(float* A, float* B, float* Ctest, int matrixSize);
int checkMatrixMultiplication(float* C, float* Ctest, int elements, float alpha);

#endif
