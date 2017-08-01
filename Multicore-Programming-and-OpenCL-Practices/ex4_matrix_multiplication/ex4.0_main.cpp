/*
 Title: OpenCL Matrix-Matrix Multiplication Example 0: a sequential code for naive comparison.
 References:
	[1] Munshi, Aaftab, et al. OpenCL programming guide. Pearson Education, 2011.
	[2] Bernd Burgstaller. "Programming Accelerator with OpenCL." Multicore Programming. Yonsei University. Engineering Bld. 2016. Lecture.
 Note:
	- Develop with OpenCL in OS X. Xcode.
	- Floating point error in multiplication is not properly managed.
*/


#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <mach/clock.h>
#include <mach/mach.h>

// A hard-coded error bound.
static float ERROR_BOUND = 0.5;

void mat_mul_ijk(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result);
void mat_mul_ikj(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result);
void mat_mul_kij(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result);
void Validation(float* correct_result, float* result, size_t n, size_t m);

int main(int argc, const char * argv[]) {
	
	clock_serv_t cclock;
	mach_timespec t_start, t_end;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);

	int n, p, m;
	std::ifstream fin;
	fin.open("./problem_2.txt");
	// Input file "problem_2.txt" contains a matrix multiplication problem with ( n = 1024, p = 2048, m = 512 )
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
	
	float* a = new float[n*p];//[DIM_N][DIM_P];
	float* b = new float[p*m];//[DIM_P][DIM_M];
	float* result = new float[n*m];//[DIM_N][DIM_M];
	for(int i = 0; i < n*m; i++)
		result[i] = 0;
	float* correct_result = new float[n*m];

	// matrix a (NxP)
	for(size_t i = 0; i < n; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(size_t j = 0; j < p; j++){
			ss >> buf; a[i*p+j] = atof(buf.c_str());
		}
	}
	// matrix b (PxM)
	for(size_t i = 0; i < p; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(size_t j = 0; j < m; j++){
			ss >> buf; b[i*m+j] = atof(buf.c_str());
		}
	}
	for(int i = 0; i < n; i++){
		getline(fin, read_str);
		std::stringstream ss(read_str);
		for(int j = 0; j < m; j++){
			ss >> buf; correct_result[i*m+j] = atof(buf.c_str());
		}
	}

	clock_get_time(cclock, &t_start);
	//mat_mul_ijk(n, p, m, a, b, result);
	mat_mul_kij(n, p, m, a, b, result);
	//mat_mul_ikj(n, p, m, a, b, result);
	clock_get_time(cclock, &t_end);


	unsigned long elapsed = (t_end.tv_sec*1e9 + t_end.tv_nsec) - (t_start.tv_sec*1e9 + t_start.tv_nsec);
	printf("Elapsed Time: (ns) %lu\n", elapsed);
	mach_port_deallocate(mach_task_self(), cclock);
	
	Validation(correct_result, result, n, m);
	
	delete[] a;
	delete[] b;
	delete[] result;
	delete[] correct_result;
	
	return 0;
}


void mat_mul_ijk(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result){
	int i, j, k;
	for (i=0; i<Ndim; i++)
		for (j=0; j<Mdim; j++)
			for(k=0; k<Pdim; k++)
				result[i*Mdim+j] += a[i*Pdim+k] * b[k*Mdim+j];
}


void mat_mul_ikj(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result){
	int i, j, k;
	for (i=0; i<Ndim; i++)
		for(k=0; k<Pdim; k++)
			for (j=0; j<Mdim; j++)
				result[i*Mdim+j] += a[i*Pdim+k] * b[k*Mdim+j];
}

void mat_mul_kij(int Ndim, int Pdim, int Mdim, float* a, float* b, float* result){
	int i, j, k;
	for(k=0; k<Pdim; k++)
		for (i=0; i<Ndim; i++)
			for (j=0; j<Mdim; j++)
				result[i*Mdim+j]  += a[i*Pdim+k] *  b[k*Mdim+j];
}

// Function 5. Result correctness validation.
void Validation(float* correct_result, float* result, size_t n, size_t m){
	// Floating point number precision error not properly managed.
	bool flag = true;
	for(size_t i = 0; i < n*m; i++){
		if( fabs(correct_result[i] - result[i]) > ERROR_BOUND ){
			flag = false;// break;
			std::cout << "error: "<< fabs( result[i] - correct_result[i] ) << std::endl;
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
