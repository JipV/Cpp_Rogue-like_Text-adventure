#include "stdafx.h"
#include "GameController.h"

#include "iostream"
#include "fstream"

GameController::GameController()
{
}

GameController::~GameController()
{
	delete map_;
	Map* map_ = nullptr;

	delete hero_;
	Hero* hero_ = nullptr;
}

void GameController::startGame()
{
	map_ = createMap();
	hero_ = createHero();

	/*Hero player = Hero();
	player.setCurrentRoom(map.getStartLocation());

	std::cout << player.getCurrentRoom()->getDescription() << "\n\n";

	map.showMap();*/

	std::cin.get(); // Console blijft open
}

Map* GameController::createMap()
{
	/*int xSize;
	std::cout << "Hoeveel kamers breed wil je de kerker is: ";
	std::cin >> xSize;

	int ySize;
	std::cout << "Hoeveel kamers lang wil je de kerker is: ";
	std::cin >> ySize;

	int zSize;
	std::cout << "Hoeveel verdiepingen wil je de kerker heeft: ";
	std::cin >> zSize;

	Map map = Map(xSize, ySize, zSize);*/

	return new Map(10, 3, 3);
}

Hero* GameController::createHero()
{
	std::cout << "Wil je spelen met een nieuwe held of met een eerder opgeslagen held?\n";
	std::cout << "[new|load]\n";

	std::string action;
	std::cin >> action;

	while (action.compare("new") != 0 && action.compare("load") != 0) {
		std::cout << "Ingevoerde actie is niet herkent.\n";
		std::cout << "[new|load]\n";
		std::cin >> action;
	}

	if (action.compare("new") == 0) {
		std::cout << "Wat is de naam van de nieuwe held?\n";
		std::string name;
		std::cin >> name;

		// ER MOET NOG GECHECKT WORDEN OF DE NAAM NIET AL BESTAAT IN VERBAND MET OPSLAAN
		// ER MOET NOG GECHECHT WORDEN OF DE INGEVOERDE TEKENS GELDIG ZIJN
		while (name.length() < 3) {
			std::cout << "De moet minstens drie tekens lang zijn.\nVoer opnieuw een naam in.\n";
			std::cin >> name;
		}

		std::cout << "De naam voor de nieuwe held is: " << name << "\n";
		return new Hero(name);
	}
	else if (action.compare("load") == 0) {
		std::cout << "Wat is de naam van held?\n";
		std::string name;
		std::cin >> name;
		
		while (name.length() < 3) {
			std::cout << "De moet minstens drie tekens lang zijn.\nVoer opnieuw een naam in.\n";
			std::cin >> name;
		}

		std::cout << name << ".txt\n";

		std::ifstream input_file(name + ".txt");
		while (!input_file) {
			std::cout << "Er is geen held met die naam.\nVoer opnieuw een naam in.\n";
			std::cin >> name;

			std::ifstream input_file(name + ".txt");
		}

		std::cout << "Test.\n";

		std::string line;
		/*while (getline(input_file, line)) {
			//std::string optionName = line.substr(0, line.find(":"));
			//std::string option = line.substr(line.find(":") + 1, line.length());


		}*/

		input_file.close();

		std::cout << "Load hero\n";
	}
	return nullptr;
}



