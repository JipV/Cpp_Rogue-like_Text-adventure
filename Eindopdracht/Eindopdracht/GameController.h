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
	bool gameShouldRun();

private:
	Map* map_ = nullptr;
	Hero* hero_ = nullptr;

	bool isRunning_;
	bool exitGame_;

	Map* createMap();
	Hero* createHero();
};

#endif

