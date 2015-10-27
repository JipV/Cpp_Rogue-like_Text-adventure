#include "stdafx.h"
#include "GameController.h"

#include "iostream"

int main()
{
	GameController gameController = GameController();
	gameController.startGame();

	std::cin.get(); // Console blijft open

	return 0;
}