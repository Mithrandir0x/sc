/* Host code */
/* Matrices are stored in row-major order: */
/* M(row, col) = *(M.elements + row * M.width + col) */

/*

work-item  = processing element
work-group = compute unit
NDRANGE    = compute device?

*/

#include <stdio.h>
#include "simple-opencl/simpleCL.h"

#define BLOCK_SIZE_H 1
#define BLOCK_SIZE_V 1
#define ALPHA 0.0001
#define MATRIX_SIZE 4

/* STRUCT HELPERS */
typedef struct {
	sclHard hardware;
	sclSoft software;
	size_t globalSize[2];
	size_t localSize[2];
	char* modulePath;
	char* kernelName;
} SimpleKernelDefinition;

/* UTILITY FUNCTIONS */
// Yeah I'm just THAT lazy
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

/* The SimpleCL Hello World example, though utterly simplified (or vilified) */
void ReadWrite(sclHard hardware, char* module, char* kernel) {
	SimpleKernelDefinition d;

	/* Host data */
	char* in = "Hello World";
	char* out;
	int variableLength;

	d.modulePath = module;
	d.kernelName = kernel;

	variableLength = strlen(in);
	out = (char *) malloc(sizeof(char) * variableLength);

	d.globalSize[0] = variableLength;
	d.globalSize[1] = 1;

	d.localSize[0] = variableLength;
	d.localSize[1] = 1;

	printf("Loading kernel\n");
	getSoftware(hardware, &d);

	printf("Executing kernel\n");
	sclManageArgsLaunchKernel(
		d.hardware, d.software,
		d.globalSize, d.localSize,
		"%r %w",
		variableLength, in,
		variableLength, out);

	//printf("[%s]\n", out);
	printf("Copied string: ");
	printf("[");
	printHexChar(out, variableLength);
	printf("]\n");
	
	free(out);
}

int main() {
	/* This code executes on the OpenCL host */
	int found;

	/* SimpleOpenCL types declaration */
	sclHard* hardware;
	sclHard selected;

	/* Initialize kernel */
	//hardware = sclGetGPUHardware( 0, &found );
	hardware = sclGetAllHardware(&found);
	selected = hardware[1];
	printf("\n\n");

	// ReadWrite(selected, "./kernel/read_write_kernel.cl", "ReadAndWriteKernel");
	MatrixMultiplicationSingleWorkItem(selected, "./kernel/matmul_singlewg_kernel.cl", "MatMulKernel");

	printf("\n");

	return 0;
}

