#include "stdafx.h"
#include "Map.h"
#include "Hero.h"

#include "iostream"

int main()
{
	/*int xSize;
	std::cout << "Hoeveel kamers breed wil je de kerker is: \n";
	std::cin >> xSize;

	int ySize;
	std::cout << "Hoeveel kamers lang wil je de kerker is: \n";
	std::cin >> ySize;

	int zSize;
	std::cout << "Hoeveel verdiepingen wil je de kerker heeft: \n";
	std::cin >> zSize;

	Map map = Map(xSize, ySize, zSize);*/

	Map map = Map(10, 3, 3);

	Hero player = Hero();
	player.setCurrentRoom(map.getStartLocation());

	std::cout << player.getCurrentRoom()->getDescription() << "\n\n";

	map.showMap();

	std::cin.get(); // Console blijft open

	return 0;
}