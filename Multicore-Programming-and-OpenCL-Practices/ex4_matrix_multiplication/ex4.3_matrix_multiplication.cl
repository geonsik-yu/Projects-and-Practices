// Kernel block.
kernel void my_matrix_multiplication( const int dimN, const int dimP, const int dimM,
									 global float* input_a, global float* input_b, global float* output ){
	size_t k, j, k2;
	size_t r_id = get_global_id(0);
	
	float tmp;
	float array[2048]; // for problem_2, kernel private variable (not dynamically allocated).
	for (k = 0; k < dimP; k++){
		array[k] = input_a[r_id*dimP + k];
	}
	
	for (j = 0; j < dimM; j++){
		tmp = 0.0;
		for (k = 0; k < dimP; k++){
			tmp += array[k] * input_b[k * dimM + j];
		}
		output[r_id * dimM + j] = tmp;
	}
}
