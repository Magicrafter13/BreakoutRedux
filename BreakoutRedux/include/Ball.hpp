#include "BreakoutRedux.hpp"

class Ball {
	float x, y;
	float radius;
	int degree = 3150; //convert like this: degree * (PI / 1800.0)
	std::vector<Brick> *bricks;
	Paddle paddle = Paddle(0.0f, 0.0f, 0.0f, 0.0f);
	float slope = 0.0f;
	std::vector<float> range = { 0.0f, 0.0f, 0.0f, 0.0f };
	std::vector<float> coords = { 0.0f, 0.0f };
	std::vector<float> ghostCoords = { 0.0f, 0.0f };
public:
	void Up(int amount) { //FOR TESTING ONLY
		y -= amount; //DELETE FOR RELEASE
	} //DO NOT KEEP
	void Update(std::vector<Brick> *sBricks, Paddle sPaddle) {
		bricks = sBricks;
		paddle = sPaddle;
		slope = Slope();
		range = Range();
		coords = GetCoords();
		int tQ = TravelQuadrant();
		bool ghostSet = false;
		bool invert = tQ == 2 || tQ == 3;
		for (int drawX = (int)round(range[invert ? 1 : 0]); invert ? (drawX >= (int)round(range[0])) : (drawX <= (int)round(range[1])); drawX += invert ? -1 : 1) {
			float drawY = slope * ((float)drawX - coords[0]) + coords[1];
			if (!ghostSet) {
				for (int angle = degree - 900; angle <= degree + 900 && !ghostSet; angle += 10) {
					float tX = (float)drawX + (float)cos((double)angle * (PI / 1800.0)) * radius;
					float tY = drawY + (float)sin((double)angle * (PI / 1800.0)) * radius;
					if (paddle.Inside(tX, tY) || tX <= 0.0f || tX >= 400.0f || tY <= 0.0f || tY >= 240.0f) {
						ghostCoords = { (float)drawX - radius, drawY - radius };
						ghostSet = true;
					}
				}
			}
		}
		if (slope < 1.0f && slope > -1.0f) {
			bool ghostSet = false;
			bool invert = (tQ == 2 || tQ == 3);
			for (int drawX = (int)round(range[invert ? 1 : 0]); invert ? (drawX >= (int)round(range[0])) : (drawX <= (int)round(range[1])); drawX += invert ? -1 : 1) {
				float drawY = slope * ((float)drawX - coords[0]) + coords[1];
				if (!ghostSet) {
					for (int angle = degree - 900; angle <= degree + 900 && !ghostSet; angle += 10) {
						float tX = (float)drawX + (float)cos((double)angle * (PI / 1800.0)) * radius;
						float tY = drawY + (float)sin((double)angle * (PI / 1800.0)) * radius;
						if (paddle.Inside(tX, tY) || tX <= 0.0f || tX >= 400.0f || tY <= 0.0f || tY >= 240.0f) {
							ghostCoords = { (float)drawX - radius, drawY - radius };
							ghostSet = true;
						}
					}
				}
			}
		} else {
			bool ghostSet = false;
			bool invert = (tQ == 3 || tQ == 4);
			for (int drawY = (int)round(range[invert ? 3 : 2]); invert ? (drawY >= (int)round(range[2])) : (drawY <= (int)round(range[3])); drawY += invert ? -1 : 1) {
				float drawX = ((float)drawY - coords[1]) / slope + coords[0];
				if (!ghostSet) {
					for (int angle = degree - 900; angle <= degree + 900 && !ghostSet; angle += 10) {
						float tX = drawX + (float)cos((double)angle * (PI / 1800.0)) * radius;
						float tY = (float)drawY + (float)sin((double)angle * (PI / 1800.0)) * radius;
						if (paddle.Inside(tX, tY) || tX <= 0.0f || tX >= 400.0f || tY <= 0.0f || tY >= 240.0f) {
							ghostCoords = { drawX - radius, (float)drawY - radius };
							ghostSet = true;
						}
					}
				}
			}
		}
	}
	int GetDegree() {
		return degree;
	}
	void AddDegree(int inc) {
		degree += inc;
		if (degree >= 3600) degree -= 3600;
		if (degree < 0) degree += 3600;
	}
	int TravelQuadrant() {
		if (degree <    0) return 0;
		if (degree <  900) return 1;
		if (degree < 1800) return 2;
		if (degree < 2700) return 3;
		if (degree < 3600) return 4;
		return 5;
	}
	std::vector<float> Range() {
		int tQ = TravelQuadrant();
		switch (tQ) {
		case 1:
			return std::vector<float> { x, 400.0f, y, 240.0f };
		case 2:
			return std::vector<float> { 0.0f, x, y, 240.0f };
		case 3:
			return std::vector<float> { 0.0f, x, 0.0f, y };
		case 4:
			return std::vector<float> { x, 400.0f, 0.0f, y };
		}
		//if (tQ == 1 || tQ == 4) return std::vector<float> { x, 400.0f };
		//if (tQ == 2 || tQ == 3) return std::vector<float> { 0.0f, x };
		return std::vector<float> { 0.0f, 1.0f };
	}
	std::vector<float> GetCoords() {
		return std::vector<float> { x, y };
	}
	float Slope() {
		return (float)tan((double)degree * (PI / 1800.0));
	}
	void Draw() {
		C2D_DrawEllipse(x - radius, y - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		C2D_DrawEllipse(x - radius + 1.0f, y - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		C2D_DrawEllipse(ghostCoords[0], ghostCoords[1], 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
		C2D_DrawEllipse(ghostCoords[0] + 1.0f, ghostCoords[1] + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		//x = (y - y1)/m + x1
		//y = m(x - x1) + y1
		/*if (slope < 1.0f && slope > -1.0f) {
			for (int drawX = (int)round(range[0]); drawX < (int)round(range[1]); drawX++) {
				float drawY = slope * ((float)drawX - coords[0]) + coords[1];
				if (drawX < 0 || drawX >= 400 || drawY < 0 || drawY >= 240) continue;
				float drawLeft[] = { (float)drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius };
				float drawRight[] = { (float)drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius };
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), (float)drawX, drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawLeft[0], drawLeft[1]);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawRight[0], drawRight[1]);
			}
		} else {
			for (int drawY = (int)round(range[2]); drawY < (int)round(range[3]); drawY++) {
				float drawX = ((float)drawY - coords[1]) / slope + coords[0];
				if (drawX < 0 || drawX >= 400 || drawY < 0 || drawY >= 240) continue;
				float drawLeft[] = { drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius };
				float drawRight[] = { drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius };
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), drawX, (float)drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawLeft[0], drawLeft[1]);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawRight[0], drawRight[1]);
			}
		}*/
	}
	void Reset(float sX, float sY, float sRadius) {
		x = sX;
		y = sY;
		radius = sRadius;
		degree = 3150;
	}
	Ball(float sX, float sY, float sRadius) {
		Reset(sX, sY, sRadius);
	}
};
