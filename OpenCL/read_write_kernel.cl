__kernel void ReadAndWriteKernel(__global char* in, __global char* out) {
	int i = get_global_id(0);
	out[i] = in[i];
}

