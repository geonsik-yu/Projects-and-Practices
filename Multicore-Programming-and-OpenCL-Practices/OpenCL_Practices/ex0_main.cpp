/*
 Title: OpenCL Platform & Device Information.
 References:
	[1] Munshi, Aaftab, et al. OpenCL programming guide. Pearson Education, 2011.
	[2] B. Gaster, "Bgaster/opencl-book-samples". Github repository. N.p., 2011. Web. 30 June 2017
	[3] Clifford Wolf, "Simple OpenCL demo program." http://svn.clifford.at/tools/trunk/examples/cldemo.c 2009.
 Note:
	- Develop with OpenCL in OS X.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <OpenCL/cl.h>
int main(int argc, char** argv){
	
	cl_platform_id platforms[100];
	cl_uint platforms_n = 0;
	clGetPlatformIDs(100, platforms, &platforms_n);
	
	std::cout << "=== " << platforms_n <<" OpenCL platform(s) found: ===" << std::endl;
	for (int i=0; i<platforms_n; i++){
		char buffer[10240];
		std::cout << "  -- " << i << " --" << std::endl;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL);
		std::cout << "  PROFILE = " << buffer << std::endl;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, NULL);
		std::cout << "  VERSION = " << buffer << std::endl;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 10240, buffer, NULL);
		std::cout << "  NAME = " << buffer << std::endl;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL);
		std::cout << "  VENDOR = " << buffer << std::endl;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL);
		std::cout << "  EXTENSIONS = " << buffer << std::endl;
	}
	
	if (platforms_n == 0)
		return 1;
	
	cl_device_id devices[100];
	cl_uint devices_n = 0;
	clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 100, devices, &devices_n);
	//clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 100, devices, &devices_n);

	std::cout << std::endl << std::endl
	;
	std::cout << "=== " << platforms_n << " OpenCL device(s) found on platform:" << std::endl;
	for (int i=0; i<devices_n; i++){
		char buffer[10240];
		cl_uint buf_uint;
		cl_ulong buf_ulong;
		std::cout << "  -- " << i << " --" << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
		std::cout << "  DEVICE_NAME = " << buffer << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
		std::cout << "  DEVICE_VENDOR = %s" << buffer << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
		std::cout << "  DEVICE_VERSION = %s" << buffer << std::endl;
		clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
		std::cout << "  DRIVER_VERSION = %s" << buffer << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
		std::cout << "  DEVICE_MAX_COMPUTE_UNITS = " << (unsigned int)buf_uint << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
		std::cout << "  DEVICE_MAX_CLOCK_FREQUENCY = " << (unsigned int)buf_uint << std::endl;
		clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
		std::cout << "  DEVICE_GLOBAL_MEM_SIZE = " << (unsigned long long)buf_ulong << std::endl;
	}
	
	std::cout << "Executed program succesfully." << std::endl;
	
	return 0;
}


