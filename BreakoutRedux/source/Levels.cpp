#include "Levels.hpp"

std::vector<int> GetLevelTypes(int lvl) {
	switch (lvl) {
	case 0:
		return std::vector<int> {
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
			4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
			3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};
	 case 1:
		 return std::vector<int> {
		 	5, 5, 0, 0, 0, 0, 0, 0, 0, 0,
			4, 4, 4, 4, 0, 0, 0, 0, 0, 0,
			3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
			2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};
	case 2:
		return std::vector<int> {
			0,  1, 0, 0, 0, 0, 0, 0,  1, 0,
			0, 10, 0, 0, 0, 0, 0, 0,  6, 0,
			0,  8, 0, 0, 0, 0, 0, 0,  9, 0,
			0,  9, 3, 1, 3, 4, 3, 2,  8, 0,
			0,  6, 3, 3, 5, 3, 2, 2, 10, 0
		};
	case 3:
		return std::vector<int> {
			0, 0, 1, 1, 2, 2, 1, 1, 0, 0,
			1, 1, 2, 2, 3, 3, 2, 2, 1, 1,
			2, 2, 3, 3, 9, 9, 3, 3, 2, 2,
			1, 1, 2, 2, 3, 3, 2, 2, 1, 1,
			0, 0, 1, 1, 2, 2, 1, 1, 0, 0
		};
	case 4:
		return std::vector<int> {
			5,  0,  3,  2,  0,  0,  2,  3,  0,  5,
			4,  8,  2,  1, 10, 10,  1,  2,  8,  4,
			0,  2,  0,  0,  9,  9,  0,  0,  2,  0,
			4,  8,  2,  1, 10, 10,  1,  2,  8,  4,
			5,  0,  3,  2,  0,  0,  2,  3,  0,  5
		};
	default:
		return std::vector<int> {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
	}
}

std::vector<int> GetLevelSize(int lvl) {
	switch (lvl) {
	default:
		return std::vector<int> { 10, 5 };
	}
}

std::vector<int> GetLevelBrickSize(int lvl) {
	switch (lvl) {
	default:
		return std::vector<int> { 36, 16 };
	}
}

std::vector<int> GetLevelSpacing(int lvl) {
	switch (lvl) {
	default:
		return std::vector<int> { 40, 20 };
	}
}

std::vector<Brick> *GetLevelBricks(int lvl) {
	std::vector<Brick> *retVec = new std::vector<Brick>;
	int ind = 0;
	std::vector<int> lvlSpacing = GetLevelSpacing(lvl);
	std::vector<int> lvlBrickSize = GetLevelBrickSize(lvl);
	std::vector<int> lvlSize = GetLevelSize(lvl);
	std::vector<int> lvlTypes = GetLevelTypes(lvl);
	for (int y = 0; y < lvlSize[1]; y++) {
		for (int x = 0; x < lvlSize[0]; x++) {
			retVec->push_back(Brick(2 + (lvlSpacing[0] * x), 2 + (lvlSpacing[1] * y), lvlBrickSize[0], lvlBrickSize[1], lvlTypes[ind]));
			ind++;
		}
	}
	return retVec;
}
