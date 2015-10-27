#include "stdafx.h"
#include "Map.h"
#include "Hero.h"

#include "iostream"

int main()
{
	Map map = Map(10, 3, 3);

	Hero player = Hero();
	player.setCurrentRoom(map.getStartLocation());

	std::cout << player.getCurrentRoom()->getDescription() << "\n\n";

	//map.showMap();

	std::cin.get(); // Console blijft open

	return 0;
}