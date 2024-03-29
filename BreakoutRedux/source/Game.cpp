#include "Game.hpp"

void Game::LoadLevel() {
	std::vector<Brick> *requestedData = GetLevel(currentLevelSet, currentLevel);
	for (Brick brick : *requestedData)
		bricks.push_back(new Brick(brick));
}

void Game::ResetBalls() {
	balls = std::vector<Ball*>();
	balls.push_back(new Ball(200, 200, 7));
}

void Game::Draw() {
	for (Brick *brick : bricks)
		if (brick->Exists())
			brick->Draw();
	paddle->Draw();
	//for each ball, for (int i = 7; i > 0; i--) draw-scaled extra ball[i], end loop, draw ball
	//[original code] pp2d_draw_texture_scale(extraBallID[i], (tBall.trail_new_frame_circle[i].x - tBall.trail_new_frame_circle[i].rad) + 1.0, (tBall.trail_new_frame_circle[i].y - tBall.trail_new_frame_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0); //RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i))
	for (Ball *ball : balls)
		ball->Draw();
	for (Powerup *powerup : powerups) {
		//C2D_DrawRectangle(powerup->x, powerup->y, 0, powerup->width, powerup->height, C2D_Color32(0x00, 0xFF, 0x00, 0xFF), C2D_Color32(0x00, 0xFF, 0x00, 0xFF), C2D_Color32(0x00, 0xFF, 0x00, 0xFF), C2D_Color32(0x00, 0xFF, 0x00, 0xFF));
		//continue;
		switch (powerup->type) {
			case 0:
				DrawTexture(GetImage(powerupSheet, powerups_laser00_idx + (int)(powerup->y / 8) % 6), powerup->x, powerup->y);
				break;
			case 1:
				DrawTexture(GetImage(powerupSheet, powerups_paddle_big00_idx + (int)(powerup->y / 8) % 6), powerup->x, powerup->y);
				break;
			case 2:
				DrawTexture(GetImage(powerupSheet, powerups_paddle_small00_idx + (int)(powerup->y / 8) % 6), powerup->x, powerup->y);
				break;
			case 3:
				DrawTexture(GetImage(powerupSheet, powerups_life00_idx + (int)(powerup->y / 8) % 6), powerup->x, powerup->y);
				break;
			case 4:
				DrawTexture(GetImage(powerupSheet, powerups_multi_ball00_idx + (int)(powerup->y / 8) % 6), powerup->x, powerup->y);
				break;
		}
	}
}

int Game::Lives() {
	if (balls.size() == 0) {
		--lives;
		ResetBalls();
		balls[0]->Update(bricks, *paddle);
	}
	bool nextLevel = true;
	for (Brick *brick : bricks)
		if (brick->GetType() != 10 && brick->Exists()) // I can probably remove the Exists check
			nextLevel = false;
	if (nextLevel) {
		ResetBalls();
		balls[0]->Update(bricks, *paddle);
		currentLevel++;
		LoadLevel();
	}
	return lives;
}

void Game::Move() {
	paddle->Move();
	for (size_t i = 0; i < balls.size(); ++i) {
		if (balls[i]->Exists()) {
			balls[i]->Move();
			balls[i]->Update(bricks, *paddle);
		}
		else
			balls.erase(balls.begin() + i);
	}
	// TODO: set ball trails, unless this is implemented inside the ball class
	double *coords = paddle->Coords();
	for (size_t i = 0; i < powerups.size(); ++i) {
		if (powerups[i]->y < 240) {
			++powerups[i]->y;
			if (powerups[i]->y <= coords[1] + coords[3] && powerups[i]->x <= coords[0] + coords[2] && powerups[i]->y + powerups[i]->height >= coords[1] && powerups[i]->x + powerups[i]->width >= coords[0]) {
				switch (powerups[i]->type) {
					case 0:
						//laser
						break;
					case 1:
						// big paddle
						break;
					case 2:
						// small paddle
						break;
					case 3:
						lives++;
						break;
					case 4:
						// multi-ball
						break;
				}
				powerups.erase(powerups.begin() + i);
			}
		}
		else
			powerups.erase(powerups.begin() + i);
	}
	for (size_t i = 0; i < bricks.size(); ++i) {
		if (!bricks[i]->Exists()) {
			switch (bricks[i]->GetType()) {
				case 0:
				case 1:
				case 5:
				case 6: {
					int chance = rand() % 20;
					if (chance <= 3)
						powerups.push_back(new Powerup{ bricks[i]->Coords()[0] + bricks[i]->Coords()[2] / 2, bricks[i]->Coords()[1] + bricks[i]->Coords()[3] / 2, 18, 7, rand() % 5 });
					break;
				}
				case 2:
				case 3:
				case 4:
				case 7:
				case 8:
				case 9: {
					int chance = rand() % 4;
					if (chance == 1)
						powerups.push_back(new Powerup{ bricks[i]->Coords()[0] + bricks[i]->Coords()[2] / 2, bricks[i]->Coords()[1] + bricks[i]->Coords()[3] / 2, 18, 7, rand() % 5 });
					break;
				}
			}
			bricks.erase(bricks.begin() + i);
		}
	}
}

void Game::MovePaddle(double amount) {
	paddle->speed = amount;
}

void Game::Reset(int levelSet) {
	lives = 3;
	currentLevelSet = levelSet;
	currentLevel = 0;
	LoadLevel();
	paddle = new Paddle(175, 215, 50, 5);
	ResetBalls();
	/*level = 1; points = 0; last_power = 0;
	times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;*/
}

void Game::Shoot() {
	for (Ball *ball : balls)
		if (ball->speed == 0.0)
			ball->speed = 2;
}

Game::Game(int levelSet) {
	Reset(levelSet);
}
