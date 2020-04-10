#include "BreakoutRedux.hpp"

class Paddle {
	float x, y;
	float width, height;
	int size;
public:
	float CenterX() {
		return x + width / 2.0f;
	}
	bool Inside(float tX, float tY) {
		return !(tX < x || tX > x + width || tY < y || tY > y + height);
	}
	void Move(int amount) {
		x += amount;
	}
	std::vector<float> Error(float tX, float tY) {
		return std::vector<float> { tX < x + width / 2.0f ? tX - x : tX - x - width, tY - y };
	}
	void Draw() {
		C2D_DrawRectangle(x, y, 0.0f, width, height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF));
		C2D_DrawRectangle(x + 1.0f, y + 1.0f, 0.0f, width - 2.0f, height - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		return;
		switch (size) {
		case 0:
			DrawTexture(GetImage(spriteSheet, sprites_paddle_small_idx), x, y);
			break;
		case 1:
			DrawTexture(GetImage(spriteSheet, sprites_paddle_idx), x, y);
			break;
		case 2:
			DrawTexture(GetImage(spriteSheet, sprites_paddle_big_idx), x, y);
			break;
		default:
			//handle error
			break;
		}
	}
	void Reset(float sX, float sY, float sWidth, float sHeight) {
		x = sX;
		y = sY;
		width = sWidth;
		height = sHeight;
		size = 1;
	}
	Paddle(float sX, float sY, float sWidth, float sHeight) {
		Reset(sX, sY, sWidth, sHeight);
	}
};
