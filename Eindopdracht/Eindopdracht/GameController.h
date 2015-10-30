#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

class Map;
class Hero;

class GameController
{
	public:

		GameController();
		~GameController();

		void readEnemies();
		void startGame();
		Map* createMap();
		Hero* createHero();
		void createEnemies();

	private:

		Map* map_ = nullptr;
		Hero* hero_ = nullptr;

		std::vector<std::string>* monsterOptions_ = nullptr;
		std::vector<std::string>* bossesOptions_ = nullptr;
		std::vector<std::string>* enemySizeOptions_ = nullptr;

		bool isRunning_;
};

#endif

