/* Host code */


/*

work-item  = processing element
work-group = compute unit
NDRANGE    = compute device?

*/

#include <stdio.h>
#include "matmul.h"

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
	//MatrixMultiplicationSingleWorkItem(selected, "./kernel/matmul_singlewg_kernel.cl", "MatMulKernel");
	MatrixMultiplicationXAxisWorkItems(hardware, "./kernel/matmul_singlewg_kernel.cl", "MatMulKernel");

	printf("\n");

	return 0;
}

