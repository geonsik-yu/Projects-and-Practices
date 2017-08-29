#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "MyLinkedList.cpp"
#include "Jacobi.h"
using namespace std;

int shortestCountFunc(int startVertex, int targetVertex, int maxLength, int verCount, int** mat);
int shortestCountFuncThroughK(int startVertex, int targetVertex, int via, int nowVia, int maxLength, int verCount, int** mat);

void main(){
	ifstream fin;
	ofstream fout;
	fin.open("c:\\ds\\input.txt");
	fout.open("c:\\ds\\hw4\\2008122092.txt");

	if( fin.is_open() )	// input file open Checker
		cout << "input file opened properly\n";
	else { cout << "file open failure\n"; exit(1); } 


	MyNode* instList = new MyNode[20];
	int* matrixSize	  = new int[20];	// "n" : size of adjacent matrix
	int*** adjacencyMat = new int**[20];	// "adjacent matrices x20"
	int*** shortestDist = new int**[20];
	int*** shortestCount= new int**[20];

	double*** eigVecList = new double**[20];
	double** eigValList = new double*[20];
	//// Array Initialization
	for(int i = 0; i < 20; i++){
		matrixSize[i]  = 0;
		instList[i] = MyNode();
	}
	//// Initialization End

	int index = -1;
	string str; // temporary string for data input
	while( !fin.eof() ){
		fin >> str;
		if(str == "graph"){
			index++;
			fin >> str; // matrix Size
			matrixSize[index] = atoi(str.c_str());
			int tempMatSize = matrixSize[index];

			//// Adjacency Matrix Initailization
			adjacencyMat[index] = new int*[  tempMatSize  ];
			shortestDist[index] = new int*[  tempMatSize  ];
			shortestCount[index]= new int*[  tempMatSize  ];
			eigVecList[index]   = new double*[  tempMatSize  ];
			eigValList[index]   = new double[  tempMatSize  ];
			for(int i = 0; i < tempMatSize; i++){
				adjacencyMat[index][i] = new int[ tempMatSize ];
				shortestDist[index][i] = new int[ tempMatSize ];
				shortestCount[index][i]= new int[ tempMatSize ];
				eigVecList[index][i]   = new double[ tempMatSize ];
			};
			for(int i = 0; i < tempMatSize; i++)
				for(int j = 0; j < tempMatSize; j++)
					shortestCount[index][i][j] = 0;
			//// initialization End

			/// Floyd-Washal Algorithm to calculate shortest distances
			for(int i = 0; i < tempMatSize; i++){// initialization
				for(int j = 0; j < tempMatSize; j++){
					fin >> str;
					int temp = atoi(str.c_str());
					adjacencyMat[index][i][j] = temp;
					if(temp != 0)
						shortestDist[index][i][j] = temp;
					else
						shortestDist[index][i][j] = tempMatSize+1;
				};
			};

			for(int k = 0; k < tempMatSize; k++){
				for(int i = 0; i < tempMatSize; i++){
					for(int j = 0; j < tempMatSize; j++){
						if(i == j)
							shortestDist[index][i][j] = 0;
						else if( shortestDist[index][i][j] > shortestDist[index][i][k] + shortestDist[index][k][j])
							shortestDist[index][i][j] = shortestDist[index][i][k] + shortestDist[index][k][j];
					};
				};
			};
			///END

			///to find Eigenvector & Eigenvalues, use numerical method "jacobian method" referenced from GNU LGPL
			jacobi_eigenvalue(tempMatSize, adjacencyMat[index], eigVecList[index], eigValList[index]);
			///END - look "jacobi.cpp"

			/// # of shortest paths from A to B --- upper triangular & using recursive call
			for(int j = 0; j < tempMatSize; j++){
				for(int k = j+1; k < tempMatSize; k++){
					int maxLength = shortestDist[index][j][k];// j와 k를 잇는 최단거리.
						shortestCount[index][j][k] = shortestCountFunc(j, k, maxLength, tempMatSize, adjacencyMat[index]);
				};
			};
			///END

		}else if(str == "quit"){
			/// do nothing... anyway it goes for 20 samples...
		}else{
			/// Linked List >>> store details of instructions
			string tempInst = str;
			char* tempChar = new char[2];
			for(int i = 0; i < 2; i++)
				tempChar[i] = str.at(i);
			fin >> str;
			int tempNumber = atoi(str.c_str());
			MyNode* node = new MyNode(tempChar, tempNumber);
			MyNode* temp = &instList[index];
			while( temp->getNext() != NULL ){
				temp = temp->getNext();
			};
			temp->setNext(node);
		};
	};

//	for(int i = 0; i < 20; i++){
//		cout << "#" << i << "\n";
//		int temp = matrixSize[i];
//		for(int j = 0; j < temp; j++){
//			for(int k = 0; k < temp; k++)
//				cout << adjacencyMat[i][j][k];
//			cout << "\n";
//		};
//		for(int j = 0; j < temp; j++){
//			for(int k = 0; k < temp; k++)
//				cout << shortestDist[i][j][k];
//			cout << "\n";
//		};
//
//		MyNode* current = instList[i].getNext();
//		while(current != NULL){
//			string inst = "";
//			for(int i = 0; i < 2; i++)
//				inst = inst + current->getInstruction()[i];
//
//			cout << inst << " " << current->getNumber() <<"\n";
//			current = current->getNext();
//		};
//	};

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	fout.setf(ios::fixed);
	fout.setf(ios::showpoint);
	fout.precision(2);

	for(int i = 0; i < 20; i++){
		cout << "#" << i+1 <<"\n";
		fout << "#" << i+1 <<"\n";
		MyNode* current = instList[i].getNext();
		while(current != NULL){
			char inst = current->getInstruction()[0];
			int number = current->getNumber();
			if(inst == 'd'){////Degree Centrality
				double sum = 0.0;
				int size = matrixSize[i];
				for(int j = 0; j < size; j++)
					sum += adjacencyMat[i][number][j];
				sum = sum/(double)(size-1);
				sum = floor(sum * 100 +0.5) / 100.0;
				cout << "dc " << sum <<"\n";
				fout << "dc " << number << " " << sum <<"\n";
			}else if(inst == 'b'){////Betweenness Centrality
				int size = matrixSize[i];
				double sum = 0;
				for(int j = 0; j < size; j++){
					for(int k = j+1; k < size; k++){
						if( j != number && k != number){
							int countShortestThroughK = 0;
							int maxLength = shortestDist[i][j][k];// j와 k를 잇는 최단거리.
							countShortestThroughK = shortestCountFuncThroughK(j, k, number, false, maxLength, size, adjacencyMat[i]);
							//cout <<"CSK: "<< countShortestThroughK <<"\n";
							//cout <<"SCM: "<< shortestCount[i][j][k] <<"\n";
							sum = sum + (double)countShortestThroughK/(double)shortestCount[i][j][k];
							//cout <<"Sum: "<< sum <<"\n";
						};
					};
				};
				sum = (2*sum) / (double)((size-1)*(size-2));
				sum = floor(sum * 100 +0.5) / 100.0;
				cout << "bc " << sum <<"\n";
				fout << "bc " << number << " " << sum <<"\n";
			}else if(inst == 'c'){////Closeness Centrality
				double sum = 0.0;
				int size = matrixSize[i];
				for(int j = 0; j < size; j++)
					sum += shortestDist[i][number][j];
				sum = (double)(size-1)/sum;
				sum = floor(sum * 100 +0.5) / 100.0;
				cout << "cc " << sum <<"\n";
				fout << "cc " << number << " " << sum <<"\n";
			}else if(inst == 'e'){////Eigenvector Centrality
				int size = matrixSize[i];
				int maxIndex = 0;
				double maxVal = 0.0;
				for(int j = 0; j < size; j++){
					if( eigValList[i][j] > maxVal){ 
						maxIndex = j;
						maxVal = eigValList[i][j];
					};
				};
				double result = 0.0;
				for(int k = 0; k < size; k++)
					result = result + (double)adjacencyMat[i][number][k] * (double)eigVecList[i][maxIndex][k];
				result = result/eigValList[i][maxIndex];
				result = floor(result * 100 +0.5) / 100.0;
				cout << "ec " << result <<"\n";
				fout << "ec " << number << " " << result <<"\n";
			}else if(inst == 'q'){
				cout << "quit " <<"\n";
				/// actually do nothing...
			}else{
				cout << "others " <<"\n";
				/// there will be no wrong inputs...
			};
			current = current->getNext();
		};
	};

	fin.close();
	fout.close();


	// 링크드 리스트 삭제
	for(int i = 0; i < 20 ; i++){
		if(&instList[i] != NULL){
			MyNode* deleteMe = &instList[i];
			while( deleteMe != NULL ){
				MyNode* next = deleteMe->getNext();
				deleteMe->~MyNode();
				deleteMe = next;
			};
		}
	};

	for(int i = 0; i < matrixSize[i]; i++){
		for(int j = 0; j < matrixSize[i]; j++){
			delete[] adjacencyMat[i][j];
			delete[] shortestDist[i][j];
			delete[] shortestCount[i][j];
			delete[] eigVecList[i][j];
		}
		delete[] adjacencyMat[i];
		delete[] shortestDist[i];
		delete[] shortestCount[i];
		delete[] eigVecList[i];
		delete[] eigValList[i];
	}
	delete[] adjacencyMat;
	delete[] shortestDist;
	delete[] shortestCount;
	delete[] eigVecList;
	delete[] eigValList;
	delete[] instList;
	delete[] matrixSize;
	int i;
	cin >> i;
};

