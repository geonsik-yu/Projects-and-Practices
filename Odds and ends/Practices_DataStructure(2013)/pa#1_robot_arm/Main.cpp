/*
 Title: Robot Arm Manipulation, Programming Assignment (2013).
 Description: This c++ source code written by Geonsik Yu, for data structure programming practice.
 Note:
	- This programming practice is about manipulating virtual robot arm.
	- Robot arm:
		1. Given robot arm is composed of n links (with length L) and each link has a servo (mechanic joint).
 		2. Servo-1 is attached to the ground of our virtual factory (xy-plane).
		3. Every (i)th link has a length value L(i).
 		4. Every (i)th link is connected to servo-(i) in one end and servo-(i+1) in the other.
 		5. Since there is no servo-(n), (n)th link is connected to the robot hand instead.

	       (servo-1) ===link-1=== (servo-2) ===link-2=== (servo-3) ===link-3=== ... ===link-n=== (Hand)

 		6. Servo-(i) is rotatable parallel to xz-plane (which is redefined by previous rotations) 
			when i is odd and rotatable parallel to yz-plane when i is even.
		7. Degrees of rotations are in [-90, 90].
		8. Every link is independent to following servos' rotations.
		9. Initially, every link is perpendicular to the ground.
	- Input:
		1. 20 sets of setting and instructions are given.
		2. First number of each case is the number of links (n)
		3. Following n numbers are lengths of links, in that corresponding order.
		4. Following n numbers are degrees of servos, in that corresponding order.
		5. Values are separated by a blank.
	- Output:
		1. If any part of the robot arm is below the factory's ground, then print "1".
		2. If any pair of links are intersected, then print "2".
		3. Otherwise, print robot arm's cartesian coordinates.
*/

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "Coord_3d.cpp"
using namespace std;

Coord_3d rotator(Coord_3d Point, Coord_3d axis, long double theta);
// x axis rotation = true, y axis rotation = false;
bool err1Checker(Coord_3d* coordArray, int totalNumber);
bool err2Checker(Coord_3d* coordArray, int totalNumber);
long double distanceBetweenLines(Coord_3d P0, Coord_3d P1, Coord_3d Q0, Coord_3d Q1);

bool intersectionChecker(Coord_3d P0, Coord_3d P1, Coord_3d Q0, Coord_3d Q1);
long double norm(Coord_3d v);
long double dot(Coord_3d u, Coord_3d v);
Coord_3d cross(Coord_3d u, Coord_3d v);
bool dependencyCheck(Coord_3d A, Coord_3d B);

