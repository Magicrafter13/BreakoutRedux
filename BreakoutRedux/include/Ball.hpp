#include "BreakoutRedux.hpp"

class Ball {
	float x, y;
	float radius;
	int degree = 3150; //convert like this: degree * (PI / 1800.0)
public:
	int GetDegree() {
		return degree;
	}
	void AddDegree(int inc) {
		degree += inc;
		if (degree > 3600) degree -= 3600;
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
		//x = (y - y1)/m + x1
		//y = m(x - x1) + y1
		if (slope < 1.0f && slope > -1.0f) {
			for (int x = (int)round(range[0]); x < (int)round(range[1]); x++) {
				float y = slope * ((float)x - coords[0]) + coords[1];
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), (float)x, y);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), (float)x + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, y + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), (float)x + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, y + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius);
			}
		} else {
			for (int y = (int)round(range[2]); y < (int)round(range[3]); y++) {
				float x = ((float)y - coords[1]) / slope + coords[0];
				DrawTexture(GetImage(spriteSheet, sprites_cyan_idx), x, (float)y);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), x + (float)cos((double)(degree - 900) * (PI / 1800.0)) * radius, (float)y + (float)sin((double)(degree - 900) * (PI / 1800.0)) * radius);
				DrawTexture(GetImage(spriteSheet, sprites_green_idx), x + (float)cos((double)(degree + 900) * (PI / 1800.0)) * radius, (float)y + (float)sin((double)(degree + 900) * (PI / 1800.0)) * radius);
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
