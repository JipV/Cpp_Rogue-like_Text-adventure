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

	hero_->getCurrentRoom()->showDescription();
	hero_->getCurrentRoom()->showExits();

	isRunning_ = true;
	while (isRunning_) {
		

		std::cout << "\nWat doe je?\n";
		std::cout << "[loop verder | vecht | vlucht | zoek | rust uit | bekijk spullen | bekijk kaart]\n";
		std::cout << "\nActie: ";

		std::string action;
		std::getline(std::cin, action);

		while (	action.compare("loop verder") != 0 &&
				action.compare("vecht") != 0 &&
				action.compare("vlucht") != 0 &&
				action.compare("zoek") != 0 &&
				action.compare("rust uit") != 0 &&
				action.compare("bekijk spullen") != 0 &&
				action.compare("bekijk kaart") != 0) {
			std::cout << "\nIngevoerde actie is niet herkent. Voer opnieuw een actie in.\n";
			std::cout << "[loop verder | vecht | vlucht | zoek | rust uit | bekijk spullen | bekijk kaart]\n";
			std::cout << "\nActie: ";
			std::getline(std::cin, action);
		}

		if (action.compare("loop verder") == 0) {
			hero_->goToNextRoom(hero_->getCurrentRoom());
		}
		else if(action.compare("vecht") == 0) {
			// vecht
		}
		else if (action.compare("vlucht") == 0) {
			// vlucht
		}
		else if (action.compare("zoek") == 0) {
			// zoek
		}
		else if (action.compare("rust uit") == 0) {
			// rust uit
		}
		else if (action.compare("bekijk spullen") == 0) {
			// bekijk spullen
		}
		else if (action.compare("bekijk kaart") == 0) {
			map_->showMap();
		}
	}
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
	std::cout << "[new | load]\n";
	std::cout << "\nActie: ";

	std::string action;
	std::getline(std::cin, action);

	while (action.compare("new") != 0 && action.compare("load") != 0) {
		std::cout << "\nIngevoerde actie is niet herkent. Voor opnieuw een actie in.\n";
		std::cout << "[new | load]\n";
		std::cout << "\nActie: ";
		std::getline(std::cin, action);
	}

	if (action.compare("new") == 0) {
		std::cout << "\nWat is de naam van de nieuwe held?\n";
		std::cout << "\nNaam: ";
		std::string name;
		std::getline(std::cin, name);

		// ER MOET NOG GECHECKT WORDEN OF DE NAAM NIET AL BESTAAT IN VERBAND MET OPSLAAN
		// ER MOET NOG GECHECHT WORDEN OF DE INGEVOERDE TEKENS GELDIG ZIJN
		while (name.length() < 3) {
			std::cout << "\nDe moet minstens drie tekens lang zijn.\nVoer opnieuw een naam in.\n";
			std::cout << "\nNaam: ";
			std::getline(std::cin, name);
		}

		return new Hero(name);
	}
	else if (action.compare("load") == 0) {
		std::cout << "\nWat is de naam van held?\n";
		std::cout << "\nNaam: ";
		std::string name;
		std::getline(std::cin, name);

		std::ifstream input_file(name + ".txt");
		while (!input_file) {
			std::cout << "\nEr is geen held met die naam.\nVoer opnieuw een naam in.\n";
			std::cout << "\nNaam: ";
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



