#include "BreakoutRedux.hpp"
#include "Player.hpp"
#include "Levels.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"

class Game {
	Player player;
	std::vector<Brick> *bricks;
	Paddle *paddle = new Paddle(175.0f, 215.0f, 50.0f, 10.0f);
	std::vector<Ball> *balls;
	int level;
	bool customLevel;

	void SetBricks() {
		bricks = GetLevelBricks(level);
	}
	void ResetBalls() {
		balls = new std::vector<Ball>;
		balls->push_back(Ball(200.0f, 200.0f, 7.0f));
	}
public:
	std::vector<Brick> *GetBricks() {
		return bricks;
	}
	Paddle *GetPaddle() {
		return paddle;
	}
	std::vector<Ball> *GetBalls() {
		return balls;
	}
	void Reset() {
		player = Player();
		level = 0;
		customLevel = false;
		SetBricks();
		paddle = new Paddle(175.0f, 215.0f, 50.0f, 10.0f);
		ResetBalls();
	}
	Game() {
		Reset();
	}
};
