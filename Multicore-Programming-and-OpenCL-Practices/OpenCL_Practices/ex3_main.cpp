#include <stdio.h>
#include <sys/stat.h>
#include <OpenCL/OpenCL.h>

#define ARRAY_SIZE 1024

cl_context CreateContext()
{
	cl_int errNum = 0;
	cl_uint numPlatforms = 0;
	cl_platform_id firstPlatformId = 0;
	cl_context context = NULL;
	
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (CL_SUCCESS != errNum || numPlatforms == 0)
	{
		fprintf(stderr, "Failed to find any OpenCL platforms\n");
		return NULL;
	}
	
	cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)firstPlatformId,
		0
	};
	
	context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Could not create GPU context, trying CPU...\n");
		
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
		if (CL_SUCCESS != errNum)
		{
			fprintf(stderr, "Failed to create an OpenCL GPU or CPU context\n");
			return NULL;
		}
	}
	
	return context;
}

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
	cl_int errNum = 0;
	cl_device_id *devices = NULL;
	cl_command_queue commandQueue = NULL;
	size_t deviceBufferSize = -1;
	
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Failed call clGetContextInfo(..., CL_CONTEXT_DEVICES, ...)\n");
		return NULL;
	}
	
	if (0 == deviceBufferSize)
	{
		fprintf(stderr, "No OpenCL devices available\n");
		return NULL;
	}
	
	devices = (cl_device_id *)malloc(deviceBufferSize);
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Failed to get OpenCL device IDs\n");
		free(devices);
		return NULL;
	}
	
	commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
	
	if (!commandQueue)
	{
		fprintf(stderr, "Failed to create command queue\n");
		return NULL;
	}
	
	return commandQueue;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char *kernelFileName)
{
	cl_int errNum = 0;
	cl_program program;
	
	FILE *file = fopen(kernelFileName, "r");
	if (!file)
	{
		fprintf(stderr, "Failed to open kernel file\n");
		return NULL;
	}
	
	struct stat st;
	stat(kernelFileName, &st);
	size_t fileSize = st.st_size;
	
	if (0 == fileSize)
	{
		fprintf(stderr, "Kernel source file was empty\n");
		return NULL;
	}
	
	char *fileBuffer = (char*)malloc(fileSize);
	
	size_t bytesRead = fread(fileBuffer, sizeof(char), fileSize, file);
	
	fclose(file);
	
	if (bytesRead != fileSize)
	{
		fprintf(stderr, "Failed to read complete kernel source file\n");
		free(fileBuffer);
		return NULL;
	}
	
	program = clCreateProgramWithSource(context, 1, (const char **)&fileBuffer, NULL, NULL);
	
	free(fileBuffer);
	
	if (!program)
	{
		fprintf(stderr, "Failed to create program from source\n");
		return NULL;
	}
	
	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (CL_SUCCESS != errNum)
	{
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
		fprintf(stderr, "Error when building program:\n%s\n", buildLog);
		clReleaseProgram(program);
		return NULL;
	}
	
	return program;
}

bool CreateMemObjects(cl_context context, cl_mem *memObjects, float *a, float *b)
{
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ARRAY_SIZE, NULL, NULL);
	
	if (!memObjects[0] || !memObjects[1] || !memObjects[2])
	{
		fprintf(stderr, "Failed to create memory objects\n");
		return false;
	}
	
	return true;
}

void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem *memObjects)
{
	if (context)
	{
		clReleaseContext(context);
	}
	
	if (commandQueue)
	{
		clReleaseCommandQueue(commandQueue);
	}
	
	if (program)
	{
		clReleaseProgram(program);
	}
	
	if (kernel)
	{
		clReleaseKernel(kernel);
	}
	
	for (int i = 0; i < 3; ++i)
	{
		if (memObjects[i])
		{
			clReleaseMemObject(memObjects[i]);
		}
	}
}

int main(int argc, const char * argv[])
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_program program = NULL;
	cl_device_id device = NULL;
	cl_kernel kernel = NULL;
	cl_mem memObjects[] = { NULL, NULL, NULL };
	cl_int errNum = 0;
	
	context = CreateContext();
	if (!context)
	{
		fprintf(stderr, "Failed to create OpenCL context\n");
		return 1;
	}
	
	commandQueue = CreateCommandQueue(context, &device);
	if (!commandQueue)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	program = CreateProgram(context, device, "./ex3_vector_addition.cl");
	if (!program)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	kernel = clCreateKernel(program, "ex3_vector_addition", NULL);
	if (!kernel)
	{
		fprintf(stderr, "Failed to create kernel\n");
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	float result[ARRAY_SIZE];
	float a[ARRAY_SIZE];
	float b[ARRAY_SIZE];
	
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		a[i] = i;
		b[i] = i * 2;
	}
	
	if (!CreateMemObjects(context, memObjects, a, b))
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Failed to set kernel arguments\n");
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	size_t globalWorkSize[] = { ARRAY_SIZE };
	size_t localWorkSize[] = { 1 };
	
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Error queuing kernel for execution\n");
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, ARRAY_SIZE * sizeof(float), result, 0, NULL, NULL);
	
	if (CL_SUCCESS != errNum)
	{
		fprintf(stderr, "Error reading result buffer\n");
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		printf("%f ", result[i]);
	}
	printf("\n");
	printf("Executed program successfully.\n");
	
	return 0;
}
