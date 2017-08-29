#include <iostream>
#include "Coord_3d.cpp"
using namespace std;

class MyStack{
	public:
		MyStack(int capa =10){
			capacity = capa;
			if(capacity < 1) throw "Stack Capa Must Be > 0";
			try{
				stack = new Coord_3d[capacity];
			} catch(int e) {
				cout << "Stack allocation Failure";
			};
			currentTop = -1;
		};

		bool isEmpty(){return currentTop == -1;};
		bool isFull(){return currentTop == capacity -1;};
		Coord_3d top(){		
			return stack[currentTop];
		};
		void push(Coord_3d item){
			if( isFull() ){
				Coord_3d* newStack = NULL;
				try{
					newStack = new Coord_3d[capacity*2];
				}catch(int e){
					cout << "Stack allocation Failure";
				};
			
				for(int i = 0; i <capacity; i++)
					newStack[i] = stack[i];
		
				delete[] stack;
				stack = newStack;
				capacity *= 2;
			}
			currentTop = currentTop + 1;
			stack[currentTop] = item;
		};
		void pop(){
			if( isEmpty() ) throw "Stack is Empty, Pop() not Available";
			currentTop = currentTop -1;
			stack[currentTop].~Coord_3d();
		};

	private:
		Coord_3d* stack;
		int currentTop;
		int capacity;
};
