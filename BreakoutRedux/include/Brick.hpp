#include "BreakoutRedux.hpp"

class Brick {
	float x, y;
	float width, height;
	int type;
	int health;
public:
	std::vector<float> Coords() {
		return std::vector<float>{ x, y, width, height };
	}
	bool Inside(float tX, float tY) {
		if (tX < x || tX > x + width || tY < y || tY > y + height) return false;
		return true;
	}
	std::vector<float> Error(float tX, float tY) {
		return std::vector<float> { tX < x + width / 2.0f ? tX - x : tX - x - width, tY < y + height / 2.0f ? tY - y : tY - y - height };
	}
	void Draw() {
		C2D_DrawRectangle(x, y, 0.0f, width, height, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
		C2D_DrawRectangle(x + 1.0f, y + 1.0f, 0.0f, width - 2.0f, height - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		return;
		//above is temp
		if (type == 0 || health == 0) return;
		switch (type) {
		case 1:
			DrawTexture(GetImage(spriteSheet, sprites_brick01_idx), x, y);
			break;
		case 2:
			DrawTexture(GetImage(spriteSheet, sprites_brick02_idx), x, y);
			break;
		case 3:
			DrawTexture(GetImage(spriteSheet, sprites_brick03_idx), x, y);
			break;
		case 4:
			DrawTexture(GetImage(spriteSheet, sprites_brick04_idx), x, y);
			break;
		case 5:
			DrawTexture(GetImage(spriteSheet, sprites_brick05_idx), x, y);
			break;
		case 6:
			if (health == 2) DrawTexture(GetImage(spriteSheet, sprites_brick10_idx), x, y);
			else DrawTexture(GetImage(spriteSheet, sprites_brick00_idx), x, y);
			break;
		case 7:
			if (health == 2) DrawTexture(GetImage(spriteSheet, sprites_brick11_idx), x, y);
			else DrawTexture(GetImage(spriteSheet, sprites_brick06_idx), x, y);
			break;
		case 8:
			if (health == 2) DrawTexture(GetImage(spriteSheet, sprites_brick12_idx), x, y);
			else DrawTexture(GetImage(spriteSheet, sprites_brick07_idx), x, y);
			break;
		case 9:
			if (health == 2) DrawTexture(GetImage(spriteSheet, sprites_brick13_idx), x, y);
			else DrawTexture(GetImage(spriteSheet, sprites_brick08_idx), x, y);
			break;
		case 10:
			if (health == 2) DrawTexture(GetImage(spriteSheet, sprites_brick14_idx), x, y);
			else DrawTexture(GetImage(spriteSheet, sprites_brick09_idx), x, y);
			break;
		}
	}
	bool Exists() {
		return health > 0;
	}
	void Hit() {
		health--;
	}
	Brick(float sX, float sY, float sWidth, float sHeight, int sType) {
		x = sX;
		y = sY;
		width = sWidth;
		height = sHeight;
		type = sType;
		if (type >= 1 && type <= 5)
			health = 1;
		else if (type >= 6 && type <= 10)
			health = 2;
		else
			health = 0;
	}
	bool operator==(const Brick& other) {
		return x == other.x && y == other.y && width == other.width && height == other.height && type == other.type && health == other.health;
	}
};
