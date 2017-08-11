// Kernel block.
kernel void my_matrix_multiplication( const int dimN, const int dimP, const int dimM,
									 __global float* input_a, __global float* input_b, __global float* output ){
	size_t k;
	size_t r_id = get_global_id(0);
	size_t c_id = get_global_id(1);
	
	float tmp = 0.0;
	for (k = 0; k < dimP; k++)
		tmp += input_a[r_id * dimP + k] * input_b[k * dimM + c_id];
	output[r_id * dimM + c_id] = tmp;
}
