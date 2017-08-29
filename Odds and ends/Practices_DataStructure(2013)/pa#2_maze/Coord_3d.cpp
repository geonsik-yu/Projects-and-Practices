#include <cmath>
class Coord_3d{
public:
	Coord_3d(): x_axis(0), y_axis(0), z_axis(0) {};
	Coord_3d(int new_x, int new_y, int new_z){
		x_axis = new_x;
		y_axis = new_y;
		z_axis = new_z;
	}; 
	~Coord_3d() { };

	int getX(){ return x_axis; };
	int getY(){ return y_axis; };
	int getZ(){ return z_axis; };
	void setX(int newX){ x_axis = newX; };
	void setY(int newY){ y_axis = newY; };
	void setZ(int newZ){ z_axis = newZ; };

private:
	int x_axis;
	int y_axis;
	int z_axis;
};

