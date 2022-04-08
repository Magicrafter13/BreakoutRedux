#include "Paddle.hpp"

double Paddle::CenterX() {
	return x + width / 2;
}

double *Paddle::Coords() {
	return new double[4]{ x, y, width, height };
}

void Paddle::Draw() {
	//C2D_DrawRectangle(x, y, 0, width, height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF));
	//C2D_DrawRectangle(x + 1, y + 1, 0, width - 2, height - 2, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
	//return;
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
	}
}

bool Paddle::Inside(double tX, double tY) {
	return tX >= x && tX <= x + width && tY >= y && tY <= y + height;
}

void Paddle::Move() {
	x += speed;
}

double *Paddle::Error(double tX, double tY) {
	return new double[2]{ tX < x + width / 2 ? tX - x : tX - x - width, tY < y + height / 2 ? tY - y : tY - y - height };
}

void Paddle::Reset(double sX, double sY, double sWidth, double sHeight) {
	x = sX;
	y = sY;
	width = sWidth;
	height = sHeight;
	size = 1;
	speed = 0;
}

Paddle::Paddle(double sX, double sY, double sWidth, double sHeight) {
	Reset(sX, sY, sWidth, sHeight);
}
