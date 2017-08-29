/*
 Title: 3D Maze, Programming Assignment (2013 Spring).
 Description: This c++ source code written by Geonsik Yu, for data structure programming practice.
 Note:
	- This programming practice is about finding path in 3D maze, given 20 test cases.
	- 3-dimensional maze:
		1. This programming pracice is about finding path in an 3d integer array (nxnxn).
		2. Starting point and exit are always (0,0,0) and (n-1, n-1, n-1), respectively.
		3. If the element's value is 1 then the location (block) is not accessible, and if it is 0 then the location (block) can be included as a part of valid path.
		4. There is no diagonal movement in this problem. That is, our path finder can only move to a block which shares an edge with the current block.
		5. For educational purpose, c++ data structures (e.g. stack) are not allowed.
		6. One of the shortest paths should be returned for each case.
	- Input:
		1. 20 sets of test cases are given.
		2. First number of each case is the dimension (n) and (n^2) rows of numbers are following for the case.
		3. In (n^2) rows, (n*i+j)th row's kth integer represents (i,j,k) location of the maze.
	- Output:
		1. If there exists a (0,0,0)-(n-1,n-1,n-1) path, then print out the shortest length of such paths.
 		2. Otherwise, print 0.
*/

#include <iostream>
#include <fstream>
#include <string>
#include "MyStack.cpp"
using namespace std;

