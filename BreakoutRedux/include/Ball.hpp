#include "BreakoutRedux.hpp"

class Ball {
	double x, y;
	double radius;
	int degree; //convert like this: degree * (PI / 1800)
	//std::vector<Brick*> collidingBricks;
public:
	double speed;

	void Draw() {
		DrawTexture(GetImage(spriteSheet, sprites_ball00_idx), x - radius, y - radius);
		return;
		C2D_DrawEllipse(x - radius, y - radius, 0.0, radius * 2.0, radius * 2.0, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		C2D_DrawEllipse(x - radius + 1.0, y - radius + 1.0, 0.0, radius * 2.0 - 2.0, radius * 2.0 - 2.0, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
	}
	bool Exists() {
		return y < 240.0 + radius;
	}
	int GetDegree() {
		return degree;
	}
	void Update(std::vector<Brick*> sBricks, Paddle sPaddle) {
		if (speed == 0.0) {
			x = sPaddle.CenterX();
			return;
		}
		bool collision = false;
		double error[2] = { 0, 0 };
		double edgeX = cos(degree * PI / 1800);
		double edgeY = sin(degree * PI / 1800);
		std::vector<Brick*> toCheck = std::vector<Brick*>();
		for (Brick* brick : sBricks) {
			if (brick->Exists()) { // I think I can remove this check, since the Game object should have done this already
				double* brickCoords = brick->Coords();
				if (brickCoords[0] + brickCoords[2] >= x - radius - speed * 0.6 && brickCoords[1] + brickCoords[3] >= y - radius - speed * 0.6 && brickCoords[0] <= x + radius + speed * 0.6 && brickCoords[1] <= y + radius + speed * 0.6)
					toCheck.push_back(brick);
			}
		}
		bool checkPaddle = y > 201 && y < sPaddle.Coords()[1] + sPaddle.Coords()[3] / 2;
		for (double distance = 0.0; distance < speed * 0.6 - 0.1 && !collision; distance += 0.2) {
			double testX = x + edgeX * distance;
			double testY = y + edgeY * distance;
			double specialX[] = { testX + radius, testX, testX - radius, testX };
			double specialY[] = { testY, testY + radius, testY, testY - radius };
			int change = 0;
			int step = 0;
			for (int angle = degree; angle < degree + 901 && !collision; angle += (change % 2 == 0 ? change * -1 : change) * 10) {
				switch (step) {
				case 0:
					angle = 0;
					break;
				case 1:
					angle = 900;
					break;
				case 2:
					angle = 1800;
					break;
				case 3:
					angle = 2700;
					break;
				case 4:
					angle = 450;
					break;
				case 5:
					angle = 1350;
					break;
				case 6:
					angle = 2250;
					break;
				case 7:
					angle = 3150;
					break;
				default:
					change++;
					break;
				}
				double testEdgeX = step < 4 ? specialX[step] : testX + cos(angle * PI / 1800) * radius;
				double testEdgeY = step < 4 ? specialY[step] : testY + sin(angle * PI / 1800) * radius;
				if (testEdgeX <= 0 || testEdgeX >= 400 || testEdgeY <= 0) {
					error[0] = (angle > 1350 && angle < 2250) || angle < 450 || angle > 3150 ? testEdgeX <= 0 ? testEdgeX : testEdgeX - 400.0 : testEdgeX - x;
					error[1] = angle > 2250 && angle < 3150 ? testEdgeY : y - testEdgeY;
					SetDegree(2 * angle + 1800 - degree);
					collision = true;
				}
				else if (checkPaddle && sPaddle.Inside(testEdgeX, testEdgeY)) {
					if (sPaddle.speed != 0.0) {
						//if (step > 3 && change == 0)
							//error = paddle.Error(tX, tY);
						//angle -= (angle + 1800 - degree) / 2;
						double* padCoords = sPaddle.Coords();
						std::vector<double> lengths = { (padCoords[0] - testX) * (padCoords[0] - testX) + (padCoords[1] - testY) * (padCoords[1] - testY),
							(padCoords[0] + padCoords[2] - testX) * (padCoords[0] + padCoords[2] - testX) + (padCoords[1] - testY) * (padCoords[1] - testY),
							radius * radius };
						if (lengths[0] <= lengths[2]) {
							error[0] = cos(angle * PI / 1800) * (sqrt(lengths[2]) - sqrt(lengths[0]));
							error[1] = sin(angle * PI / 1800) * (sqrt(lengths[2]) - sqrt(lengths[0]));
						}
						else if (lengths[1] <= lengths[2]) {
							error[0] = cos(angle * PI / 1800) * (sqrt(lengths[2]) - sqrt(lengths[1]));
							error[1] = sin(angle * PI / 1800) * (sqrt(lengths[2]) - sqrt(lengths[1]));
						}
						//else
							//error = paddle.Error(tX, tY);
						/*double vec[2] = { cos((double)degree * PI / 1800.0) * speed + paddle.speed, sin((double)degree * PI / 1800.0) * speed };
						speed = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
						if (speed < 2)
							speed = 2.0;
						degree = atan(vec[1] / vec[0]);*/
					}
					//else
						SetDegree(2 * angle + 1800 - degree);
					/*if (paddle.speed != 0.0) {
						if (angle != 900) {
							std::vector<double> vec = { cos((double)degree * (PI / 1800.0)) * (speed / 60.0) + paddle.speed, sin((double)degree * (PI / 1800.0)) * (speed / 60.0) };
							//speed = sqrt(vec[0] * vec[0] + vec[1] * vec[1]) * 60.0;
							//if (speed < 2.0)
								//speed = 2.0;
							degree = atan(vec[1] / vec[0]);
							//angle = degree + 1800;
							std::vector<double> padCoords = paddle.Coords();
							std::vector<double> lengths = { (padCoords[0] - drawX) * (padCoords[0] - drawX) + (padCoords[1] - drawY) * (padCoords[1] - drawY), (padCoords[0] + padCoords[2] - drawX) * (padCoords[0] + padCoords[2] - drawX) + (padCoords[1] - drawY) * (padCoords[1] - drawY), radius * radius };
						}
					}*/
					collision = true;
				}
				else if (!checkPaddle) {
					for (Brick* brick : toCheck) {
						// check if we've already collided
						if (brick->Inside(testEdgeX, testEdgeY)) {
							brick->Hit();
							//error = brick->Error(tX, tY);
							//add to collidingBricks
							SetDegree(2 * angle + 1800 - degree);
							collision = true;
							break;
						}
					}
				}
				if (collision) {
					//SetDegree(2 * angle + 1800 - degree);
					x -= error[0];
					y -= error[1];
				}
				if (step < 4)
					angle = degree;
				step++;
			}
		}
		// check if any of the contents of collidingBricks are no longer nearby and if not, remove them
	}
	void Move() {
		x += cos((double)degree * PI / 1800) * speed;
		y += sin((double)degree * PI / 1800) * speed;
	}
	void Reset(double sX, double sY, double sRadius) {
		x = sX;
		y = sY;
		radius = sRadius;
		do
			degree = (rand() % 1400) + 2000;
		while (degree > 2650 && degree < 2750);
		speed = 0;
	}
	void SetDegree(int deg) {
		degree = deg;
		if (degree >= 3600) degree -= 3600;
		if (degree < 0) degree += 3600;
	}

	Ball(double sX, double sY, double sRadius) {
		Reset(sX, sY, sRadius);
	}
};
