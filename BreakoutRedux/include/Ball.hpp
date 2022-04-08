#include "BreakoutRedux.hpp"
#include "Brick.hpp"
#include "Paddle.hpp"

class Ball {
	double x, y;
	double radius;
	int degree; // convert like this: degree * (PI / 1800)
	//std::vector<Brick*> collidingBricks;

public:
	double speed;

	void Draw();
	bool Exists();
	int GetDegree();
	void Update(std::vector<Brick*> sBricks, Paddle sPaddle);
	void Move();
	void Reset(double sX, double sY, double sRadius);
	void SetDegree(int deg);

	Ball(double sX, double sY, double sRadius);
};
