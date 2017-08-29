#include <iostream>
using namespace std;

class MyStack{
	public:
		MyStack(int capa =10){
			capacity = capa;
			if(capacity < 1) throw "Stack Capa Must Be > 0";
			try{
				stack = new int[capacity];
			} catch(int e) {
				cout << "Stack allocation Failure";
			};
			currentTop = -1;
		};

		bool isEmpty(){return currentTop == -1;};
		bool isFull(){return currentTop == capacity -1;};
		int top(){		
			return stack[currentTop];
		};
		void push(int item){
			if( isFull() ){
				int* newStack = NULL;
				try{
					newStack = new int[capacity*2];
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
			stack[currentTop].~int();
		};

	private:
		int* stack;
		int currentTop;
		int capacity;
};
