#ifndef PADDLE_H
#define PADDLE_H

#include "BreakoutRedux.hpp"

class Paddle {
	double x, y;
	double width, height;
	int size;

public:
	double speed;

	double CenterX();
	double *Coords();
	void Draw();
	bool Inside(double tX, double tY);
	void Move();
	double *Error(double tX, double tY);
	void Reset(double sX, double sY, double sWidth, double sHeight);

	Paddle(double sX, double sY, double sWidth, double sHeight);
};

#endif
