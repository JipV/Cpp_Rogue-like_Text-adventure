#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

#include "Map.h"
#include "Hero.h"

class GameController
{
	public:

		GameController();
		~GameController();

		void startGame();
		Map* createMap();
		Hero* createHero();

	private:

		Map* map_ = nullptr;
		Hero* hero_ = nullptr;

		bool isRunning_;
};

#endif

