#include "stdafx.h"
#include "GameController.h"
#include "Room.h"
#include "Enemy.h"
#include "Random.h"
#include "Hero.h"
#include "Map.h"

GameController::GameController()
{
	isRunning_ = false;
	readEnemies();
}

GameController::~GameController()
{
	delete map_;
	map_ = nullptr;

	delete hero_;
	hero_ = nullptr;

	delete monsterOptions_;
	monsterOptions_ = nullptr;

	delete bossesOptions_;
	bossesOptions_ = nullptr;

	delete enemySizeOptions_;
	enemySizeOptions_ = nullptr;
}

void GameController::readEnemies()
{
	monsterOptions_ = new std::vector<std::string>();
	bossesOptions_ = new std::vector<std::string>();
	enemySizeOptions_ = new std::vector<std::string>();

	std::string line;
	std::ifstream input_file("enemy_options.txt");
	if (input_file) {
		while (getline(input_file, line)) {
			std::string optionName = line.substr(0, line.find(":"));
			std::string option = line.substr(line.find(":") + 1, line.length());

			if (optionName.compare("monsters") == 0) {
				monsterOptions_->push_back(option);
			}
			else if (optionName.compare("bosses") == 0) {
				bossesOptions_->push_back(option);
			}
			else if (optionName.compare("sizes") == 0) {
				enemySizeOptions_->push_back(option);
			}
		}
	}
	input_file.close();
}

void GameController::startGame()
{
	map_ = createMap();
	hero_ = createHero();
	createEnemies();

	std::cout << "\nDe game begint nu.\n";

	hero_->setCurrentRoom(map_->getStartLocation());

	isRunning_ = true;
	while (isRunning_) {
		std::vector<std::string>* actions = new std::vector<std::string>();
		hero_->getActions(actions);
		map_->getActions(actions);

		std::cout << "\nWat doe je?\n( ";
		for (auto iterator = actions->begin(); iterator != actions->end(); ++iterator) {
			std::cout << *iterator;
			if (iterator != --actions->end()) {
				std::cout << " | ";
			}
		}
		std::cout << " )\n";

		delete actions;

		std::cout << "\nActie: ";

		std::string action;
		std::getline(std::cin, action);

		std::vector<std::string> elems;
		std::stringstream ss(action);
		std::string item;
		while (std::getline(ss, item, ' ')) {
			elems.push_back(item);
		}

		if (elems.size() >= 1)
		{	// We kunnen geen lege acties afhandelen
			if (!(hero_->handleAction(elems) ||
				map_->handleAction(elems, hero_)))
			{
				std::cout << "actie \"" << action << "\" is niet geldig." << std::endl;
			}
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

void split(const std::string& s, char delim, std::vector<std::string>& v) {
	int i = 0;
	int pos = (int)s.find(delim);
	while (pos != std::string::npos) {
		v.push_back(s.substr(i, pos - i));
		i = ++pos;
		pos = (int)s.find(delim, pos);

		if (pos == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

void GameController::createEnemies()
{
	for (int z = 0; z < map_->getZSize(); z++) {
		for (int y = 0; y < map_->getYSize(); y++) {
			for (int x = 0; x <  map_->getXSize(); x++) {
				
				Room* room = map_->getRoom(x, y, z);
				int numberOfEnemies = Random::getRandomNumber(0, 3);

				for (int i = 0; i < numberOfEnemies; i++) {
					
					// Bepaal het soort monster
					std::string monsterOption = monsterOptions_->at(Random::getRandomNumber(0, (int)monsterOptions_->size() - 1));
					std::vector<std::string> monsterData = std::vector<std::string>();
					
					split(monsterOption, ',', monsterData);
					
					std::string monsterType = monsterData.at(0);
					std::string monsterHP = monsterData.at(1);
					std::string monsterAttack = monsterData.at(2);
					std::string monsterDefense = monsterData.at(3);
					std::string monsterChanceHeroEscapes = monsterData.at(4);

					// Bepaal de grootte van het monster
					std::string enemySizeOption = enemySizeOptions_->at(Random::getRandomNumber(0, (int)enemySizeOptions_->size() - 1));
					std::vector<std::string> enemySizeData = std::vector<std::string>();

					split(enemySizeOption, ',', enemySizeData);

					std::string enemySizeType = enemySizeData.at(0);
					std::string enemySizeHP = enemySizeData.at(1);
					std::string enemySizeAttack = enemySizeData.at(2);
					std::string enemySizeDefense = enemySizeData.at(3);
					std::string enemySizeChanceHeroEscapes = enemySizeData.at(4);

					// Bepaal de benodigde gegevens
					int totalHP = std::stoi(monsterHP) + std::stoi(enemySizeHP);
					int totalAttack = std::stoi(monsterAttack) + std::stoi(enemySizeAttack);
					int totalDefense = std::stoi(monsterDefense) + std::stoi(enemySizeDefense);
					int totalChanceHeroEscapes = std::stoi(monsterChanceHeroEscapes) + std::stoi(enemySizeChanceHeroEscapes);

					int min = 1;
					if ((z - 2) > 1) {
						min = (z - 2);
					}
					int level = Random::getRandomNumber(min, z + 1);

					room->addEnemy(new Enemy(monsterType, enemySizeType, level, totalHP, totalAttack, totalDefense, totalChanceHeroEscapes));
				}
			}
		}
	}
}

