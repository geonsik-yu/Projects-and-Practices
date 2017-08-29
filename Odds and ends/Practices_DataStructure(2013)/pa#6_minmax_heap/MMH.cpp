#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
class MMH{
private:
	int lastLocation;
	int arrayLength;
	int* arr;		// array => only positive integers
				// array used from index 2(min) to .... (0 and 1 are empty)
public:
	MMH(int initialCapacity = 3){//init Capacity > 0, exception not considered
		arrayLength = initialCapacity + 1;
		arr = new int[arrayLength];
		for(int i = 0; i < arrayLength; i++)
			arr[i] = 0;
		lastLocation = 0; /// 1번부터 사용한다.
	};
	~MMH(){
		delete arr;
	};
	void resetMMH(){
		delete[] arr;
		arrayLength = 4;
		arr = new int[arrayLength];
		for(int i = 0; i < arrayLength; i++)
			arr[i] = 0;
		lastLocation = 0;
	};
	string getArraySize(){ return IntToString(arrayLength-1); };
	string getSize(){ return IntToString(lastLocation); };
	string getMin(){
		if(lastLocation == 0) throw "wrong command";
		return IntToString(arr[1]);
	};
	string getMax(){
		if(lastLocation == 0) throw "wrong command";
		else return IntToString(max(arr[2], arr[3]));
	};

	string insert(int value){
		if(lastLocation +2 == arrayLength)
			extendArr();
		int currentNode = ++lastLocation;// 맨 뒤에 집어넣는다 일단...
		arr[currentNode] = value;

		bool done = false;
		while( !done ){			
			/// level check
			double pivot = currentNode;
			int minMax = 1; // -1(min), +1(max)
			while( pivot >= 1){
				minMax = -minMax;
				pivot = pivot / 2;
			};

			if(minMax == -1){// currently minimum level
				if( currentNode > 1 && arr[currentNode] > arr[currentNode/2] ){ 
					// 아빠(max)보다 크다.
					int temp = arr[currentNode];
					arr[currentNode] = arr[currentNode/2];
					arr[currentNode/2] = temp;
					currentNode = currentNode/2;
				}else if(currentNode > 3 && arr[currentNode] < arr[(currentNode/2)/2]){ 
					// 할아버지(min)보다 작다 
					int temp = arr[currentNode];
					arr[currentNode] = arr[(currentNode/2)/2];
					arr[(currentNode/2)/2] = temp;
					currentNode = (currentNode/2)/2;			
				}else
					done = true;
			}else{// currently maximum level
				if( currentNode > 1 && arr[currentNode] < arr[currentNode/2] ){
					// 아빠(min)보다 작다.
					int temp = arr[currentNode];
					arr[currentNode] = arr[currentNode/2];
					arr[currentNode/2] = temp;
					currentNode = currentNode/2;
				}else if( currentNode > 3 && arr[currentNode] > arr[(currentNode/2)/2]){
					// 할아버지(max)보다 크다.
					int temp = arr[currentNode];
					arr[currentNode] = arr[(currentNode/2)/2];
					arr[(currentNode/2)/2] = temp;
					currentNode = (currentNode/2)/2;			
				}else
					done = true;			
			};
		};
		string str = IntToString(lastLocation) + " {";
		for(int i = 1; i <= lastLocation; i++){
			str = str + IntToString(arr[i]);
			if(i != lastLocation) str = str + " ";
			else str = str + "}";
		}
		return str;
	};

	string deleteMin(){
		int value = arr[lastLocation];
		lastLocation--;
		int currentNode = 1;// min index;

		bool done = false;
		while( !done ){
			int target[6];
			target[0] = 2 * currentNode;		//left child
			target[1] = 2 * currentNode + 1;	//right child
			target[2] = 2 * (target[0]);		//leftleft gc
			target[3] = 2 * (target[0]) + 1;	//leftright gc
			target[4] = 2 * (target[1]);
			target[5] = 2 * (target[1]) + 1;

			if(currentNode*2 <= lastLocation){
				int minimumChild = 2 * currentNode;//default = leftChild
				for(int i = 0; i < 6; i++){
					if( target[i] <= lastLocation && arr[minimumChild] > arr[target[i]] ){
						minimumChild = target[i];
					}
				}
				if(value > arr[minimumChild]){
					arr[currentNode] = arr[minimumChild];
					currentNode = minimumChild;
				}else{
					arr[currentNode] = value;
					done = true;
				}
			}else{
				arr[currentNode] = value;
				done = true;
			}
		}
			
		string str = IntToString(lastLocation) + " {";
		for(int i = 1; i <= lastLocation; i++){
			str = str + IntToString(arr[i]);
			if(i != lastLocation) str = str + " ";
			else str = str + "}";
		}
		if(lastLocation == 0)
			str = str + "}";

		return str;
	};

	string deleteMax(){
		int currentNode = 0;
		if(lastLocation == 2){
			currentNode = 2;			
		}else if(lastLocation > 2 && arr[2] > arr[3]){
			currentNode = 2;
		}else
			currentNode = 3;

		int value = arr[lastLocation];
		lastLocation--;

		bool done = false;
		while( !done ){
			int target[6];
			target[0] = 2 * currentNode;		//left child
			target[1] = 2 * currentNode + 1;	//right child
			target[2] = 2 * (target[0]);		//leftleft gc
			target[3] = 2 * (target[0]) + 1;	//leftright gc
			target[4] = 2 * (target[1]);
			target[5] = 2 * (target[1]) + 1;

			if(currentNode*2 <= lastLocation){
				int maximumChild = 2 * currentNode;//default = leftChild
				for(int i = 0; i < 6; i++){
					if( target[i] <= lastLocation && arr[maximumChild] < arr[target[i]] ){
						maximumChild = target[i];
					}
				}
				if(value < arr[maximumChild]){
					arr[currentNode] = arr[maximumChild];
					currentNode = maximumChild;
				}else{
					arr[currentNode] = value;
					done = true;
				}
			}else{
				arr[currentNode] = value;
				done = true;
			}
		}
			
		string str = IntToString(lastLocation) + " {";
		for(int i = 1; i <= lastLocation; i++){
			str = str + IntToString(arr[i]);
			if(i != lastLocation) str = str + " ";
			else str = str + "}";
		}
		return str;
	};

	void extendArr(){
		int newArrayLength = arrayLength + (arrayLength - 2) * 2;
		int* newArr = new int[newArrayLength];
		for(int i = 0; i < newArrayLength; i++)
			newArr[i] = 0;
		for(int i = 0; i < arrayLength; i++)
			newArr[i] = arr[i];
		arrayLength = newArrayLength;
		delete[] arr;
		arr = newArr;
	};

	string IntToString (int a){
	    ostringstream temp;
	    temp << a;
	    string str = temp.str();
	    return str;
	};
};

