#include <cmath>
class Node{
public:
	Node(): infected(0), recoveryClock(0) {};
	Node(int infState, int clockState){
		infected		= infState;
		recoveryClock	= clockState;
	}; 
	~Node() { };

	int getInfected(){ return infected; };
	int getRecClock(){ return recoveryClock; };
	void setInfected(int infState){ infected = infState; };
	void setRecClock(int clocksta){ recoveryClock = clocksta; };

private:
	int infected;	// if( viral infected ) infected = 1; else infected = 0;
	int recoveryClock;	// when infected, Clock = r; and for every phase, Clock--; 
						//		if(Clock == 0 && infected ==1) infected = 0; 	
};

