// Kernel block.
kernel void my_matrix_multiplication( const int dimN, const int dimP, const int dimM,
									 global float* input_a, global float* input_b, global float* output ){
	size_t k, j;
	size_t r_id = get_global_id(0);
	
	float tmp;
	float array[dimN];
	
	for (k = 0; k < dimP; k++)
		array[k] = input_a[r_id*dimP + k];
	for (j = 0; j < dimM; j++){
		tmp = 0.0;
		for (k = 0; k < dimP; k++)
			tmp += input_a[k] * input_b[k * dimM + j];
		output[r_id * dimM + j] = tmp;
	}
}
