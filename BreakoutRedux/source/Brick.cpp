#include "Brick.hpp"

double *Brick::Coords() {
	return new double[4]{ x, y, width, height };
}

void Brick::Draw() {
	//C2D_DrawRectangle(x, y, 0, width, height, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
	//C2D_DrawRectangle(x + 1, y + 1, 0, width - 2, height - 2, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
	//return;
	// above is temp
	switch (type) {
	case 0:
		DrawTexture(GetImage(spriteSheet, sprites_brick01_idx), x, y);
		break;
	case 1:
		DrawTexture(GetImage(spriteSheet, sprites_brick02_idx), x, y);
		break;
	case 2:
		DrawTexture(GetImage(spriteSheet, sprites_brick03_idx), x, y);
		break;
	case 3:
		DrawTexture(GetImage(spriteSheet, sprites_brick04_idx), x, y);
		break;
	case 4:
		DrawTexture(GetImage(spriteSheet, sprites_brick05_idx), x, y);
		break;
	case 5:
		if (health == 2)
			DrawTexture(GetImage(spriteSheet, sprites_brick10_idx), x, y);
		else
			DrawTexture(GetImage(spriteSheet, sprites_brick00_idx), x, y);
		break;
	case 6:
		if (health == 2)
			DrawTexture(GetImage(spriteSheet, sprites_brick11_idx), x, y);
		else
			DrawTexture(GetImage(spriteSheet, sprites_brick06_idx), x, y);
		break;
	case 7:
		if (health == 2)
			DrawTexture(GetImage(spriteSheet, sprites_brick12_idx), x, y);
		else
			DrawTexture(GetImage(spriteSheet, sprites_brick07_idx), x, y);
		break;
	case 8:
		if (health == 2)
			DrawTexture(GetImage(spriteSheet, sprites_brick13_idx), x, y);
		else
			DrawTexture(GetImage(spriteSheet, sprites_brick08_idx), x, y);
		break;
	case 9:
		if (health == 2)
			DrawTexture(GetImage(spriteSheet, sprites_brick14_idx), x, y);
		else
			DrawTexture(GetImage(spriteSheet, sprites_brick09_idx), x, y);
		break;
	case 10:
		DrawTexture(GetImage(spriteSheet, sprites_brick15_idx), x, y);
		break;
	}
}

double *Brick::Error(double tX, double tY) {
	return new double[2]{ tX < x + width / 2.0 ? tX - x : tX - x - width, tY < y + height / 2.0 ? tY - y : tY - y - height };
}

bool Brick::Exists() {
	return health > 0;
}

int Brick::GetType() {
	return type;
}

void Brick::Hit() {
	--health;
}

bool Brick::Inside(double tX, double tY) {
	return tX >= x && tX <= x + width && tY >= y && tY <= y + height;
}

Brick::Brick(double sX, double sY, double sWidth, double sHeight, int sType) {
	x = sX;
	y = sY;
	width = sWidth;
	height = sHeight;
	type = sType;
	// I think I'm gonna change how many "types" there are, and add a 'skin' or 'texture' variable
	if (type >= 0 && type <= 4)
		health = 1;
	else if (type >= 5 && type <= 9)
		health = 2;
	else if (type == 10)
		health = 2147483647;
	else
		health = 0;
}

bool Brick::operator==(const Brick& other) {
	return x == other.x && y == other.y && width == other.width && height == other.height && type == other.type && health == other.health;
}
