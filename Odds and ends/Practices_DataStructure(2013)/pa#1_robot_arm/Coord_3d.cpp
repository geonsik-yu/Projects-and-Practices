#include <cmath>
class Coord_3d{
public:
	Coord_3d(): x_axis(0), y_axis(0), z_axis(0) {};
	Coord_3d(long double new_x, long double new_y, long double new_z){
		if(abs(new_x) > 10^-10) x_axis = new_x; else x_axis = 0;
		if(abs(new_y) > 10^-10) y_axis = new_y; else x_axis = 0;
		if(abs(new_z) > 10^-10) z_axis = new_z; else x_axis = 0;
	}; 
	~Coord_3d() { };

	long double getX(){ return x_axis; };
	long double getY(){ return y_axis; };
	long double getZ(){ return z_axis; };
	void setX(long double newX){ if(abs(newX) > 10^-10) x_axis = newX; else x_axis = 0; };
	void setY(long double newY){ if(abs(newY) > 10^-10) y_axis = newY; else y_axis = 0; };
	void setZ(long double newZ){ if(abs(newZ) > 10^-10) z_axis = newZ; else z_axis = 0; };
	void roundC(){
		x_axis = floor((x_axis) * 1000 +0.5) / 1000.0;
		y_axis = floor((y_axis) * 1000 +0.5) / 1000.0;
		z_axis = floor((z_axis) * 1000 +0.5) / 1000.0;		
	};

private:
	long double x_axis;
	long double y_axis;
	long double z_axis;
};

