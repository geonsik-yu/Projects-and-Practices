#include <iostream>
#include <fstream>
#include <string>
#include "SMMH.cpp"
#include "MMH.cpp"
using namespace std;

void main(){
	ifstream fin;
	ofstream fout;
	fin.open("c:\\ds\\input.txt");
	fout.open("c:\\ds\\hw6\\2008122092.txt");

	if( fin.is_open() )	// input file open Checker
		cout << "input file opened properly\n\n";
	else { cout << "file open failure\n"; exit(1); } 

	string operators[45];
	int operands[45];
	string resultStrSMMH[45];
	string resultStrMMH[45];

	for(int i = 0; i < 40; i++){
		operators[i] = "";
		operands[i] = 0;
		resultStrSMMH[i] = "";
		resultStrMMH[i] = "";
	}
	int index = 0;
	string str; // temporary string for data input
	while( !fin.eof() ){
		fin >> str;
		if(str == "Insert"){
			operators[index] = str;
			fin >> str;
			operands[index] = stoi(str);
		}else if(str == "DMin"||str == "DMax"||str == "FMin"||str == "FMax"
			||str == "Clear"||str == "Size"||str == "ASize"){
			operators[index] = str;
		}	
		index++;
	}
//	for(int i = 0; i < 40; i++){
//		cout << operators[i] << " " << operands[i] << "\n";
//	}

	//////////
	SMMH smmh = SMMH();
	MMH mmh = MMH();
	
	for(int i = 0; i < 40; i++){
		if(operators[i] == "Insert"){
			resultStrSMMH[i] = "Insert " + smmh.insert(operands[i]);
			resultStrMMH[i] = "Insert " + mmh.insert(operands[i]);
		}else if(operators[i] == "DMax"){
			resultStrSMMH[i] = "DeleteMax " + smmh.deleteMax();
			resultStrMMH[i] = "DeleteMax " + mmh.deleteMax();
		}else if(operators[i] == "DMin"){
			resultStrSMMH[i] = "DeleteMin " + smmh.deleteMin();
			resultStrMMH[i] = "DeleteMin " + mmh.deleteMin();
		}else if(operators[i] == "FMax"){
			resultStrSMMH[i] = "FindMax " + smmh.getMax();
			resultStrMMH[i] = "FindMax " + mmh.getMax();
		}else if(operators[i] == "FMin"){
			resultStrSMMH[i] = "FindMin " + smmh.getMin();
			resultStrMMH[i] = "FindMin " + mmh.getMin();
		}else if(operators[i] == "Clear"){
			smmh.resetSMMH();
			resultStrSMMH[i] = "Clear";
			mmh.resetMMH();
			resultStrMMH[i] = "Clear";
		}else if(operators[i] == "Size"){
			resultStrSMMH[i] = "Size " + smmh.getSize();
			resultStrMMH[i] = "Size " + mmh.getSize();
		}else if(operators[i] == "ASize"){
			resultStrSMMH[i] = "ArraySize " + smmh.getArraySize();
			resultStrMMH[i] = "ArraySize " + mmh.getArraySize();
		};
	}

	cout << "Symmetric Min-Max Heap\n";
	for(int i = 0; i < 40; i++){
		cout << resultStrSMMH[i] << "\n";
		fout << resultStrSMMH[i] << "\n";		
	}
	cout << "\nMin-Max Heap\n";
	for(int i = 0; i < 40; i++){
		cout << resultStrMMH[i] << "\n";
		fout << resultStrMMH[i] << "\n";
	}
	smmh.~SMMH();
	mmh.~MMH();
	fin.close();
	fout.close();
	cin >> index;
};