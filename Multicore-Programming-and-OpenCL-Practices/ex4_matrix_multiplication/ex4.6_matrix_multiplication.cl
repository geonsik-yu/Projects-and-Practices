// Kernel block.
#define WIDTH 4
kernel void my_matrix_multiplication( const int dimN, const int dimP, const int dimM,
									 global float* input_a, global float* input_b, global float* output ){
	size_t k, j, w;
	size_t r_id = get_global_id(0);
	size_t local_id = get_local_id(0);
	size_t local_size = get_local_size(0);
	
	float tmp[WIDTH];
	
	float array_a[2048]; // kernel private variable (not dynamically allocated), for problem_2.
	__local float array_b[2048][WIDTH];
 	// local (shared) varialbe, for problem_2.
	// Number of columns loaded in local memory per each iteration increased. Accordingly, the number of barrier usage (synchronization) is decreased.
	for (k = 0; k < dimP; k++){
		array_a[k] = input_a[r_id*dimP + k];
	}
	
	for (j = 0; j < dimM; j += WIDTH){
		for (k = local_id; k < dimP; k += local_size){
			for (w = 0; w <WIDTH; w++)
				array_b[k][w] = input_b[k*dimM+j+w];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		for (w = 0; w <WIDTH; w++)
			tmp[w] = 0.0;
		for (k = 0; k < dimP; k++){
			for (w = 0; w <WIDTH; w++)
				tmp[w] += array_a[k] * array_b[k][w];
		}
		for (w = 0; w <WIDTH; w++)
			output[r_id * dimM + j + w] = tmp[w];
	}
}
