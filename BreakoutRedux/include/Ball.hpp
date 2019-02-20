#include "BreakoutRedux.hpp"

class Ball {
	float x, y;
	float radius;
	int degree = 3150; //convert like this: degree * (PI / 1800.0)
	std::vector<Brick> *bricks;
	Paddle paddle = Paddle(0.0f, 0.0f, 0.0f, 0.0f);
public:
	void Update(std::vector<Brick> *sBricks, Paddle sPaddle) {
		bricks = sBricks;
		paddle = sPaddle;
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
			return std::vector<float> { x - radius, 400.0f, y - radius, 240.0f };
		case 2:
			return std::vector<float> { 0.0f, x + radius, y - radius, 240.0f };
		case 3:
			return std::vector<float> { 0.0f, x + radius, 0.0f, y + radius };
		case 4:
			return std::vector<float> { x - radius, 400.0f, 0.0f, y + radius };
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
		std::vector<float> range = Range();
		std::vector<float> coords = GetCoords();
		float slope = Slope();
		/*if (slope * (0.0f - coords[0]) + coords[1] < 0 || slope * (400.0f - coords[0]) + coords[1] < 0 && range[3] < 240.0f) {
			C2D_DrawEllipse(((float)y - coords[1]) / slope + coords[0] - radius, 0.0f, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
			C2D_DrawEllipse(((float)y - coords[1]) / slope + coords[0] - radius + 1.0f, 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		}*/
		//x = (y - y1)/m + x1
		//y = m(x - x1) + y1
		if (slope < 1.0f && slope > -1.0f) {
			for (int drawX = (int)round(range[0]); drawX < (int)round(range[1]); drawX++) {
				float drawY = slope * ((float)drawX - coords[0]) + coords[1];
				if (TravelQuadrant() == 1 || TravelQuadrant() == 4) {
					if (drawX == (int)round(400.0f - radius)) {
						C2D_DrawEllipse((float)drawX - radius, drawY - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
						C2D_DrawEllipse((float)drawX - radius + 1.0f, drawY - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
					}
				} else {
					if (drawX == (int)round(0.0f + radius)) {
						C2D_DrawEllipse((float)drawX - radius, drawY - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
						C2D_DrawEllipse((float)drawX - radius + 1.0f, drawY - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
					}
				}
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), (float)drawX, drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), (float)drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), (float)drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius);
			}
		} else {
			for (int drawY = (int)round(range[2]); drawY < (int)round(range[3]); drawY++) {
				float drawX = ((float)drawY - coords[1]) / slope + coords[0];
				if (TravelQuadrant() == 1 || TravelQuadrant() == 2) {
					if (drawY == (int)round(240.0f - radius)) {
						//C2D_DrawEllipse(drawX - radius, (float)drawY - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
						//C2D_DrawEllipse(drawX - radius + 1.0f, (float)drawY - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
					}
				} else {
					if (drawY == (int)round(0.0f + radius)) {
						C2D_DrawEllipse(drawX - radius, (float)drawY - radius, 0.0f, radius * 2.0f, radius * 2.0f, C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF), C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
						C2D_DrawEllipse(drawX - radius + 1.0f, (float)drawY - radius + 1.0f, 0.0f, radius * 2.0f - 2.0f, radius * 2.0f - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
					}
				}
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), drawX, (float)drawY);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawX + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), drawX + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, (float)drawY + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius);
			}
		}
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