void main(){
	ifstream fin;
	ofstream fout;
	fin.open("c:\\ds\\input.txt");
	fout.open("c:\\ds\\output.txt");

	if( !fin.is_open() ){ cout << "file open failure\n"; exit(1); }

	string str; // temporary string for data input
	string** strv = new string*[20]; // using dynamic arrays to store input data
	int index = 0;

	int* lengthArr = new int[20];
	int currentNumber = 0;

	while( !fin.eof() ){
		fin >> str;
		lengthArr[index] = atoi(str.c_str());
		strv[index] = new string[ lengthArr[index] * lengthArr[index] ];
		for( int i = 0; i < lengthArr[index] * lengthArr[index]; i++){
			fin >> str;
			strv[index][i] = str;
		}
		index++;
	};

	for(int m = 0; m < 20; m++){
		int length = lengthArr[m];

		int*** maze = new int**[length];
		for(int i = 0; i < length; i++){
			maze[i] = new int*[length];
			for(int j = 0; j < length; j++)
				maze[i][j] = new int[length];
		}

		string* temp = strv[m];
		int indexTemp = 0;
		for(int i = 0; i < length; i++)
			for(int j = 0; j < length; j++){
				for(int k = 0; k < length; k++){
					char tempChar = temp[indexTemp].at(k);
					char* tempP = &tempChar;
					maze[i][j][k] = atoi( tempP );
				}
				indexTemp++;
			}

		for(int i = 0; i < length; i++)
			for(int j = 0; j < length; j++)
				for(int k = 0; k < length; k++)
					maze[i][j][k] -= 2;

		MyStack stack_o = MyStack(30);
		MyStack stack_e = MyStack(30);

		maze[length-1][length-1][length-1] = 0;
		stack_o.push(Coord_3d(length-1,length-1,length-1));
		currentNumber = 1;

		while( maze[0][0][0] == -2 ){
			if(currentNumber % 2 == 0){
				if( stack_e.isEmpty() )
					break;
				while( !stack_e.isEmpty() ){
					Coord_3d target = stack_e.top();
					stack_e.pop();
					if( target.getX() -1 >= 0
						&& maze[target.getX() -1 ][target.getY() ][target.getZ() ] == -2 ){
					
						maze[target.getX() -1 ][target.getY() ][target.getZ() ] = currentNumber;
						stack_o.push( Coord_3d( target.getX() -1, target.getY(), target.getZ() ) );
					}
					if( target.getX() +1 < length
						&& maze[target.getX() +1 ][target.getY() ][target.getZ() ] == -2 ){

						maze[target.getX() +1 ][target.getY() ][target.getZ() ] = currentNumber;
						stack_o.push( Coord_3d( target.getX() +1, target.getY(), target.getZ() ) );
					}
					if( target.getY() -1 >= 0
						&& maze[target.getX() ][target.getY() -1 ][target.getZ() ] == -2 ){

						maze[target.getX() ][target.getY() -1 ][target.getZ() ] = currentNumber;
						stack_o.push( Coord_3d( target.getX(), target.getY() -1, target.getZ() ) );
					}
					if( target.getY() +1 < length 
						&& maze[target.getX() ][target.getY() +1 ][target.getZ() ] == -2 ){

						maze[target.getX() ][target.getY() +1 ][target.getZ() ] = currentNumber;
						stack_o.push( Coord_3d( target.getX(), target.getY() +1, target.getZ() ) );
					}
					if( target.getZ() -1 >= 0 
						&& maze[target.getX() ][target.getY() ][target.getZ() -1 ] == -2 ){
				
						maze[target.getX() ][target.getY() ][target.getZ() -1] = currentNumber;
						stack_o.push( Coord_3d( target.getX(), target.getY(), target.getZ() -1 ) );
					}
					if( target.getZ() +1 < length
						&& maze[target.getX() ][target.getY() ][target.getZ() +1 ] == -2 ){
				
						maze[target.getX() ][target.getY() ][target.getZ() +1] = currentNumber;
						stack_o.push( Coord_3d( target.getX(), target.getY(), target.getZ() +1 ) );
					}
				}

				currentNumber++;
			}else{
				if( stack_o.isEmpty() )
					break;
				while( !stack_o.isEmpty() ){
					Coord_3d target = stack_o.top();
					stack_o.pop();
					if( target.getX() -1 >= 0
						&& maze[target.getX() -1 ][target.getY() ][target.getZ() ] == -2 ){
					
						maze[target.getX() -1 ][target.getY() ][target.getZ() ] = currentNumber;
						stack_e.push( Coord_3d( target.getX() -1, target.getY(), target.getZ() ) );
					}
					if( target.getX() +1 < length
						&& maze[target.getX() +1 ][target.getY() ][target.getZ() ] == -2 ){

						maze[target.getX() +1 ][target.getY() ][target.getZ() ] = currentNumber;
						stack_e.push( Coord_3d( target.getX() +1, target.getY(), target.getZ() ) );
					}
					if( target.getY() -1 >= 0
						&& maze[target.getX() ][target.getY() -1 ][target.getZ() ] == -2 ){

						maze[target.getX() ][target.getY() -1 ][target.getZ() ] = currentNumber;
						stack_e.push( Coord_3d( target.getX(), target.getY() -1, target.getZ() ) );
					}
					if( target.getY() +1 < length 
						&& maze[target.getX() ][target.getY() +1 ][target.getZ() ] == -2 ){

						maze[target.getX() ][target.getY() +1 ][target.getZ() ] = currentNumber;
						stack_e.push( Coord_3d( target.getX(), target.getY() +1, target.getZ() ) );
					}
					if( target.getZ() -1 >= 0 
						&& maze[target.getX() ][target.getY() ][target.getZ() -1 ] == -2 ){
				
						maze[target.getX() ][target.getY() ][target.getZ() -1] = currentNumber;
						stack_e.push( Coord_3d( target.getX(), target.getY(), target.getZ() -1 ) );
					}
					if( target.getZ() +1 < length
						&& maze[target.getX() ][target.getY() ][target.getZ() +1 ] == -2 ){
				
						maze[target.getX() ][target.getY() ][target.getZ() +1] = currentNumber;
						stack_e.push( Coord_3d( target.getX(), target.getY(), target.getZ() +1 ) );
					}		
				}
				currentNumber++;
			}
		};

		if(maze[0][0][0] == -2 || maze[0][0][0] == -1){
			fout << "#" << m+1 <<" "<< 0 <<"\n";
		}else{
			fout << "#" << m+1 <<" "<< maze[0][0][0]+1 <<"\n";
		};


		for(int i=0; i<length; i++){
			  for(int j=0; j<length; j++)
					 delete[] maze[i][j];
			  delete[] maze[i];
		}
		delete[] maze;
	}
	

	for(int i = 0; i < 20; i++)
		delete[] strv[i];
	delete[] strv;
	
	delete[] lengthArr;
	fin.close();
	fout.close();
};
