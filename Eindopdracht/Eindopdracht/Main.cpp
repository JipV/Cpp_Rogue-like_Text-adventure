#include "stdafx.h"
#include "GameController.h"

int main()
{
	GameController gameController = GameController();

	while (gameController.gameShouldRun())
	{
		gameController.startGame();
	}

	return 0;
}