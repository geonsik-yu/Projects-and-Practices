// Kernel block.
kernel void my_matrix_multiplication( const int dimN, const int dimP, const int dimM,
									 global int* input_a, global int* input_b, global int* output ){
	size_t k, j;
	size_t r_id = get_global_id(0);
	size_t local_id = get_local_id(0);
	size_t local_size = get_local_size(0);

	int tmp;
	int array_a[2048]; // kernel private variable (not dynamically allocated), for problem_2.
	__local int array_b[2048]; // local (shared) varialbe, for problem_2.
	for (k = 0; k < dimP; k++){
		array_a[k] = input_a[r_id*dimP + k];
	}

	for (j = 0; j < dimM; j++){
		for (k = local_id; k < dimP; k = k + local_size){
			array_b[k] = input_b[k*dimM+j];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		tmp = 0.0;
		for (k = 0; k < dimP; k++){
			tmp += array_a[k] * array_b[k];
		}
		output[r_id * dimM + j] = tmp;
	}
}
