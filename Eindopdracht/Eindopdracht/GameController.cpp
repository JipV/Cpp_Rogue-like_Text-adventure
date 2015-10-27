#include "stdafx.h"
#include "GameController.h"

#include "iostream"
#include "fstream"

GameController::GameController()
{
	isRunning_ = false;
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

	hero_->setCurrentRoom(map_->getStartLocation());

	std::cout << "\nDe game begint nu.\n";

	isRunning_ = true;

	while (isRunning_) {
		std::cout << "\n" << hero_->getCurrentRoom()->getDescription() << "\n";

		std::cout << "\nWat doe je?\n";
		std::cout << "[bekijk kaart]\n";

		std::string action;
		std::getline(std::cin, action);

		while (action.compare("bekijk kaart") != 0) {
			std::cout << "\nIngevoerde actie is niet herkent.\n";
			std::cout << "[bekijk kaart]\n";
			std::getline(std::cin, action);
		}

		if (action.compare("bekijk kaart") == 0) {
			map_->showMap();
		}
	}
	std::cin.get(); // Console blijft open
}

Map* GameController::createMap()
{
	std::string xSize;
	std::cout << "Hoeveel kamers breed wil je de kerker is: ";
	std::getline(std::cin, xSize);

	std::string ySize;
	std::cout << "\nHoeveel kamers lang wil je de kerker is: ";
	std::getline(std::cin, ySize);

	std::string zSize;
	std::cout << "\nHoeveel verdiepingen wil je de kerker heeft: ";
	std::getline(std::cin, zSize);

	// ER MOET NOG WORDEN GECHECKT OF ER GELDIGE WAARDEN ZIJN INGEVOERD
	return new Map(std::stoi(xSize), std::stoi(ySize), std::stoi(zSize));
}

Hero* GameController::createHero()
{
	std::cout << "\nWil je spelen met een nieuwe held of met een eerder opgeslagen held?\n";
	std::cout << "[new|load]\n";

	std::string action;
	std::getline(std::cin, action);

	while (action.compare("new") != 0 && action.compare("load") != 0) {
		std::cout << "\nIngevoerde actie is niet herkent.\n";
		std::cout << "[new|load]\n";
		std::getline(std::cin, action);
	}

	if (action.compare("new") == 0) {
		std::cout << "\nWat is de naam van de nieuwe held?\n";
		std::string name;
		std::getline(std::cin, name);

		// ER MOET NOG GECHECKT WORDEN OF DE NAAM NIET AL BESTAAT IN VERBAND MET OPSLAAN
		// ER MOET NOG GECHECHT WORDEN OF DE INGEVOERDE TEKENS GELDIG ZIJN
		while (name.length() < 3) {
			std::cout << "\nDe moet minstens drie tekens lang zijn.\nVoer opnieuw een naam in.\n";
			std::getline(std::cin, name);
		}

		return new Hero(name);
	}
	else if (action.compare("load") == 0) {
		std::cout << "\nWat is de naam van held?\n";
		std::string name;
		std::getline(std::cin, name);

		std::ifstream input_file(name + ".txt");
		while (!input_file) {
			std::cout << "\nEr is geen held met die naam.\nVoer opnieuw een naam in.\n";
			std::getline(std::cin, name);

			input_file.open(name + ".txt");
		}

		Hero* newHero = new Hero(name);

		std::string line;
		while (getline(input_file, line)) {
			std::string propertyName = line.substr(0, line.find(":"));
			std::string propertyValue = line.substr(line.find(":") + 1, line.length());

			if (propertyName == "level") {
				newHero->setLevel(std::stoi(propertyValue));
			}
			else if (propertyName == "hp") {
				newHero->setHP(std::stoi(propertyValue));
			}
			else if (propertyName == "xp") {
				newHero->setXP(std::stoi(propertyValue));
			}
			else if (propertyName == "attack") {
				newHero->setAttack(std::stoi(propertyValue));
			}
			else if (propertyName == "defense") {
				newHero->setDefense(std::stoi(propertyValue));
			}
			else if (propertyName == "mindfulness") {
				newHero->setMindfulness(std::stoi(propertyValue));
			}
		}
		input_file.close();

		return newHero;
	}
	return nullptr;
}



