/*
 Title: Viral Infection, Programming Assignment (2013 Spring).
 Description: This c++ source code written by Geonsik Yu, for data structure programming practice.
 Note:
	- This programming practice is about viral infection in social networks.
	- Viral infection:
		1. A social network is given and 
		2. Any nodes adjacent to a infected node at time (t) will be infected at time (t+1)
		3. Any infected node will not be affected by other nodes anymore.
		4. Any infected node will be recovered to normal after (r) units of time
 2.
 
		2. Starting point and exit are always (0,0,0) and (n-1, n-1, n-1), respectively.
		3. If the element's value is 1 then the location (block) is not accessible, and if it is 0 then the location (block) can be included as a part of valid path.
		4. There is no diagonal movement in this problem. That is, our path finder can only move to a block which shares an edge with the current block.
		5. For educational purpose, c++ data structures (e.g. stack) are not allowed.
		6. One of the shortest paths should be returned for each case.

		1.
 주어진 social network을 나타내는 graph에서, 초기에 (at t0 ) ‘virus’에
 발생하였을 때 일정 시간 후에 총 감염 노드 수를 출력하는 프로그램을 작성하시오. Infection process는 다음과 같다.
 1. 감염되지 않은 노드는 이웃하는 감염된 노드로부터 1 time unit 후에 virus 에 감염된다.
 2. 감염된 노드는 다른 감염된 노드에 의해 영향을 받지 않는다.
 3. 어떤노드이든지감염된지rtimeunit후에는정상이된다.단,정상이된시점에
 감염된 이웃 노드가 있더라도 그 즉시 감염되지 않으나, 다음 time unit 에 감염된 이웃 노드가 있다면 재 감염된다.
 
	- Input:
 1. 20 sets of test cases are given.
 2. First number of each case is the dimension (n) and (n^2 + 1) rows of numbers are following for the case.
 3. Following n numbers (1s and 0s) are lengths of links, in that corresponding order.
 4. Following n numbers are degrees of servos, in that corresponding order.
 5. Values are separated by a blank.
	- Output:
		1. If any part of the robot arm is below the factory's ground, then print "1".
		2. If any pair of links are intersected, then print "2".
		3. Otherwise, print robot arm's cartesian coordinates.
*/


#include <iostream>
#include <fstream>
#include <string>
#include "Node.cpp"
using namespace std;

