#include "stdafx.h"
#include "Generators.h"
#include "Random.h"
#include "Map.h"
#include "Enemy.h"
#include "Trap.h"

void split(const std::string& s, char delim, std::vector<std::string>& v) {
	int i = 0;
	int pos = static_cast<int>(s.find(delim));
	while (pos != std::string::npos) {
		v.push_back(s.substr(i, pos - i));
		i = ++pos;
		pos = static_cast<int>(s.find(delim, pos));

		if (pos == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

// ---------------------------- ENEMY GENERATOR ----------------------------

EnemyGenerator::EnemyGenerator()
{
	monsterOptions_ = std::vector<std::string>();
	bossesOptions_ = std::vector<std::string>();
	enemySizeOptions_ = std::vector<std::string>();

	std::string line;
	std::ifstream input_file("enemy_options.txt");
	if (input_file) {
		while (getline(input_file, line)) {
			std::string optionName = line.substr(0, line.find(":"));
			std::string option = line.substr(line.find(":") + 1, line.length());

			if (optionName.compare("monsters") == 0) {
				monsterOptions_.push_back(option);
			}
			else if (optionName.compare("bosses") == 0) {
				bossesOptions_.push_back(option);
			}
			else if (optionName.compare("sizes") == 0) {
				enemySizeOptions_.push_back(option);
			}
		}
	}
	input_file.close();
}

EnemyGenerator::~EnemyGenerator()
{
}

Enemy* EnemyGenerator::createEnemy(int z)
{
	// Maak een enemy aan
	Enemy* result = createEnemy(monsterOptions_.at(Random::getRandomNumber(0, static_cast<int>(monsterOptions_.size()) - 1)));

	// Bepaal de grootte van het monster
	result = modifyEnemy(result, enemySizeOptions_.at(Random::getRandomNumber(0, static_cast<int>(enemySizeOptions_.size()) - 1)));

	return result;
}

Enemy* EnemyGenerator::createEnemy(std::string stats)
{
	std::vector<std::string> monsterData = std::vector<std::string>();

	split(stats, ',', monsterData);

	return new Enemy(monsterData.at(0),
		"normaal",
		std::stoi(monsterData.at(1)),
		std::stoi(monsterData.at(2)),
		std::stoi(monsterData.at(3)),
		std::stoi(monsterData.at(4)),
		std::stoi(monsterData.at(5)),
		std::stoi(monsterData.at(6)));
}

Enemy* EnemyGenerator::modifyEnemy(Enemy* enemy, std::string modifiers)
{
	std::vector<std::string> modifierData = std::vector<std::string>();

	split(modifiers, ',', modifierData);

	enemy->size_ = modifierData.at(0);
	enemy->level_ += std::stoi(modifierData.at(1));
	enemy->maxHP_ += std::stoi(modifierData.at(2));
	enemy->currentHP_ += std::stoi(modifierData.at(2));
	enemy->chanceToHit_ += std::stoi(modifierData.at(3));
	enemy->chanceToDefend_ += std::stoi(modifierData.at(4));
	enemy->attack_ += std::stoi(modifierData.at(5));
	enemy->chanceHeroEscapes_ += std::stoi(modifierData.at(6));

	return enemy;
}

// ---------------------------- TRAP GENERATOR ----------------------------

TrapGenerator::TrapGenerator(EnemyGenerator* enemyGenerator) : enemyGenerator_(enemyGenerator)
{
	std::string line;
	std::ifstream input_file("traps.txt");

	Trap* currentTrap = nullptr;

	if (input_file) {
		while (getline(input_file, line)) {
			if (line.empty())
			{
				saveTrap(currentTrap);
				currentTrap = new Trap();
			}
			else if (line.find("//") != 0)
			{
				parseLine(line, currentTrap);
			}
		}
		saveTrap(currentTrap);
	}
	input_file.close();
}

TrapGenerator::~TrapGenerator()
{
	enemyGenerator_ = nullptr; // RoomGenerator ruimt deze op

	for (auto it = possibleTraps_.begin(); it != possibleTraps_.end(); ++it)
	{
		std::for_each(it->second.begin(), it->second.end(), [](Trap* t)
		{
			delete t;
		});
	}
}

Trap* TrapGenerator::createTrap(int z)
{
	//TODO: hoeveel kans willen we dat een kamer een val heeft?
	if (Random::getRandomNumber(1,100) <= 10)
	{
		std::vector<Trap*> allowedTraps;
		for (int i = z; i <= z + 2; i++)
		{
			allowedTraps.insert(allowedTraps.end(), possibleTraps_[i].begin(), possibleTraps_[i].end());
		}

		if (allowedTraps.size() > 0)
		{
			// Copy constructor
			return new Trap(*allowedTraps[Random::getRandomNumber(0,allowedTraps.size()-1)]);
		}
	}

	return nullptr;
}

void TrapGenerator::parseLine(std::string line, Trap* trap)
{
	std::vector<std::string> splitLine = std::vector<std::string>();
	split(line, ':', splitLine);

	if (splitLine.size() == 2)
	{
		std::string type = splitLine[0];
		std::string value = splitLine[1];

		if (type == "level")
			trap->level_ = std::stoi(value);
		else if (type == "roomDescription")
			trap->roomDescription_ = value;
		else if (type == "findDescription")
			trap->findDescription_ = value;
		else if (type == "triggerDescription")
			trap->triggerDescription_ = value;
		else if (type == "chanceToFind")
			trap->chanceToFind_ = std::stoi(value);
		else if (type == "chanceToTrigger")
			trap->chanceToTrigger_ = std::stoi(value);
		else if (type == "triggerCommand")
			split(value, ',', trap->triggerCommands_);
		else if (type == "directDamage")
			trap->directDamage_ = std::stoi(value);
		else if (type == "damageOverTime")
			trap->damageOverTime_ = std::stoi(value);
		else if (type == "damageOverTimeTurns")
			trap->damageOverTimeTurns_ = std::stoi(value);
		else if (type == "damageOverTimeMessage")
			trap->damageOverTimeMessage_ = value;
		else if (type == "damageOverTimeDone")
			trap->damageOverTimeDone_ = value;
		else if (type == "summonMonster")
		{
			trap->enemiesToSummon_.push_back(enemyGenerator_->createEnemy(value));
		}
	}
}

void TrapGenerator::saveTrap(Trap* trap)
{
	if (trap != nullptr && trap->level_ != -1)
		possibleTraps_[trap->level_].push_back(trap);
}

// ---------------------------- ROOM GENERATOR ----------------------------

RoomGenerator::RoomGenerator() : x_(-1), y_(-1), specialType_(Room::ROOM_TYPE::NormalRoom)
{
	std::string line;
	std::ifstream input_file("room_options.txt");
	if (input_file) {
		while (getline(input_file, line)) {
			std::string optionName = line.substr(0, line.find(":"));
			std::string option = line.substr(line.find(":") + 1, line.length());

			if (optionName.compare("size") == 0) {
				sizeOptions_.push_back(option);
			}
			else if (optionName.compare("floor") == 0) {
				floorOptions_.push_back(option);
			}
			else if (optionName.compare("decor") == 0) {
				decorOptions_.push_back(option);
			}
			else if (optionName.compare("chest") == 0) {
				chestOptions_.push_back(option);
			}
			else if (optionName.compare("tidy") == 0) {
				tidyOptions_.push_back(option);
			}
			else if (optionName.compare("lighting") == 0) {
				lightingOptions_.push_back(option);
			}
			else if (optionName.compare("atmosphere") == 0) {
				atmosphereOptions_.push_back(option);
			}
		}
	}
	input_file.close();

	enemyGenerator_ = new EnemyGenerator();
	trapGenerator_ = new TrapGenerator(enemyGenerator_);
}

RoomGenerator::~RoomGenerator()
{
	delete trapGenerator_;
	delete enemyGenerator_;
}

void RoomGenerator::setSpecialRoom(int x, int y, Room::ROOM_TYPE type)
{
	x_ = x;
	y_ = y;
	specialType_ = type;
}

Room* RoomGenerator::createRoom(int x, int y, int z)
{
	Room* result;
	Room::ROOM_TYPE createType = Room::ROOM_TYPE::NormalRoom;

	if (x_ == x &&
		y_ == y)
	{	// Hier willen we een speciale room
		createType = specialType_;
	}

	switch (createType)
	{
	case Room::ROOM_TYPE::StairsDown:
		result = new Room(createType, "Je staat in een kamer met een trap naar beneden.", z);
		break;
	case Room::ROOM_TYPE::StairsUp:
		result = new Room(createType, "Je staat in een kamer met een trap naar boven.", z);
		break;
	case Room::ROOM_TYPE::EndEnemy:
		result = new Room(createType, "Je staat in een grote kamer, het lijkt wel of hier ooit iets groots leefde...", z);
		// TODO: add boss enemy
		break;
	case Room::ROOM_TYPE::StartLocation:
		result = new Room(createType, generateDescription(), z);
		break;
	case Room::ROOM_TYPE::NormalRoom:
		result = new Room(createType, generateDescription(), z);
		addEnemies(result, z);
		addTraps(result, z);
		break;
	default:
		result = new Room(Room::ROOM_TYPE::NormalRoom, generateDescription(), z);
		break;
	}

	return result;
}

std::string RoomGenerator::generateDescription()
{
	return "Beschrijving: "
		+ sizeOptions_.at(Random::getRandomNumber(0, sizeOptions_.size() - 1)) + " "
		+ floorOptions_.at(Random::getRandomNumber(0, floorOptions_.size() - 1)) + " "
		+ decorOptions_.at(Random::getRandomNumber(0, decorOptions_.size() - 1)) + " "
		+ chestOptions_.at(Random::getRandomNumber(0, chestOptions_.size() - 1)) + " "
		+ tidyOptions_.at(Random::getRandomNumber(0, tidyOptions_.size() - 1)) + " "
		+ lightingOptions_.at(Random::getRandomNumber(0, lightingOptions_.size() - 1)) + " "
		+ atmosphereOptions_.at(Random::getRandomNumber(0, atmosphereOptions_.size() - 1)) + " ";
}

void RoomGenerator::addEnemies(Room* room, int z)
{
	int numEnemies = Random::getRandomNumber(0, 3);
	for (int i = 0; i < numEnemies; i++)
	{
		room->addEnemy(enemyGenerator_->createEnemy(z));
	}
}

void RoomGenerator::addTraps(Room* room, int z)
{
	Trap* trap = trapGenerator_->createTrap(z);

	if (trap != nullptr)
		room->setTrap(trap);
}

// ---------------------------- MAP GENERATOR ----------------------------

MapGenerator::MapGenerator()
{
	roomGenerator_ = new RoomGenerator();
}

MapGenerator::~MapGenerator()
{
	delete roomGenerator_;
	roomGenerator_ = nullptr;
}

Map* MapGenerator::generateMap(int xSize, int ySize, int zSize)
{
	Map* result = new Map(xSize, ySize, zSize);

	// Genereer een random startpositie
	int x = Random::getRandomNumber(0, xSize - 1),
		y = Random::getRandomNumber(0, ySize - 1);

	// Variabelen voor het eindpunt van de vorige verdieping
	int endX = x,
		endY = y;

	// Loop door alle verdiepingen
	for (int z = 0; z < zSize; z++)
	{
		// Eerste verdieping, start met een startlocation.
		if (z == 0)
		{
			roomGenerator_->setSpecialRoom(endX, endY, Room::ROOM_TYPE::StartLocation);

			result->startLocation_ = roomGenerator_->createRoom(x, y, z);

			result->addRoom(result->startLocation_, x, y, z);
		}
		// Niet de eerste verdieping, maak een stairs up
		else
		{
			roomGenerator_->setSpecialRoom(endX, endY, Room::ROOM_TYPE::StairsUp);

			x = endX;
			y = endY;

			Room* stairsUp = roomGenerator_->createRoom(x, y, z);
			result->addRoom(stairsUp, x, y, z);

			Room* stairsDown = result->getRoom(x, y, z - 1);

			stairsUp->addExit("omhoog", stairsDown);
			stairsDown->addExit("omlaag", stairsUp);
		}

		// Genereer random eindpositie die niet gelijk is aan de startpositie
		while (x == endX &&
			y == endY)
		{
			endX = Random::getRandomNumber(0, xSize - 1);
			endY = Random::getRandomNumber(0, ySize - 1);
		}

		if (z < zSize - 1)
			roomGenerator_->setSpecialRoom(endX, endY, Room::ROOM_TYPE::StairsDown);
		else
			roomGenerator_->setSpecialRoom(endX, endY, Room::ROOM_TYPE::EndEnemy);

		// Genereer de maze voor deze verdieping
		addRoom(x, y, z, result);
	}

	return result;
}

Room* MapGenerator::addRoom(int x, int y, int z, Map* map)
{
	Room* currentRoom = map->getRoom(x, y, z);
	if (currentRoom == nullptr) // Start positie/stairs down is al geset.
	{
		currentRoom = roomGenerator_->createRoom(x, y, z);
		map->addRoom(currentRoom, x, y, z);
	}

	std::array<std::string, 4> possibleExits = { "noord", "oost", "zuid", "west" };

	for (int i = 3; i >= 0; i--)
	{
		// Kies een random exit om te verwerken
		int exitToCheck = Random::getRandomNumber(0, i);
		std::string currentExit = possibleExits[exitToCheck];
		std::string oppositeExit = "";

		// Zet de huidige exit op zijn plek. De huidige plek word niet meer gebruikt.
		possibleExits[exitToCheck] = possibleExits[i];

		int newX = x,
			newY = y;

		// Bereken nieuwe coordinaten en exit van de nieuwe room
		if (currentExit == "noord")
		{
			newY--;
			oppositeExit = "zuid";
		}
		else if (currentExit == "zuid")
		{
			oppositeExit = "noord";
			newY++;
		}
		else if (currentExit == "oost")
		{
			oppositeExit = "west";
			newX++;
		}
		else if (currentExit == "west")
		{
			oppositeExit = "oost";
			newX--;
		}

		// Als de coordinaten geldig zijn, genereer een nieuwe kamer. 
		if (newX >= 0 &&
			newX < map->xSize_ &&
			newY >= 0 &&
			newY < map->ySize_ &&
			map->rooms_[map->index(newX, newY, z)] == nullptr)
		{
			Room* newRoom = addRoom(newX, newY, z, map); // Recursieve call om een nieuwe room te genereren

			currentRoom->addExit(currentExit, newRoom);
			newRoom->addExit(oppositeExit, currentRoom);
		}
	}

	return currentRoom;
}