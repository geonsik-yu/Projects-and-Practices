/*
 Title: OpenCL Matrix-Matrix Multiplication Example 3
 References:
	[1] Munshi, Aaftab, et al. OpenCL programming guide. Pearson Education, 2011.
	[2] B. Gaster, "Bgaster/opencl-book-samples". Github repository. N.p., 2011. Web. 30 June 2017
	[3] Bernd Burgstaller. "Programming Accelerator with OpenCL." Multicore Programming. Yonsei University. Engineering Bld. 2016. Lecture.
	[4] W. Hwu Wen-Mei. "Basic Matrix-Matrix Multiplication." Heterogeneous Parallel Programming, Coursera. 2016. Lecture.
 Note:
	- Develop with OpenCL in OS X. Xcode.
	- 1st optimization: increase workload per individual kernel (one row of the result matrix per workitem)
	- + minor modification: additional rows padded to utilize every computation unit available.
	- Since floating number precision error is not properly handled in this introductory example, our tolerance value is quite large.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <OpenCL/cl.h>

// A hard-coded error bound.
static float ERROR_BOUND = 0.5;

// Set-up Function Declarations.
cl_context CreateContext();
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device);
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName);
bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float *a, float *b, size_t n, size_t p, size_t m);
void Validation(float* correct_result, float* result, size_t n, size_t m);
void TimeStamp(cl_event prof_event);
void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjs[3]);
void PrintMatrix(size_t Nrow, size_t Ncol, float* matrix);

int main(int argc, char** argv){
	
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[3] = { 0, 0, 0 };
	cl_int errNum;
	cl_event prof_event; // Event that handles profile kernel execution times.
	
	// 1. Context ----------------------------------------------------------
	context = CreateContext();
	if (context == NULL){
		std::cerr << "Failed to create OpenCL context." << std::endl;
		return 1;
	}
	
	// 2. Command Queue ----------------------------------------------------
	commandQueue = CreateCommandQueue(context, &device);
	if (commandQueue == NULL){
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	// 3. Program ----------------------------------------------------------
	program = CreateProgram(context, device, "ex4.3_matrix_multiplication.cl");
	if (program == NULL){
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	// 4. Kernel -----------------------------------------------------------
	kernel = clCreateKernel(program, "my_matrix_multiplication", NULL);
	if (kernel == NULL){
		std::cerr << "Failed to create kernel" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	
	// 5. Kernel Memory Objects and Kernel Arguments -----------------------
	// 2D matrices flattended into 1D array.
	size_t n; size_t p; size_t m;
	
	std::ifstream fin;
	fin.open("./problem_2.txt");
	// Input file "problem_1.txt" contains a matrix multiplication problem with ( n = 4, p = 8, m = 2 )
	// 	and "problem_2.txt" contains a problem with ( n = 1024, p = 2048, m = 512 )
	if (!fin.is_open()){
		std::cout << "input file open failure" << std::endl;
		exit(1);
	}
	
	std::string read_str;
	std::string buf;
	getline(fin, read_str);
	std::stringstream ss(read_str);
	ss >> buf; n = atoi(buf.c_str());
	ss >> buf; p = atoi(buf.c_str());
	ss >> buf; m = atoi(buf.c_str());
	
	size_t padded_n = 0;
	cl_ulong numMaxCU = 0;
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(numMaxCU), &numMaxCU, NULL);
	if (numMaxCU == 0){
		std::cerr << "Max CU value is 0" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	} else {
		while (padded_n < n)
			padded_n += (size_t) numMaxCU;
	}
	
	float* a = new float[padded_n*p];//[DIM_N][DIM_P];
	float* b = new float[p*m];//[DIM_P][DIM_M];
	float* result = new float[padded_n*m];//[DIM_N][DIM_M];
	
	// matrix a (NxP)
	for(size_t i = 0; i < n; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(size_t j = 0; j < p; j++){
			ss >> buf; a[i*p+j] = (atof(buf.c_str()));
		}
	}
	for(size_t i = n; i < padded_n; i++){
		for(size_t j = 0; j < p; j++){
			a[i*p+j] = 0;
		}
	}
	
	// matrix b (PxM)
	for(size_t i = 0; i < p; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(size_t j = 0; j < m; j++){
			ss >> buf; b[i*m+j] = (atof(buf.c_str()));
		}
	}
	//PrintMatrix(n, p, a);
	//PrintMatrix(p, m, b);
	
	if (!CreateMemObjects(context, memObjects, a, b, padded_n, p, m)){
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	errNum = clSetKernelArg(kernel, 0, sizeof(int), &padded_n);
	errNum |= clSetKernelArg(kernel, 1, sizeof(int), &p);
	errNum |= clSetKernelArg(kernel, 2, sizeof(int), &m);
	errNum |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &memObjects[2]);
	if (errNum != CL_SUCCESS){
		std::cerr << "Error setting kernel arguments." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	// 6. Queue Kernel and Read the output back to the Host ----------------
	size_t globalWorkSize = (size_t) padded_n;
	size_t localWorkSize = (size_t) padded_n / numMaxCU;
	
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, &prof_event);
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, padded_n*m*sizeof(float), result, 0, NULL, &prof_event);
	if (errNum != CL_SUCCESS){
		std::cerr << "Error queuing kernel for execution and/or reading result buffer." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	// 7. Output the result buffer, result validation and measure performance.
	//PrintMatrix(DIM_N, DIM_M, result);
	float* correct_result = new float[n*m];
	for(int i = 0; i < n; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(int j = 0; j < m; j++){
			ss >> buf; correct_result[i*m+j] = atof(buf.c_str());
		}
	}
	
	Validation(correct_result, result, n, m);
	TimeStamp(prof_event);
	std::cout << "Executed program succesfully." << std::endl;
	delete[] a;
	delete[] b;
	delete[] result;
	Cleanup(context, commandQueue, program, kernel, memObjects);
	
	return 0;
}

// Function 1. OpenCL Context.
// Create an OpenCL context on the first available platform.
cl_context CreateContext(){
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context = NULL;
	
	// 1-1. Select an OpenCL platform to run on.  For this example, we simply choose the first available platform.
	// (*) Normally, you would query for all available platforms and select the most appropriate one.
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0){
		std::cerr << "Failed to find any OpenCL platforms." << std::endl;
		return NULL;
	}
	
	// 1-2. Create an OpenCL context on the platform. Attempt to create a GPU-based context.
	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)firstPlatformId, 0};
	context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
	// - If the attempt fails, try to create a CPU-based context.
	if (errNum != CL_SUCCESS){
		std::cout << "Could not create GPU context, trying CPU..." << std::endl;
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
		if (errNum != CL_SUCCESS){
			std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
			return NULL;
		}
	}
	return context;
}


// Function 2. OpenCL Context.
// Create a command queue on the first device available on the context.
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device){
	cl_int errNum;
	cl_device_id *devices;
	cl_command_queue commandQueue = NULL;
	size_t deviceBufferSize = -1;
	
	// 2-1. Get the size of the devices buffer
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
	if (errNum != CL_SUCCESS){
		std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
		return NULL;
	}
	if (deviceBufferSize <= 0){
		std::cerr << "No devices available.";
		return NULL;
	}
	
	// 2-2. Allocate memory for the devices buffer.
	devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
	if (errNum != CL_SUCCESS){
		delete [] devices;
		std::cerr << "Failed to get device IDs";
		return NULL;
	}
	// 2-3. Choose an availble device (first available device in this example).
	// (*) Normally, you would query for all available devices and select the most appropriate one.
	commandQueue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, NULL);
	if (commandQueue == NULL){
		delete [] devices;
		std::cerr << "Failed to create commandQueue for device 0";
		return NULL;
	}
	
	*device = devices[0];
	delete [] devices;
	return commandQueue;
}


// Function 3. OpenCL Program.
// Create an OpenCL program from the kernel source file.
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName){
	cl_int errNum;
	cl_program program;
	
	std::ifstream kernelFile;
	kernelFile.open(fileName, std::ios::in);
	if (!kernelFile.is_open()){
		std::cerr << "Failed to open file for reading: " << fileName << std::endl;
		return NULL;
	}
	
	std::ostringstream oss;
	oss << kernelFile.rdbuf();
	
	std::string srcStdStr = oss.str();
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
	if (program == NULL)
	{
		std::cerr << "Failed to create CL program from source." << std::endl;
		return NULL;
	}
	
	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS){
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
		std::cerr << "Error in kernel: " << std::endl;
		std::cerr << buildLog;
		clReleaseProgram(program);
		return NULL;
	}
	
	return program;
}

// Function 4. OpenCL Kernel Memory Objects.
bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float *a, float *b, size_t n, size_t p, size_t m){
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*n*p, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*p*m, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float)*n*m, NULL, NULL);
	
	if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL){
		std::cerr << "Error creating memory objects." << std::endl;
		return false;
	}
	return true;
}

// Function 5. Result correctness validation.
void Validation(float* correct_result, float* result, size_t n, size_t m){
	// Floating point number precision error not properly managed.
	bool flag = true;
	for(size_t i = 0; i < n*m; i++){
		if( fabs(correct_result[i] - result[i]) > ERROR_BOUND ){
			flag = false;// break;
			std::cout << std::setprecision(10) << "error: "<< fabs( result[i] - correct_result[i] ) << std::endl;
		}
	}
	if(flag==true){
		std::cout << "Computation result is proper." << std::endl;
		std::cout << "------------------------------------------" << std::endl;
	}
	else{
		std::cout << "Computation result is not proper." << std::endl;
		std::cout << "------------------------------------------" << std::endl;
	}
}


// Function 6. Measure performance.
// print out timestamp on a nanosecond granularity.
void TimeStamp(cl_event prof_event){
	cl_ulong queue_time;
	cl_ulong submission_time;
	cl_ulong exe_start_time;
	cl_ulong exe_end_time;
	cl_int errNum;
	
	errNum = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queue_time, NULL);
	errNum = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &submission_time, NULL);
	errNum = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &exe_start_time, NULL);
	errNum = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &exe_end_time, NULL);
	
	unsigned long elapsed_queue = (unsigned long) (submission_time - queue_time);
	unsigned long elapsed_submitted = (unsigned long) (exe_start_time - submission_time);
	unsigned long elapsed_execution = (unsigned long) (exe_end_time - exe_start_time);
	
	std::cout << "elapsed time (queue)     : " << elapsed_queue << std::endl;
	std::cout << "elapsed time (submitted) : " << elapsed_submitted << std::endl;
	std::cout << "elapsed time (execution) : " << elapsed_execution << std::endl;
	std::cout << "------------------------------------------" << std::endl;
}


// Function 7. Cleanup any created OpenCL resources.
void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjects[3]){
	for (int i = 0; i < 3; i++)
		if (memObjects[i] != 0) clReleaseMemObject(memObjects[i]);
	if (commandQueue != 0) clReleaseCommandQueue(commandQueue);
	if (kernel != 0) clReleaseKernel(kernel);
	if (program != 0) clReleaseProgram(program);
	if (context != 0) clReleaseContext(context);
}


// Function 8. Print a flattened matrix
void PrintMatrix(size_t Nrow, size_t Ncol, float* matrix){
	// matrix a[Ndim][Pdim]
	for(size_t i = 0; i < Nrow; i++){
		for(size_t j = 0; j < Ncol; j++)
			std::cout << std::setprecision(10) <<matrix[(Ncol*i)+j]  << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
};