void main(){
	ifstream fin;
	ofstream fout;
	fin.open("c:\\ds\\input.txt");
	fout.open("c:\\ds\\output.txt");

	if( fin.is_open() )	{ cout << "input file opened properly\n"; }
	else { cout << "file open failure\n"; exit(1); } 

	string str;
	vector<string>* strv = new vector<string>[30];
	int index = 0;
	while( !fin.eof() ){
		fin >> str;
		int count = atoi(str.c_str()) * 2;
		for( int i = 0; i < count; i++){
			fin >> str;
			strv[index].push_back(str);
		}
		index++;
	}

	for(int i = 0; i < 20; i++){
		/// Local Variable initializations
		int totalNumber = strv[i].size() / 2;
		long double* lengthArray = new long double[totalNumber];
		long double* angleArray	 = new long double[totalNumber];
		Coord_3d*    coordArray  = new Coord_3d[totalNumber];
		Coord_3d axisX = Coord_3d(1, 0, 0); // Using in Axis Change : X Axis
		Coord_3d axisY = Coord_3d(0, 1, 0); // Using in Axis Change : Y Axis
		Coord_3d axisZ = Coord_3d(0, 0, 1); // Using in Axis Change : Z Axis
		bool flagErr_1 = false;				// Error Type 1 Flag
		bool flagErr_2 = false;				// Error Type 2 Flag

		long double currentLength = 0;
		for(int j = 0; j < totalNumber; j++){	// initializing Link Length Array
			lengthArray[j] = atof( strv[i].at(j).c_str() );
			currentLength += lengthArray[j];
			coordArray[j] = Coord_3d(0, 0, currentLength);
		}
		for(int j = 0; j < totalNumber; j++)	// initializing Servo Angle Array
			angleArray[j] = atof( strv[i].at(totalNumber + j).c_str() );  

		for(int j = 0; j < totalNumber; j++){
			for(int k = j; k < totalNumber; k++){
				Coord_3d currentlyMoving = coordArray[k];
				Coord_3d recentlyMoved   = Coord_3d(0, 0, 0);
				if(j != 0)	recentlyMoved= coordArray[j-1];

				currentlyMoving.setX( currentlyMoving.getX() - recentlyMoved.getX() );
				currentlyMoving.setY( currentlyMoving.getY() - recentlyMoved.getY() );
				currentlyMoving.setZ( currentlyMoving.getZ() - recentlyMoved.getZ() );

				if( (j+1)%2 == 1 )
					currentlyMoving = rotator(currentlyMoving, axisY, angleArray[j]);
				else
					currentlyMoving = rotator(currentlyMoving, axisX, angleArray[j]);

				currentlyMoving.setX( currentlyMoving.getX() + recentlyMoved.getX() );
				currentlyMoving.setY( currentlyMoving.getY() + recentlyMoved.getY() );
				currentlyMoving.setZ( currentlyMoving.getZ() + recentlyMoved.getZ() );			

				coordArray[k] = currentlyMoving;
			}

			if( (j+1)%2 == 1 ){	// rotating axes
				axisX = rotator(axisX, axisY, angleArray[j]);
				axisZ = rotator(axisZ, axisY, angleArray[j]);
			}else{
				axisY = rotator(axisY, axisX, angleArray[j]);
				axisZ = rotator(axisZ, axisX, angleArray[j]);
			}
		}

		// error type #1 check
		flagErr_1 = err1Checker(coordArray, totalNumber);
		// type #2 check
		flagErr_2 = err2Checker(coordArray, totalNumber);
	
		if( flagErr_1 == false && flagErr_2 == false ){
			for(int j = 0; j < totalNumber; j++){
				coordArray[j].roundC();
				if( j == totalNumber-1 ){
					fout.setf(ios::fixed);
					fout.setf(ios::showpoint);
					fout.precision(3);
					fout << "#" << i+1 << " " 
						<< coordArray[j].getX() << " " 
						<< coordArray[j].getY() << " " 
						<< coordArray[j].getZ() << "\n";
				}
 			}
		}else{
			if( flagErr_1 == true )
				fout << "#" << i+1 << " 1\n";
			else					
				fout << "#" << i+1 << " 2\n";
		}
		delete[] coordArray;
		delete[] angleArray;
		delete[] lengthArray;
	}

	delete[] strv;
	fin.close();
	fout.close();
};

Coord_3d rotator(Coord_3d point, Coord_3d axis, long double theta){
	Coord_3d result = Coord_3d();
	theta = (theta * M_PI) / 180.0; // From Degree To Radian 

	// By Arbitrary Axis Rotation Matrix from Wiki
	result.setX(
		( cos(theta) + axis.getX() * axis.getX() * (1-cos(theta)) ) * point.getX()
	  + ( axis.getX() * axis.getY() * (1-cos(theta)) - axis.getZ() * sin(theta) ) * point.getY()
	  + ( axis.getX() * axis.getZ() * (1-cos(theta)) + axis.getY() * sin(theta) ) * point.getZ()
		);
	result.setY(
		( axis.getY() * axis.getX() * (1-cos(theta)) + axis.getZ() * sin(theta) ) * point.getX()
	  + ( cos(theta) + axis.getY() * axis.getY() * (1-cos(theta)) ) * point.getY()
	  + ( axis.getY() * axis.getZ() * (1-cos(theta)) - axis.getX() * sin(theta) ) * point.getZ()
		);
	result.setZ(
		( axis.getZ() * axis.getX() * (1-cos(theta)) - axis.getY() * sin(theta) ) * point.getX()
	  + ( axis.getZ() * axis.getY() * (1-cos(theta)) + axis.getX() * sin(theta) ) * point.getY()
	  + ( cos(theta) + axis.getZ() * axis.getZ() * (1-cos(theta)) ) * point.getZ()
		);
	return result;
};

bool err1Checker(Coord_3d* coordArray, int totalNumber){
	bool flag = false;
	for(int i = 0; i < totalNumber; i++){
		if( flag == true )
			continue;
		if( coordArray[i].getZ() < 0.0)
			flag = true;
	}
	return flag;
};

bool err2Checker(Coord_3d* coordArray, int totalNumber){
	bool flag = false;
	for(int j = 0; j < totalNumber - 1; j++)
		flag = intersectionChecker(Coord_3d(0,0,0), coordArray[0], coordArray[j], coordArray[j+1]);
	for(int i = 0; i < totalNumber - 1; i++){
		if(flag == true)
			continue;
		for(int j = i+1; j < totalNumber - 1; j++)
			flag = intersectionChecker(coordArray[i], coordArray[i+1], coordArray[j], coordArray[j+1]);
	}
	return flag;
};

