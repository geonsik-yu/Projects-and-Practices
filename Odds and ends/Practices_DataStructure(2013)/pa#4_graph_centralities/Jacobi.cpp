# include <cmath>
# include "Jacobi.h"
using namespace std;
//////////Reference #1 : JACOBI_EIGENVALUE _ Eigenvalues and Eigenvectors of a Symmetric Matrix
//////////				from <the GNU LGPL>
//////////Reference #2 : en.wikipedia.org/wiki/Jacobi_eigenvalue_algorithm
////////// 참조했는데, 수치 방법론의 구현 원리에 대해 일부분 이해가 모자란 부분이 있습니다;;

void jacobi_eigenvalue(int matSize, int** A, double** eigVec, double* eigVal){
	//// double type의 행렬로 변환하여 저장하기 위한 임시 행렬 a(원본 A는 int 행렬)
	double** a = new double*[matSize];
	for(int i = 0; i < matSize; i++)
		a[i] = new double[matSize];
	for(int i = 0; i < matSize; i++)
		for(int j = 0; j < matSize; j++)
			a[i][j] = (double)A[i][j];
	//// End of 변환

	double c;	double g;	double h;
	double s;	double t;
	double tau;	double term;
	double termp;	double termq;
	double theta;

	double gapq;
	identityFunc(matSize, eigVec); //// 아이겐벡터들을 저장하는 저장공간을 단위행렬로 채움
	getDiag(matSize, a, eigVal);   //// 아이겐벨유들을 저장하는 저장공간을 행렬a의 대각원소로 채움
	
	double* bw = new double[matSize];
	double* zw = new double[matSize];
	
	for(int i = 0; i < matSize; i++){
		bw[i] = eigVal[i];		//임시 저장공간 bw에 아이겐벨유를 복사
		zw[i] = 0.0;			//임시 저장공간 zw는 0.0으로 채움
	}
	
	int it_num = 0; // 이터레이션 수
	while( it_num < 100 ){	
		// 목표하는 정확도가 소수점 2번째 자리로 매우 낮으므로,
		// 100회 정도의 이터레이션으로 고정하여도 목표하는 값에 근사하는데 무리가 없을 것으로 예상했다.
		it_num = it_num + 1;
	    double thresh = 0.0;
		for(int i = 0; i < matSize; i++){
			for(int j = 0; j < i; j++){	//lower triangle 부분을 탐색하면서, thresh에 원소의 제곱값을 저장.
				thresh = thresh + a[i][j] * a[i][j];
			}
		}
		thresh = sqrt(thresh)/(double)(4*matSize);
		if( thresh == 0.0 )//반복 중단 조건.
			break;

		for(int p = 0; p < matSize; p++){
			for(int q = p + 1; q < matSize; q++){
				gapq = 10.0 * fabs(a[q][p]);
				termp = gapq + fabs(eigVal[p]);
				termq = gapq + fabs(eigVal[q]);
		
				if (4<it_num && termp== fabs(eigVal[p]) && termq == fabs(eigVal[q]))
					a[q][p] = 0.0;
				else if(thresh <= fabs(a[q][p])){
					h = eigVal[q] - eigVal[p];
					term = fabs(h) + gapq;

					if(term == fabs(h))
						t = a[q][p]/h;
					else{
						theta = 0.5 * h / a[q][p];
						t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0)
							t = - t;
					}
					c = 1.0/sqrt(1.0 + t*t);
					s = t * c;
					tau = s / ( 1.0 + c );
					h = t * a[q][p];

					zw[p] = zw[p] - h;                 
					zw[q] = zw[q] + h;
					eigVal[p] = eigVal[p] - h;
					eigVal[q] = eigVal[q] + h;
					a[q][p] = 0.0;

					for(int j = 0; j < p; j++){
						g = a[p][j];
						h = a[q][j];
						a[p][j] = g - s * ( h + g * tau );
						a[q][j] = h + s * ( g - h * tau );
					}

					for(int j = p+1; j < q; j++){
						g = a[j][p];
						h = a[q][j];
						a[j][p] = g - s * ( h + g * tau );
						a[q][j] = h + s * ( g - h * tau );
					}

					for(int j = q+1; j < matSize; j++){
						g = a[j][p];
						h = a[j][q];
						a[j][p] = g - s * ( h + g * tau );
						a[j][q] = h + s * ( g - h * tau );
					}

					for(int j = 0; j < matSize; j++){
						g = eigVec[p][j];
						h = eigVec[q][j];
						eigVec[p][j] = g - s * ( h + g * tau );
						eigVec[q][j] = h + s * ( g - h * tau );
					}
				}
			}
		}
		
		///bw업데이트, 아이겐벨유 업데이트, zw초기화.
		for(int i = 0; i < matSize; i++){
			bw[i] = bw[i] + zw[i];
			eigVal[i] = bw[i];
			zw[i] = 0.0;
		}
	}

	//상삼각 행렬 정리.
	for(int j = 0; j < matSize; j++){
		for(int i = 0; i < j; i++){
			a[j][i] = a[i][j];
		}
	}
	
	///이상 함수에서 사용했던 dynamic array 삭제
	delete[] bw;
	delete[] zw;

	for(int i = 0; i < matSize; i++)
		delete[] a[i];
	delete[] a;
	return;
};


//// 메트릭스 사이즈를 확인하고 행렬 a의 대각원소를 벡터 v에 저장.
void getDiag(int matSize, double** a, double* v){
	for(int i = 0; i < matSize; i++)
		v[i] = a[i][i];
  return;
};

//// 메트릭스 사이즈를 확인하고 행렬 a를 단위행렬로 만든다.
void identityFunc(int matSize, double** a)
{
	for(int i = 0; i < matSize; i++)
		for(int j = 0; j < matSize; j++)
			if( i == j ) a[i][j] = 1;
			else		 a[i][j] = 0;
  return;
};

//// 행렬의 Norm을 계산한다.
double norm ( int matSize, int** a ){
	double value = 0.0;
	for (int i = 0; i < matSize; i++ ){
		for (int j = 0; j < matSize; j++ ){
			value = value + a[i][j]*a[i][j];
		}
	}
	value = sqrt( value );
	return value;
};