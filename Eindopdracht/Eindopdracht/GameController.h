#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

class Map;
class Hero;

class GameController
{
public:
	GameController();
	~GameController();

	void startGame();

private:
	Map* map_ = nullptr;
	Hero* hero_ = nullptr;

	bool isRunning_;

	Map* createMap();
	Hero* createHero();
};

#endif

