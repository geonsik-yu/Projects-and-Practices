#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
class SMMH{
private:
	int lastLocation;
	int arrayLength;
	int* arr;		// array => only positive integers
				// array used from index 2(min) to .... (0 and 1 are empty)
public:
	SMMH(int initialCapacity = 2){//init Capacity > 0, exception not considered
		arrayLength = initialCapacity + 2;
		arr = new int[arrayLength];
		for(int i = 0; i < arrayLength; i++)
			arr[i] = 0;
		lastLocation = 1;
	};
	~SMMH(){
		delete arr;
	};
	void resetSMMH(){
		delete[] arr;
		arrayLength = 4;
		arr = new int[arrayLength];
		for(int i = 0; i < arrayLength; i++)
			arr[i] = 0;
		lastLocation = 1;
	};
	string getArraySize(){ return IntToString(arrayLength-2); };
	string getSize(){ return IntToString(lastLocation-1); };
	string getMin(){
		if(lastLocation == 1) throw "wrong command";
		return IntToString(arr[2]);
	};
	string getMax(){
		if(lastLocation == 1) throw "wrong command";
		if(lastLocation == 2) 
			return IntToString(arr[2]);
		else return IntToString(arr[3]);
	};
	string insert(int value){
		if(lastLocation +2 == arrayLength)
			extendArr();
		int currentNode = ++lastLocation;
		if(lastLocation % 2 == 1 && value < arr[lastLocation - 1]){
			// current location is right child and value is smaller then left child
			arr[lastLocation] = arr[lastLocation - 1];
			currentNode--;
		}

		bool done = false;
		while(!done && currentNode >= 4){
			int gp = currentNode / 4;
			int leftChild_gp  = 2*gp;
			int rightChild_gp = leftChild_gp + 1;
			if( value < arr[leftChild_gp] ){
				arr[currentNode] = arr[leftChild_gp];
				currentNode = leftChild_gp;
			}
			else if(value > arr[rightChild_gp]){
				arr[currentNode] = arr[rightChild_gp];
				currentNode = rightChild_gp;
			}
			else done = true;
		}
		arr[currentNode] = value;

		string str = IntToString(lastLocation - 1) + " {";
		for(int i = 2; i <= lastLocation; i++){
			str = str + IntToString(arr[i]);
			if(i != lastLocation) str = str + " ";
			else str = str + "}";
		}
		return str;
	};
	string deleteMin(){
		int value = arr[lastLocation];
		lastLocation--;
		int currentNode = 2;// min index;
		bool done = false;

		while(!done){
			int gp = currentNode / 4;
			int P_LCLC = (currentNode / 2) * 4;
			int P_RCLC = (currentNode / 2) * 4 + 2;

			if( lastLocation < P_LCLC ){
				arr[currentNode] = value;
				if( currentNode +1 <= lastLocation && arr[currentNode] > arr[currentNode+1] ){
					int temp = arr[currentNode];
					arr[currentNode] = arr[currentNode+1];
					arr[currentNode+1] = temp;
				}
				done = true;
			}else if( lastLocation < P_RCLC ){
				if( value >= arr[P_RCLC] ){
					arr[currentNode] = arr[P_LCLC];
					currentNode = P_LCLC;
				}else{
					arr[currentNode] = value;
					done = true;				
				}
			}else{
				if( arr[P_LCLC] < arr[P_RCLC] && value >= min(arr[P_LCLC], arr[P_RCLC]) ){
					arr[currentNode] = arr[P_LCLC];
					currentNode = P_LCLC;		
				}else if( arr[P_LCLC] >= arr[P_RCLC] && value >= min(arr[P_LCLC], arr[P_RCLC]) ){
					arr[currentNode] = arr[P_RCLC];
					currentNode = P_RCLC;				
				}else{
					arr[currentNode] = value;
					done = true;
				}
			}
		}
		string str = IntToString(lastLocation - 1) + " {";
		for(int i = 2; i <= lastLocation; i++){
			str = str + IntToString(arr[i]);
			if(i != lastLocation) str = str + " ";
			else str = str + "}";
		}
		if(lastLocation == 1)
			str = str + "}";
		return str;
	};

	string deleteMax(){
		int value = arr[lastLocation];
		lastLocation--;
		int currentNode = 3;// max index;
		bool done = false;

		while(!done){
			int gp = currentNode / 4;
			int P_LCRC = (currentNode / 2) * 4 + 1;
			int P_RCRC = (currentNode / 2) * 4 + 3;

			if( lastLocation < P_LCRC ){
				arr[currentNode] = value;
				if( arr[currentNode] < arr[currentNode-1] ){
					int temp = arr[currentNode];
					arr[currentNode] = arr[currentNode+1];
					arr[currentNode+1] = temp;
				}
				done = true;
			}else if( lastLocation < P_RCRC ){
				if( value <= arr[P_LCRC] ){
					arr[currentNode] = arr[P_LCRC];
					currentNode = P_LCRC;
				}else{
					arr[currentNode] = value;
					done = true;	
				}
			}else{
				if( arr[P_LCRC] < arr[P_RCRC] && value <= max(arr[P_LCRC], arr[P_RCRC]) ){
					arr[currentNode] = arr[P_RCRC];
					currentNode = P_RCRC;		
				}else if( arr[P_LCRC] >= arr[P_RCRC] && value <= max(arr[P_LCRC], arr[P_RCRC]) ){
					arr[currentNode] = arr[P_LCRC];
					currentNode = P_LCRC;				
				}else{
					arr[currentNode] = value;
					done = true;
				}
			}
		}
		string str = IntToString(lastLocation - 1) + " {";
		for(int i = 2; i <= lastLocation; i++){
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

