// Kernel block.
kernel void my_vector_addition( global float* input_a, global float* input_b, global float* output ){
	
	// A unsigned integer variable storing index value retrieved from thread id (dimension 0)
	size_t gid = get_global_id(0);
	
	output[gid] = input_a[gid] + input_b[gid];
}
