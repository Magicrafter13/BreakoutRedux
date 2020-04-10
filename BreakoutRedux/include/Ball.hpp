#include "BreakoutRedux.hpp"

class Ball {
	float x, y;
	float radius;
	int degree; //convert like this: degree * (PI / 1800)
	std::vector<Brick*> bricks;
	Paddle paddle = Paddle(0.0f, 0.0f, 0.0f, 0.0f);
	float slope;
	std::vector<float> range;
	std::vector<Brick*> collidingBricks;
public:
	float speed;
	void Move() {
		x += (float)cos((double)degree * (PI / 1800.0)) * (speed / 60.0f);
		y += (float)sin((double)degree * (PI / 1800.0)) * (speed / 60.0f);
		range = Range();
	}
	void Up(int amount) { //FOR TESTING ONLY
		y -= amount; //DELETE FOR RELEASE
		range = Range();
	} //DO NOT KEEP
	void Update(std::vector<Brick*> sBricks, Paddle sPaddle) {
		if (speed == 0.0f) {
			x = sPaddle.CenterX();
			return;
		}
		bricks = sBricks;
		paddle = sPaddle;
		bool collision = false;
		std::vector<float> error = { 0.0f, 0.0f };
		float dX = cos((float)degree * (float)PI / 1800.0f);
		float dY = sin((float)degree * (float)PI / 1800.0f);
		std::vector<Brick*> toCheck = std::vector<Brick*>();
		for (Brick* brick : bricks) {
			if (brick->Exists()) {
				std::vector<float> tC = brick->Coords();
				if (!(tC[0] + tC[2] < x - radius - 1.0f || tC[1] + tC[3] < y - radius - 1.0f || tC[0] > x + radius + 1.0f || tC[1] > y + radius + 1.0f))
					toCheck.push_back(brick);
			}
		}
		bool checkPaddle = y > 194;
		for (float distance = 0.1f; distance < 1.2f && !collision; distance += 0.2f) {
			float drawX = x + dX * distance;
			float drawY = y + dY * distance;
			for (int angle = degree - 900; angle <= degree + 900 && !collision; angle += 10) {
				float tX = drawX + cos((float)angle * (float)PI / 1800.0f) * radius;
				float tY = drawY + sin((float)angle * (float)PI / 1800.0f) * radius;
				if (tX <= 0 || tX >= 400 || tY <= 0) {
					error = { (angle > 1350 && angle < 2250) || angle < 450 || angle > 3150 ? tX <= 0.0f ? tX : tX - 400.0f : tX - x, angle > 2250 && angle < 3150 ? tY : y - tY };
					collision = true;
				}
				else if (checkPaddle && paddle.Inside(tX, tY)) {
					//error = paddle.Error(tX, tY);
					collision = true;
				}
				else if (!checkPaddle) {
					for (Brick* brick : toCheck) {
						if (brick->Exists()) {
							std::vector<float> tC = brick->Coords();
							if (tC[0] + tC[2] < drawX - radius - 1.0f || tC[1] + tC[3] < drawY - radius - 1.0f || tC[0] > drawX + radius + 1.0f || tC[1] > drawY + radius + 1.0f)
								continue;
							// check if we've already collided
							if (brick->Inside(tX, tY)) {
								brick->Hit();
								//error = brick->Error(tX, tY);
								//add to collidingBricks
								collision = true;
								break;
							}
						}
					}
				}
				if (collision) {
					int difference = angle - degree;
					//degree = angle + 1800 + difference;
					AddDegree(angle + 1800 + difference - degree);
					x -= error[0];
					y -= error[1];
				}
			}
		}
		// check if any of the contents of collidingBricks are no longer nearby and if not, remove them
		//std::cout << degree << "\n";
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
	void AddDegree(int inc) {
		degree += inc;
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
	std::vector<float> Range() {
		switch (TravelQuadrant()) {
		case 1:
			return std::vector<float> { x + 1.0f, x + 2.0f, y + 1.0f, y + 2.0f };
		case 2:
			return std::vector<float> { x - 2.0f, x - 1.0f, y + 1.0f, y + 2.0f };
		case 3:
			return std::vector<float> { x - 2.0f, x - 1.0f, y - 2.0f, y - 1.0f };
		case 4:
			return std::vector<float> { x + 1.0f, x + 2.0f, y - 2.0f, y - 1.0f };
		default:
			return std::vector<float> { 0.0f, 0.0f, 0.0f, 0.0f };
		}
	}
	float Slope() {
		return (float)tan((double)degree * (PI / 1800.0));
	}
	bool Exists() {
		return y < 240 + radius;
	}
	void Draw() {
		C2D_DrawEllipse(x - radius, y - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		C2D_DrawEllipse(x - radius + 1.0f, y - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		//C2D_DrawEllipse(ghostCoords[0], ghostCoords[1], 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		//C2D_DrawEllipse(ghostCoords[0] + 1.0f, ghostCoords[1] + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		//x = (y - y1)/m + x1
		//y = m(x - x1) + y1
		/*if (slope < 1.0f && slope > -1.0f) {
			for (int drawX = (int)round(range[0]); drawX < (int)round(range[1]); drawX++) {
				float drawY = slope * ((float)drawX - x) + y;
				if (drawX < 0 || drawX >= 400 || drawY < 0 || drawY >= 240) continue;
				float drawLeft[] = { (float)drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius };
				float drawRight[] = { (float)drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius };
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), (float)drawX, drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawLeft[0], drawLeft[1]);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawRight[0], drawRight[1]);
			}
		} else {
			for (int drawY = (int)round(range[2]); drawY < (int)round(range[3]); drawY++) {
				float drawX = ((float)drawY - y) / slope + x;
				if (drawX < 0 || drawX >= 400 || drawY < 0 || drawY >= 240) continue;
				float drawLeft[] = { drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius };
				float drawRight[] = { drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius };
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), drawX, (float)drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawLeft[0], drawLeft[1]);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawRight[0], drawRight[1]);
			}
		}*/
	}
	void Reset(float sX, float sY, float sRadius, std::vector<Brick*> sBricks, Paddle sPaddle) {
		x = sX;
		y = sY;
		radius = sRadius;
		degree = 3150;
		speed = 0.0f;
		range = Range();
		slope = Slope();
		Update(sBricks, sPaddle);
	}
	Ball(float sX, float sY, float sRadius, std::vector<Brick*> sBricks, Paddle sPaddle) {
		Reset(sX, sY, sRadius, sBricks, sPaddle);
	}
};
