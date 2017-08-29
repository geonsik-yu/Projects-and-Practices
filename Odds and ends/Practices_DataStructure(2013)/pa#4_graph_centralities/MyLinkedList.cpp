#include <iostream>
using namespace std;
class MyNode{
public:
	MyNode( ) : instruction(NULL), number(0), link(NULL) { };
	MyNode( char* newInst, int instNum ) : link(NULL), number(instNum) { 
		instruction = new char[2];
		for(int i = 0; i < 2; i++)
			instruction[i] = newInst[i];
	};
	~MyNode(){
		delete instruction;
	};

	MyNode* getNext() {return link;};
	char* getInstruction(){return instruction;};
	int getNumber() {return number;};
	void setNext( MyNode* node ){ link = node;};
	void setInstruction( char* inst ){
		for(int i = 0; i < 2; i++)
			instruction[i] = inst[i];
	};
	void setNumber( int newNum ){ number = newNum; };

private:
	char* instruction;
	int number;
	MyNode* link;
};