void main(){
	ifstream fin;
	ofstream fout;
	fin.open("c:\\ds\\input.txt");
	fout.open("c:\\ds\\output.txt");

	if( fin.is_open() )	// input file open Checker
		cout << "input file opened properly\n";
	else { cout << "file open failure\n"; exit(1); } 

	int* matrixSize	  = new int[20];	// "n" : size of adjacent matrix
	int* recoveryList = new int[20];	// "r" : recovery time delay
	int* caseFlagList = new int[20];	// if(r = 0) flag = 0; else flag = 1;
	int* firstInfected= new int[20];	// "first infected node index"
	int* targetTime	  = new int[20];	// "t" : target time(only when flag = 0)
	int*** adjacentMat = new int**[20];	// "adjacent matrices x20"
	for(int i = 0; i < 20; i++){// array initialization
		matrixSize[i]  = 0;		recoveryList[i] = 0;
		caseFlagList[i]= 0;		firstInfected[i]= 0;
		targetTime[i]  = 0;
	};

	int index = 0;
	string str; // temporary string for data input
	while( !fin.eof() ){
		fin >> str;			matrixSize[index]	= atoi(str.c_str());
		fin >> str;			recoveryList[index]	= atoi(str.c_str());
							bool tempFlag = !(recoveryList[index] == 0);
							caseFlagList[index]	= tempFlag;
		fin >> str;			firstInfected[index]= atoi(str.c_str());
		if(tempFlag != 0){
				fin >> str; 
				targetTime[index] = atoi(str.c_str());
		};

		int tempSize = matrixSize[index];
		adjacentMat[index] = new int*[  tempSize  ];
		for(int i = 0; i < tempSize; i++)
			adjacentMat[index][i] = new int[ tempSize ];
	
		string testTemp = "";
		fin >> testTemp;
		str = testTemp;
		if(testTemp.length() == 1 && tempSize != 1){
			// when input string for adjacent matrix is like "1 0 1 0 1 0 1 0"
			for(int j = 0; j < tempSize; j++){
				for(int k = 0; k < tempSize; k++){
					adjacentMat[index][j][k] =  atoi(str.c_str());
					if( j != tempSize - 1 || k != tempSize - 1)
						fin >> str;
				};
			};
		}else{
			// when input string for adjacent matrix is like "1010101010101010"
			for(int j = 0; j < tempSize; j++){
				for(int k = 0; k < tempSize; k++){
					char tempChar = str.at(k);
					char* tempP = &tempChar;
					adjacentMat[index][j][k] = atoi(tempP);
				};
				if( j != tempSize - 1 )
					fin >> str;
			};
		};
		index++;
	};

//	for(int i = 0; i < 20; i++){
//		cout << matrixSize[i] << " " << recoveryList[i]
//			<< " " << caseFlagList[i] << " " <<	firstInfected[i]
//			<< " " << targetTime[i] << "\n";
//		int temp = matrixSize[i];
//		for(int j = 0; j < temp; j++){
//			for(int k = 0; k < temp; k++)
//				cout << adjacentMat[i][j][k];
//			cout << "\n";
//		};
//	};

	for( int i = 0; i < 20; i++ ){
		if(caseFlagList[i] == 0){
			// r = 0 ¿Œ ∞ÊøÏ
			// ¿¸√º ≥ÎµÂ∏¶ ¿¸∫Œ ∞®ø∞Ω√≈≥ ∂ß±Ó¡ˆ ∞…∏Æ¥¬ Ω√∞£¿ª √‚∑¬(t)
			// ∞Ì∏≥µ» ≥ÎµÂ∞° ¡∏¿Á«“ ∞ÊøÏ -1 ¿ª √‚∑¬
			int tempSize = matrixSize[i];
			int* isolationList = new int[tempSize];
			int isolation = 1;	// √÷¡æ ø¨ªÍø°º≠ 0¿Ã∏È ∞Ì∏≥ ≥ÎµÂ∞° ¡∏¿Á
			for( int j = 0; j < tempSize; j++){
				isolationList[j] = 0;
				for( int k = 0; k < tempSize; k++ ){
					isolationList[j] = (isolationList[j] || adjacentMat[i][j][k]);
				}
				isolation = (isolation && isolationList[j]);
			}
			delete[] isolationList;

			if(isolation == 0){ //∞Ì∏≥ ≥ÎµÂ∞° ¡∏¿Á«œ¥¬ ∞ÊøÏ
				cout << "#" << i+1 << " -1\n";
				fout << "#" << i+1 << " -1\n";
			}else{				
				
				//∞Ì∏≥ ≥ÎµÂ∞° ¡∏¿Á«œ¡ˆ æ æ∆º≠, tΩ√∞£ »ƒø° π›µÂΩ√ ¿¸√º ∞®ø∞¿Ã πﬂª˝«œ¥¬ ∞ÊøÏ
				int timer = 0;
				int infectionCounter = 1;
				Node* nodeList = new Node[tempSize];
				for(int m = 0; m < tempSize; m++)
					nodeList[m] = Node();//initialization
				nodeList[firstInfected[i]].setInfected(1);	// first infection.
				while(infectionCounter < tempSize){
					Node* nodeListDup = new Node[tempSize];
					for(int k = 0; k < tempSize; k++) //duplicate nodeList
						nodeListDup[k].setInfected(nodeList[k].getInfected());

					for(int k = 0; k < tempSize; k++){
						if(nodeList[k].getInfected() == 1){
							for(int m = 0; m < tempSize; m++){
								if(adjacentMat[i][k][m] == 1 && nodeList[m].getInfected() != 1){
									nodeListDup[m].setInfected(1);
									infectionCounter++;
								}
							}//End of for -- infection Search
						}
					}//End of for -- one time period infection complete
					
					//transfer Dup -> Original nodeList
					for(int k = 0; k < tempSize; k++)
						nodeList[k].setInfected(nodeListDup[k].getInfected());
					delete[] nodeListDup;
					timer++;
				}//End of while
				cout << "#" << i+1 << " t:" << timer << "\n";
				fout << "#" << i+1 << " " <<timer << "\n";
			}
		}else{
		// r > 0 ¿Œ ∞ÊøÏ
		// t Ω√∞£ »ƒø° ∞®ø∞µ» ≥ÎµÂ¿« ºˆ∏¶ √‚∑¬
			int tempSize = matrixSize[i];
			int timer = 0;
			int targetT = targetTime[i];
			int recover = recoveryList[i];

			Node* nodeList = new Node[tempSize];
			for(int m = 0; m < tempSize; m++)
				nodeList[m] = Node();//initialization
			nodeList[firstInfected[i]].setInfected(1);	// first infection.
			nodeList[firstInfected[i]].setRecClock(recover); // clock init.

			while(timer < targetT){
				Node* nodeListDup = new Node[tempSize];
				for(int k = 0; k < tempSize; k++){ //duplicate nodeList
					nodeListDup[k].setInfected(nodeList[k].getInfected());
					nodeListDup[k].setRecClock(nodeList[k].getRecClock());
				}
			
				timer++;//Ω√∞£¿Ã »Â∏•¥Ÿ.
				for(int k = 0; k < tempSize; k++){
					if(nodeList[k].getInfected() == 1){ // ∞®ø∞µ» ≥ÎµÂ∞° ¿÷¥Ÿ∏È,
						nodeListDup[k].setRecClock(nodeList[k].getRecClock() - 1); // recovery Ω√∞Ë∞° øÚ¡˜¿Ã∞Ì,
						
						if(nodeList[k].getRecClock() > 0){// ∞®ø∞ ≥ÎµÂ ¡ﬂø°º≠ Ω√∞£¿Ã æ»¡ˆ≥≠ ≥‡ºÆµÈ¿∫ ø©¿¸»˜ ∞®ø∞ Ω√≈¥.
							for(int m = 0; m < tempSize; m++){
								if(adjacentMat[i][k][m] == 1 && nodeList[m].getInfected() == 0){
									// ¿ÃøÙ«œ∞Ì ∞®ø∞¿Ã æ»µ» ≥‡ºÆµÈ¿∫ ¥Ÿ ∞®ø∞Ω√ƒ—πˆ∏±≈◊¥Ÿ.
									nodeListDup[m].setInfected(1);
									nodeListDup[m].setRecClock(recover);
								}
							}
						}
					}
				}//End of for -- one time period infection complete
				for(int k = 0; k < tempSize; k++)
					if(nodeListDup[k].getInfected() == 1 && nodeListDup[k].getRecClock() == 0){
						//ø¿ø∞ªÛ≈¬¿Ã¡ˆ∏∏ »∏∫π±‚∞£¿Ã ¥Ÿ ¡ˆ≥≠ ≥‡ºÆµÈ¿∫ ∞®ø∞ »∞µøæ¯¿Ã »∏∫π, ±◊∏Æ∞Ì »∏∫πµ» Ω√¡°ø°º≠¥¬ ¿Á∞®ø∞ æ»µ .
						nodeListDup[k].setInfected(0);					
					}
						
				//transfer Dup -> Original nodeList
				for(int k = 0; k < tempSize; k++){
					nodeList[k].setInfected(nodeListDup[k].getInfected());
					nodeList[k].setRecClock(nodeListDup[k].getRecClock());
				}
				delete[] nodeListDup;
			}//End of while

			int tempCount = 0;
			for(int k = 0; k < tempSize; k++){
				if(nodeList[k].getInfected() == 1)
					tempCount++;
			}

			cout << "#" << i+1 << " n:" << tempCount << "\n";
			fout << "#" << i+1 << " " << tempCount << "\n";
		}
			
	};


	
	for(int i = 0; i < matrixSize[i]; i++){
		for(int j = 0; j < matrixSize[i]; j++)
			delete[] adjacentMat[i][j];
		delete[] adjacentMat[i];
	}
	delete[] adjacentMat;

	delete[] matrixSize;
	delete[] recoveryList;
	delete[] caseFlagList;
	delete[] firstInfected;
	delete[] targetTime;


	fin.close();
	fout.close();


	int i;
	cin >> i;
};

