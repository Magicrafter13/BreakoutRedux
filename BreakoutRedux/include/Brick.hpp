#include "BreakoutRedux.hpp"

class Brick {
	float x, y;
	float width, height;
	int type;
	int health;
public:
	bool Inside(float tX, float tY) {
		if (tX < x || tX > x + width || tY < y || tY > y + height) return false;
		return true;
	}
	void Draw() {
		C2D_DrawRectangle(x, y, 0.0f, width, height, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
		C2D_DrawRectangle(x + 1.0f, y + 1.0f, 0.0f, width - 2.0f, height - 2.0f, C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF), C2D_Color32(0x95, 0x95, 0x95, 0xFF));
		/*for (int drawY = 0; drawY <= (int)height; drawY += (int)height) {
			for (int drawX = 0; drawX <= (int)width; drawX++) {
				DrawTexture(GetImage(spriteSheet, sprites_blue_idx), (float)drawX + x, (float)drawY + y);
				//C2D_DrawRectangle((float)drawX + x, (float)drawY + y, 0.0f, 1.0f, 1.0f, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
			}
		}
		for (int drawX = 0; drawX <= (int)width; drawX += (int)width) {
			for (int drawY = 0; drawY <= (int)height; drawY++) {
				DrawTexture(GetImage(spriteSheet, sprites_blue_idx), (float)drawX + x, (float)drawY + y);
				//C2D_DrawRectangle((float)drawX + x, (float)drawY + y, 0.0f, 1.0f, 1.0f, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
			}
		}*/
		//C2D_DrawRectangle(x, y, 0.0f, width, height, C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF), C2D_Color32(0x00, 0x00, 0xFF, 0xFF));
		/*C2D_DrawTriangle(x, y, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x + width, y, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x + (width / 2.0f), y + 1.0f, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), 0.0f);
		C2D_DrawTriangle(x, y, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x, y + height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x + 1.0f, y + (height / 2.0f), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), 0.0f);
		C2D_DrawTriangle(x + width, y + height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x + width, y, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x - 1.0f + width, y + (height / 2.0f), C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), 0.0f);
		C2D_DrawTriangle(x + width, y + height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x, y + height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), x + (width / 2.0f), y - 1.0f + height, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF), 0.0f);*/
		//DrawTexture(GetImage(spriteSheet, sprites_FFFFFF_1_idx), x, y, 0.0f, C2D_ImageTint(C2D_Tint {, 255, 0, 255), 1.0f, height);
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
	bool exists() {
		return health > 0;
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
};
