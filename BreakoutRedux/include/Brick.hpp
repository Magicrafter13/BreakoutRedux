#ifndef BRICK_H
#define BRICK_H

#include "BreakoutRedux.hpp"

class Brick {
	double x, y;
	double width, height;
	int type;
	int health;

public:
	double *Coords();
	void Draw();
	double *Error(double tX, double tY);
	bool Exists();
	int GetType();
	void Hit();
	bool Inside(double tX, double tY);

	Brick(double sX, double sY, double sWidth, double sHeight, int sType);

	bool operator==(const Brick& other);
};

#endif