bool intersectionChecker(Coord_3d P0, Coord_3d P1, Coord_3d Q0, Coord_3d Q1){
	Coord_3d Q0minusP0 = Coord_3d( Q0.getX() - P0.getX()
								 , Q0.getY() - P0.getY()
								 , Q0.getZ() - P0.getZ() );
	Coord_3d v_P = Coord_3d( P1.getX() - P0.getX()
						   , P1.getY() - P0.getY()
						   , P1.getZ() - P0.getZ() );
	Coord_3d v_Q = Coord_3d( Q1.getX() - Q0.getX()
						   , Q1.getY() - Q0.getY()
						   , Q1.getZ() - Q0.getZ() );
	Coord_3d interPoint = Coord_3d();
	
	if( dependencyCheck(cross(Q0minusP0, v_Q), cross(v_P, v_Q)) ){
		long double t = 0;

		if(abs(cross(Q0minusP0, v_Q).getX()) < 0.000001 && abs(cross(v_P, v_Q).getX() < 0.000001)
			&& abs(cross(Q0minusP0, v_Q).getY()) < 0.000001 && abs(cross(v_P, v_Q).getY() < 0.000001)
			&& abs(cross(Q0minusP0, v_Q).getZ()) < 0.000001 && abs(cross(v_P, v_Q).getZ() < 0.000001)){
			return false;
		}
		else if(abs(cross(Q0minusP0, v_Q).getX()) > 0.000001 && abs(cross(v_P, v_Q).getX()) > 0.000001 )
			t = cross(Q0minusP0, v_Q).getX() / cross(v_P, v_Q).getX();
		else if(abs(cross(Q0minusP0, v_Q).getY()) > 0.000001 && abs(cross(v_P, v_Q).getY()) > 0.000001 )
			t = cross(Q0minusP0, v_Q).getY() / cross(v_P, v_Q).getY();
		else if(abs(cross(Q0minusP0, v_Q).getZ()) > 0.000001 && abs(cross(v_P, v_Q).getZ()) > 0.000001 )
			t = cross(Q0minusP0, v_Q).getZ() / cross(v_P, v_Q).getZ();
		interPoint = Coord_3d(P0.getX() + t*v_P.getX(), P0.getY() + t*v_P.getY(), P0.getZ() + t*v_P.getZ());
		if( (interPoint.getX() > min(P0.getX(), P1.getX()) && interPoint.getX() < max(P0.getX(), P1.getX()))
			|| (interPoint.getX() > min(Q0.getX(), Q1.getX()) && interPoint.getX() < max(Q0.getX(), Q1.getX())) ){
			return true;
		}
	}
	return false;
};

long double norm(Coord_3d v){ // Magnitude of Vector
	return sqrt( abs(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ()) );
};

long double dot(Coord_3d u, Coord_3d v) { // dot product of two vectors
	return u.getX() * v.getX() + u.getY() * v.getY() + u.getZ() * v.getZ();   
};
Coord_3d cross(Coord_3d u, Coord_3d v){ // cross product of two vectors
	return Coord_3d(
			u.getY() * v.getZ() - v.getY() * u.getZ()
		  , u.getZ() * v.getX() - v.getZ() * u.getX()
		  , u.getX() * v.getY() - v.getX() * u.getY());
};
bool dependencyCheck(Coord_3d A, Coord_3d B){
	bool flag = false;
	long double ratX = 0;
	long double ratY = 0;
	long double ratZ = 0;

	if(A.getX() != 0 && B.getX() !=0)			ratX = A.getX() / B.getX();
	else if(A.getX() == 0 && B.getX() == 0)		ratX == 0;
	else										return false;

	if(A.getY() != 0 && B.getY() !=0)			ratY = A.getY() / B.getY();
	else if(A.getY() == 0 && B.getY() == 0)		ratY == 0;
	else										return false;

	if(A.getZ() != 0 && B.getZ() !=0)			ratZ = A.getZ() / B.getZ();
	else if(A.getZ() == 0 && B.getZ() == 0)		ratZ == 0;
	else										return false;

	if( ratX == 0 && ratY == 0 && ratZ ==0 )
		flag = false;
	else if( (ratX == 0 && ratY == 0 && ratZ != 0) 
		|| (ratX == 0 && ratY != 0 && ratZ == 0) 
		|| (ratX != 0 && ratY == 0 && ratZ == 0) )
		flag = true;
	else if( (ratX == ratY && ratZ == 0)
		|| (ratY == ratZ && ratX == 0)
		|| (ratX == ratZ && ratY == 0) )
		flag = true;
	else if( ratX == ratY && ratY == ratZ )
		flag = true;
	return flag;
};
