#include "BreakoutRedux.hpp"

class Ball {
	double x, y;
	double radius;
	int degree; //convert like this: degree * (PI / 1800)
	std::vector<Brick*> bricks;
	Paddle paddle = Paddle(0.0f, 0.0f, 0.0f, 0.0f);
	double slope;
	std::vector<double> range;
	std::vector<Brick*> collidingBricks;
public:
	double speed;
	double X() {
		return x;
	}
	double Y() {
		return y;
	}
	void Move() {
		x += cos((double)degree * (PI / 1800.0)) * speed;
		y += sin((double)degree * (PI / 1800.0)) * speed;
		range = Range();
	}
	void Up(int amount) { //FOR TESTING ONLY
		y -= amount; //DELETE FOR RELEASE
		range = Range();
	} //DO NOT KEEP
	void Update(std::vector<Brick*> sBricks, Paddle sPaddle) {
		if (speed == 0.0) {
			x = sPaddle.CenterX();
			return;
		}
		bricks = sBricks;
		paddle = sPaddle;
		bool collision = false;
		std::vector<double> error = { 0.0, 0.0 };
		double dX = cos((double)degree * PI / 1800.0);
		double dY = sin((double)degree * PI / 1800.0);
		std::vector<Brick*> toCheck = std::vector<Brick*>();
		for (Brick* brick : bricks) {
			if (brick->Exists()) {
				std::vector<double> tC = brick->Coords();
				if (!(tC[0] + tC[2] < x - radius - speed * 0.6 || tC[1] + tC[3] < y - radius - speed * 0.6 || tC[0] > x + radius + speed * 0.6 || tC[1] > y + radius + speed * 0.6))
					toCheck.push_back(brick);
			}
		}
		bool checkPaddle = y > 201 && y < paddle.Coords()[1] + paddle.Coords()[3] / 2;
		for (double distance = 0.0; distance < speed * 0.8 - 0.1 && !collision; distance += 0.2) {
			double drawX = x + dX * distance;
			double drawY = y + dY * distance;
			double specialX[] = { drawX + radius, drawX, drawX - radius, drawX };
			double specialY[] = { drawY, drawY + radius, drawY, drawY - radius };
			// instead of going from -90 to 90 (relative) start with 0 then add 1, subtract 2, add 3, subtract 4, etc
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
				double tX = step < 4 ? specialX[step] : drawX + cos((double)angle * PI / 1800.0) * radius;
				double tY = step < 4 ? specialY[step] : drawY + sin((double)angle * PI / 1800.0) * radius;
				if (tX <= 0 || tX >= 400 || tY <= 0) {
					error = { (angle > 1350 && angle < 2250) || angle < 450 || angle > 3150 ? tX <= 0 ? tX : tX - 400.0 : tX - x, angle > 2250 && angle < 3150 ? tY : y - tY };
					SetDegree(2 * angle + 1800 - degree);
					collision = true;
				}
				else if (checkPaddle && paddle.Inside(tX, tY)) {
					//if ((angle == 0 || angle == 3600 || angle == 1800) && paddle.speed != 0.0)
					//if (paddle.Coords()[1] < drawY || step < 8)
					if (paddle.speed != 0.0) {
						//if (step > 3 && change == 0)
							//error = paddle.Error(tX, tY);
						//angle -= (angle + 1800 - degree) / 2;
						std::vector<double> padCoords = paddle.Coords();
						std::vector<double> lengths = { (padCoords[0] - drawX) * (padCoords[0] - drawX) + (padCoords[1] - drawY) * (padCoords[1] - drawY),
							(padCoords[0] + padCoords[2] - drawX) * (padCoords[0] + padCoords[2] - drawX) + (padCoords[1] - drawY) * (padCoords[1] - drawY),
							radius * radius };
						if (lengths[0] <= lengths[2])
							error = { cos((double)angle * PI / 1800.0) * (sqrt(lengths[2]) - sqrt(lengths[0])), sin((double)angle * PI / 1800.0) * (sqrt(lengths[2]) - sqrt(lengths[0])) };
						else if (lengths[1] <= lengths[2])
							error = { cos((double)angle * PI / 1800.0) * (sqrt(lengths[2]) - sqrt(lengths[1])), sin((double)angle * PI / 1800.0) * (sqrt(lengths[2]) - sqrt(lengths[1])) };
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
						if (brick->Inside(tX, tY)) {
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
		return;
		/*int tQ = TravelQuadrant();
		if (slope < 1.0f && slope > -1.0f) {
			bool ghostSet = false;
			bool invert = (tQ == 2 || tQ == 3);
			std::vector<float> error = { 0.0f, 0.0f };
			for (float drawX = range[invert ? 1 : 0]; (invert ? (drawX >= range[0]) : (drawX <= range[1])) && !ghostSet; drawX += invert ? -1.0f : 1.0f) {
				float drawY = slope * (drawX - x) + y;
				for (int angle = degree - 900; angle <= degree + 900 && !ghostSet; angle += 10) {
					float tX = drawX + cos(angle * ((float)PI / 1800.0f)) * radius;
					float tY = drawY + sin(angle * ((float)PI / 1800.0f)) * radius;
					if (tX <= 0.0f || tX >= 400.0f || tY <= 0.0f) {
						error = { tX <= 0.0f ? tX : tX - 400.0f, tY };
						ghostSet = true;
					}
					else if (tY > 194 && paddle.Inside(tX, tY)) {
						error = paddle.Error(tX, tY);
						ghostSet = true;
					}
					else if (y < 200) {
						for (Brick brick : *sBricks) {
							std::vector<float> tC = brick.Coords();
							if (tC[0] + tC[2] < drawX - radius - 1.0f || tC[1] + tC[3] < drawY - radius - 1.0f || tC[0] > drawX + radius + 1.0f || tC[1] > drawY + radius + 1.0f)
								continue;
							for (int i = 0; i < colidingBricks->size(); i++)
								if (brick == (*colidingBricks)[i])
									continue;
							if (brick.Inside(tX, tY)) {
								error = brick.Error(tX, tY);
								colidingBricks->push_back(brick);
								ghostSet = true;
								break;
							}
						}
					}
					if (ghostSet) {
						ghostCoords = { drawX - radius, drawY - radius };
						int difference = angle - degree;
						degree = angle + 1800 + difference;
						x -= error[0];
						Move();
					}
				}
			}
		}
		else {
			bool ghostSet = false;
			bool invert = (tQ == 3 || tQ == 4);
			std::vector<float> error = { 0.0f, 0.0f };
			for (float drawY = range[invert ? 3 : 2]; (invert ? (drawY >= range[2]) : (drawY <= range[3])) && !ghostSet; drawY += invert ? -1.0f : 1.0f) {
				float drawX = (drawY - y) / slope + x;
				for (int angle = degree - 900; angle <= degree + 900 && !ghostSet; angle += 10) {
					float tX = drawX + cos(angle * ((float)PI / 1800.0f)) * radius;
					float tY = drawY + sin(angle * ((float)PI / 1800.0f)) * radius;
					if (tX <= 0.0f || tX >= 400.0f || tY <= 0.0f) {
						error = { tX <= 0.0f ? tX : tX - 400.0f, tY };
						ghostSet = true;
					}
					else if (tY > 194 && paddle.Inside(tX, tY)) {
						error = paddle.Error(tX, tY);
						ghostSet = true;
					}
					else if (y < 200) {
						for (Brick brick : *sBricks) {
							std::vector<float> tC = brick.Coords();
							if (tC[0] + tC[2] < drawX - radius - 1.0f || tC[1] + tC[3] < drawY - radius - 1.0f || tC[0] > drawX + radius + 1.0f || tC[1] > drawY + radius + 1.0f)
								continue;
							for (int i = 0; i < colidingBricks->size(); i++)
								if (brick == (*colidingBricks)[i])
									continue;
							if (brick.Inside(tX, tY)) {
								error = brick.Error(tX, tY);
								colidingBricks->push_back(brick);
								ghostSet = true;
								break;
							}
						}
					}
					if (ghostSet) {
						ghostCoords = { drawX - radius, drawY - radius };
						int difference = angle - degree;
						degree = angle + 1800 + difference;
						y -= error[1];
						Move();
					}
				}
			}
		}
		for (int i = 0; i < colidingBricks->size(); i++) {
			std::vector<float> tC = (*colidingBricks)[i].Coords();
			if (tC[0] + tC[2] < x - radius - 1.0f || tC[1] + tC[3] < y - radius - 1.0f || tC[0] > x + radius + 1.0f || tC[1] > y + radius + 1.0f)
				colidingBricks->erase(colidingBricks->begin() + i);
		}*/
	}
	int GetDegree() {
		return degree;
	}
	void SetDegree(int deg) {
		degree = deg;
		if (degree >= 3600) degree -= 3600;
		if (degree < 0) degree += 3600;
		slope = Slope();
		range = Range();
	}
	int TravelQuadrant() {
		return degree / 900;
		if (degree <    0) return 0;
		if (degree <  900) return 1;
		if (degree < 1800) return 2;
		if (degree < 2700) return 3;
		if (degree < 3600) return 4;
		return 5;
	}
	std::vector<double> Range() {
		switch (TravelQuadrant()) {
		case 1:
			return std::vector<double> { x + 1.0, x + 2.0, y + 1.0, y + 2.0 };
		case 2:
			return std::vector<double> { x - 2.0, x - 1.0, y + 1.0, y + 2.0 };
		case 3:
			return std::vector<double> { x - 2.0, x - 1.0, y - 2.0, y - 1.0 };
		case 4:
			return std::vector<double> { x + 1.0, x + 2.0, y - 2.0, y - 1.0 };
		default:
			return std::vector<double> { 0.0, 0.0, 0.0, 0.0 };
		}
	}
	double Slope() {
		return tan((double)degree * PI / 1800.0);
	}
	bool Exists() {
		return y < 240.0 + radius;
	}
	void Draw() {
		//DrawTexture(GetImage(spriteSheet, sprites_ball00_idx), x - radius, y - radius);
		C2D_DrawEllipse(x - radius, y - radius, 0.0, radius * 2.0, radius * 2.0, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		C2D_DrawEllipse(x - radius + 1.0, y - radius + 1.0, 0.0, radius * 2.0 - 2.0, radius * 2.0 - 2.0, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
	}
	void Reset(double sX, double sY, double sRadius, std::vector<Brick*> sBricks, Paddle sPaddle) {
		x = sX;
		y = sY;
		radius = sRadius;
		degree = 3150;
		speed = 0.0;
		range = Range();
		slope = Slope();
		Update(sBricks, sPaddle);
	}
	Ball(double sX, double sY, double sRadius, std::vector<Brick*> sBricks, Paddle sPaddle) {
		Reset(sX, sY, sRadius, sBricks, sPaddle);
	}
};
