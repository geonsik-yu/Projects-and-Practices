// Kernel block.
kernel void my_vector_addition1( global float* input_a, global float* input_b, global float* output ){
	
	// A unsigned integer variable storing index value retrieved from thread id (dimension 0)
	size_t gid = get_global_id(0);
	size_t temp = 0;
	size_t i;
	
	if (gid%2 == 0){
		for (i = 0; i < 1024; i++)
			temp += i * i;
		output[gid] = input_a[gid] + input_b[gid];
	}
	else{
		for (i = 0; i < 1024; i++)
			temp += i * i;
		output[gid] = temp;
	}
}
