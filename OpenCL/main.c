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
#define MATRIX_SIZE 16
/* Matrix multiplication - Host code */
/* Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
float doAPoint(int x, int y, float* A, float *B, const int sizeAX, const int sizeBX) {
	/* We assume sizeAX == sizeBY */
	float result=0;
	int posA,posB;
		
	for (int step=0; step<sizeAX; step++){
	
	  posA = (y*sizeAX) + step ;
	  posB = (step*sizeBX) + x;
	
	  result = result + A[posA] *B[posB];
		
	}
	return result;
}

int main() {
  /* This code executes on the OpenCL host */
  int found;

  /* SimpleOpenCL types declaration */
  sclHard* hardware;
  sclSoft software;

  /* Host data */
  float *A = NULL;  // Input array
  float *B = NULL;  // Input array
  float *C = NULL;  // Output array
  float *Ctest = NULL; // Test array
  //float diff;
  /* Size of the matrix */
  const int elements = MATRIX_SIZE * MATRIX_SIZE;

  /* Compute the size of the data */ 
  size_t datasize = sizeof(float) * elements;

  /* Allocate space for input/output data */
  A = (float *) malloc(datasize);
  B = (float *) malloc(datasize);
  C = (float *) malloc(datasize);
  Ctest = (float *) malloc(datasize);

  /* Initialize the input data */
  for(int i=0; i < elements; i++) {
   A[i] = i*0.00001;
   B[i] = 0.001;
  }

  /* NDRange 2D size initialization*/
  size_t global_size[2];
  size_t local_size[2];  // WorkGroup
  size_t dataSize = sizeof(float)*elements;
  size_t localBlockSize = sizeof(float)*BLOCK_SIZE_H*BLOCK_SIZE_V;

  // Set NDRange dimensions. It would be a single NDRANGE that contains:
  //   A GLOBAL WI INDEX that ranges from 0 to ( MATRIX_SIZE - 1 )
  //   A LOCAL WI INDEX that ranges from 0 to ( BLOCK_SIZE_H - 1 )
  // This means that there will be:
  //   Work-units: 16x16
  //   Work-group: 1x1
  global_size[0]=MATRIX_SIZE; global_size[1]=MATRIX_SIZE; // Define an NDRange size of 16x16 Work Groups
  local_size[0]=BLOCK_SIZE_H; local_size[1]=BLOCK_SIZE_V; // Define a Work Group size of 1x1 Work Item

  /* Inicialitzar hardware i software */
  //hardware = sclGetGPUHardware( 0, &found );
  hardware = sclGetAllHardware(&found);
  sclPrintDeviceNamePlatforms(hardware, found);
  software = sclGetCLSoftware("matmul_kernel.cl", "MatMulKernel", hardware[0]);
  /* Kernel execution */
  sclManageArgsLaunchKernel( hardware[1], software,
    global_size, local_size,
    " %r %r %w %N %N ",
    dataSize, (void*) A, // %r
    dataSize, (void*) B, // %r
    dataSize, (void*) C, // %w
    MATRIX_SIZE,  // %N
    MATRIX_SIZE); // %N
  
  /* Check results */
  printf("\nCalculant resultats a CPU\n");
  for ( int y=0 ; y < MATRIX_SIZE ; y++ ) {
  	for ( int x=0 ; x < MATRIX_SIZE ; x++) {
      // y*MATRIX_SIZE)+x == [y][x]
  	  Ctest[(y*MATRIX_SIZE)+x] = doAPoint(x, y, A, B, MATRIX_SIZE, MATRIX_SIZE);
      printf("%f\t", Ctest[(y*MATRIX_SIZE)+x]); 
  	}
    printf("\n");
  }
  
  /* Falta generar resultats per a la matriu C */
  /*for (int i=0; i<elements; i++){
  	diff=C[i]-Ctest[i];

  	if (diff>ALPHA){
  		count++;
      printf("\nError a la posicio %d de C. Valor de C = %f. Valor de Ctest= %f.",i,C[i],Ctest[i]);
    }
  }*/
  
  printf("\n");
  
  return 0;
}




