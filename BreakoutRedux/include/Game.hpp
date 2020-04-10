#include "BreakoutRedux.hpp"
#include "Player.hpp"
#include "Levels.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"

class Game {
	//Player player;
	std::vector<Brick*> bricks;
	Paddle *paddle = new Paddle(0.0f, 0.0f, 0.0f, 0.0f);
	std::vector<Ball*> balls;
	int level;
	bool customLevel;
	int lives;

	void SetBricks() {
		// There's gotta be a better way to do this... I'm making sure that the Game object has copies of the brick data, just to make sure I don't modify the actual level data.
		std::vector<Brick> *tBricks = GetLevelBricks(level);
		bricks = std::vector<Brick*>();
		for (Brick brick : *tBricks) {
			Brick *newBrick = new Brick(brick);
			bricks.push_back(newBrick);
		}
	}
	void ResetBalls() {
		balls = std::vector<Ball*>();
		balls.push_back(new Ball(200.0f, 200.0f, 7.0f, bricks, *paddle));
	}
public:
	std::vector<Ball*> GetBalls() {
		return balls;
	}
	void Shoot() {
		for (Ball* ball : balls)
			if (ball->speed == 0.0f)
				ball->speed = 60.0f;
	}
	void Move() {
		for (size_t i = 0; i < balls.size(); i++) {
			if (balls[i]->Exists()) {
				balls[i]->Move();
				balls[i]->Update(bricks, *paddle);
			}
			else
				balls.erase(balls.begin() + i);
		}
		//set ball trails, unless this is implemented inside the ball class
		//for each powerup, move add one to its y position
		//if powerup is off screen, remove it from the arraylist of onscreen powerups
	}
	void MovePaddle(int amount) {
		paddle->Move(amount);
	}
	int Lives() {
		if (balls.size() == 0) {
			lives--;
			ResetBalls();
			balls[0]->Update(bricks, *paddle);
		}
		return lives;
	}
	void Draw() {
		for (Brick *brick : bricks)
			if (brick->Exists())
				brick->Draw();
		paddle->Draw();
		//for each ball, for (int i = 7; i > 0; i--) draw-scaled extra ball[i], end loop, draw ball
		//[original code] pp2d_draw_texture_scale(extraBallID[i], (tBall.trail_new_frame_circle[i].x - tBall.trail_new_frame_circle[i].rad) + 1.0, (tBall.trail_new_frame_circle[i].y - tBall.trail_new_frame_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0); //RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i))
		for (Ball* ball : balls)
			ball->Draw();
		//draw powerups
	}
	void Reset() {
		//player = Player();
		lives = 3;
		level = 0;
		customLevel = false;
		SetBricks();
		paddle = new Paddle(175.0f, 215.0f, 50.0f, 10.0f);
		ResetBalls();
		/*level = 1; points = 0; last_power = 0;
		times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;*/
	}
	Game() {
		Reset();
	}
};
