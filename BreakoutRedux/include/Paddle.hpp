#include "BreakoutRedux.hpp"

class Paddle {
	double x, y;
	double width, height;
	int size;
public:
	double speed;
	std::vector<double> Coords() {
		return { x, y, width, height };
	}
	double CenterX() {
		return x + width / 2.0;
	}
	bool Inside(double tX, double tY) {
		return !(tX < x || tX > x + width || tY < y || tY > y + height);
	}
	void Move() {
		x += speed;
	}
	std::vector<double> Error(double tX, double tY) {
		return std::vector<double> { tX < x + width / 2 ? tX - x : tX - x - width, tY < y + height / 2 ? tY - y : tY - y - height };
	}
	void Draw() {
		C2D_DrawRectangle(x, y, 0.0, width, height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF));
		C2D_DrawRectangle(x + 1.0, y + 1.0, 0.0, width - 2.0, height - 2.0, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
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
	void Reset(double sX, double sY, double sWidth, double sHeight) {
		x = sX;
		y = sY;
		width = sWidth;
		height = sHeight;
		size = 1;
		speed = 0.0;
	}
	Paddle(double sX, double sY, double sWidth, double sHeight) {
		Reset(sX, sY, sWidth, sHeight);
	}
};
