// Kernel block.
kernel void my_square( global float* input, global float* output ){

	// A unsigned integer variable storing index value retrieved from thread id (dimension 0)
	size_t id = get_global_id(0);

	output[id] = input[id] * input[id];
}
