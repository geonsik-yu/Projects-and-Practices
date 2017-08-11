/*
 Title: OpenCL Vector Addition Example
 References:
	[1] Munshi, Aaftab, et al. OpenCL programming guide. Pearson Education, 2011.
	[2] B. Gaster, "Bgaster/opencl-book-samples". Github repository. N.p., 2011. Web. 30 June 2017
 Note:
	- Develop with OpenCL in OS X.
*/

#include <stdio.h>
#include <stdlib.h>

#include <OpenCL/opencl.h>
#include "ex2.1_vector_addition.cl.h"

#define NUM_VALUES 2048 // A hard-coded number.

static int validate(cl_float* input_a, cl_float* input_b, cl_float* output) {
	int i;
	for (i = 0; i < NUM_VALUES; i++) {
		
		// The kernel was supposed to square each value.
		if ( output[i] != (input_a[i] + input_b[i]) ) {
			fprintf(stdout,
					"Error: Element %d did not match expected output.\n", i);
			fprintf(stdout,
					"       Result %1.0f, expected %1.0f\n", output[i], input_a[i] + input_b[i]);
			fflush(stdout);
			return 0;
		}
		fprintf(stdout,
				"       Result %1.0f, input %1.0f + %1.0f\n", output[i], input_a[i], input_b[i] );
	}
	return 1;
}

int main (int argc, const char * argv[]) {
	// Step 1 ------------------------------------------------------------------------
	// : Dispatch Queue
	dispatch_queue_t queue = gcl_create_dispatch_queue(CL_DEVICE_TYPE_GPU, NULL);
 
 
	// Step 2 ------------------------------------------------------------------------
	// : Memory Allocation.
	float* test_in_a = (float*)malloc(sizeof(cl_float) * NUM_VALUES);
	float* test_in_b = (float*)malloc(sizeof(cl_float) * NUM_VALUES);
	for (int i = 0; i < NUM_VALUES; i++) {
		test_in_a[i] = (cl_float)(rand()%100);
		test_in_b[i] = (cl_float)(rand()%100);
	}
	float* test_out = (float*)malloc(sizeof(cl_float) * NUM_VALUES);
	
	void* mem_in_a  = gcl_malloc(sizeof(cl_float) * NUM_VALUES, test_in_a, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
	void* mem_in_b  = gcl_malloc(sizeof(cl_float) * NUM_VALUES, test_in_b, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
 	void* mem_out = gcl_malloc(sizeof(cl_float) * NUM_VALUES, NULL, CL_MEM_WRITE_ONLY);
 
	
	// Step 3 ------------------------------------------------------------------------
	// : Dispatch Kernel Blocks.
	dispatch_sync(queue, ^{
		size_t wgs;
		gcl_get_kernel_block_workgroup_info(my_vector_addition_kernel,
											CL_KERNEL_WORK_GROUP_SIZE,
											sizeof(wgs), &wgs, NULL);
		fprintf(stdout, "Local workgroup size: %d\n", (int)wgs);
		
		cl_ndrange range = {
			1,					// The number of dimensions to use.
			
			{0, 0, 0},			// The offset in each dimension.
			
			{NUM_VALUES, 0, 0},	// The global range—this is how many items
			
			{wgs, 0, 0}			// The local size of each workgroup.
		};
		
		my_vector_addition_kernel(&range, (cl_float*)mem_in_a, (cl_float*)mem_in_b, (cl_float*)mem_out);
		gcl_memcpy(test_out, mem_out, sizeof(cl_float) * NUM_VALUES);
	});
 
	
	// Step 4 ------------------------------------------------------------------------
	// : Validation and Memory Release.
	if ( validate(test_in_a, test_in_b, test_out)) {
		fprintf(stdout, "All values were properly added.\n");
	}
 
	gcl_free(mem_in_a);
	gcl_free(mem_in_b);
	gcl_free(mem_out);
 
	free(test_in_a);
	free(test_in_b);
	free(test_out);
 
	dispatch_release(queue);
}
