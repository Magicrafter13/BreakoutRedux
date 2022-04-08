#include "BreakoutRedux.hpp"
#include "Levels.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"

struct Powerup {
	double x, y;
	double width, height;
	int type;
};

class Game {
	std::vector<Brick*> bricks;
	std::vector<Powerup*> powerups;
	Paddle *paddle = new Paddle(0, 0, 0, 0);
	std::vector<Ball*> balls;
	int currentLevelSet, currentLevel;
	int lives;

	void LoadLevel();
	void ResetBalls();

public:
	void Draw();
	int Lives();
	void Move();
	void MovePaddle(double amount);
	void Reset(int levelSet);
	void Shoot();

	Game(int levelSet);
};