int shortestCountFunc(int startVertex, int targetVertex, int maxLength, int verCount, int** mat){
	/// 재귀호출을 이용한 최단경로 탐색(모든 이어진 노드를 탐색하되, 목적지에 최단거리조건을 만족시키며 도달하면 카운트를 1 더하여 리턴)
	int count = 0;
	if(maxLength > 0){
		for(int i = 0; i < verCount; i++){
			if(mat[startVertex][i] == 1 && i == targetVertex)
				count = count + 1;
			else if(mat[startVertex][i] == 1 && i != targetVertex)
				count = count + shortestCountFunc(i, targetVertex, maxLength-1, verCount, mat);
		}
		return count;
	}else
		return 0;
};

int shortestCountFuncThroughK(int startVertex, int targetVertex, int via, int nowVia, int maxLength, int verCount, int** mat){
	/// 재귀호출을 이용한 최단경로 탐색(모든 이어진 노드를 탐색하되, 목적지에 최단거리조건을 만족시키며 도달하면 카운트를 1 더하여 리턴)
	/// 단 이번에는 via vertex를 경유한 경우에 불리언 플래그를 세워서 플래그를 만족한 경우에만 카운트를 증가.
	int count = 0;
	if(maxLength > 0){
		for(int i = 0; i < verCount; i++){
			if(mat[startVertex][i] == 1 && i == targetVertex && nowVia == true)
				count = count + 1;
			else if(mat[startVertex][i] == 1 && i == targetVertex && nowVia == false)
				count = count;
			else if(mat[startVertex][i] == 1 && i != targetVertex && i == via)
				count = count + shortestCountFuncThroughK(i, targetVertex, via, true, maxLength-1, verCount, mat);
			else if(mat[startVertex][i] == 1 && i != targetVertex && i != via)
				count = count + shortestCountFuncThroughK(i, targetVertex, via, false, maxLength-1, verCount, mat);
		}
		return count;
	}else
		return 0;
};