#include "stdafx.h"
#include "Generators.h"
#include "Random.h"
#include "Map.h"
#include "Enemy.h"
#include "Trap.h"
#include "Weapon.h"
#include "Shield.h"

void split(const std::string& s, char delim, std::vector<std::string>& v) {
	int i = 0;
	int pos = static_cast<int>(s.find(delim));
	while (pos != std::string::npos) {
		v.push_back(s.substr(i, pos - i));
		i = ++pos;
		pos = static_cast<int>(s.find(delim, pos));
	}
	v.push_back(s.substr(i, s.length()));
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

	std::for_each(monsterOptions_.begin(), monsterOptions_.end(), [this](std::string monsterOption)
	{
		std::for_each(enemySizeOptions_.begin(), enemySizeOptions_.end(), [this,monsterOption](std::string sizeOption)
		{
			Enemy* enemy = createEnemy(monsterOption);
			modifyEnemy(enemy, sizeOption);
			possibleEnemies_[enemy->level_].push_back(enemy);
		});
	});
}

EnemyGenerator::~EnemyGenerator()
{
	for (auto it = possibleEnemies_.begin(); it != possibleEnemies_.end(); ++it)
	{
		std::for_each(it->second.begin(), it->second.end(), [](Enemy* e)
		{
			delete e;
		});
	}
}

Enemy* EnemyGenerator::createEnemy(int z)
{
	std::vector<Enemy*> allowedEnemies;
	for (int i = z; i <= z + 2; i++)
	{
		allowedEnemies.insert(allowedEnemies.end(), possibleEnemies_[i].begin(), possibleEnemies_[i].end());
	}

	if (allowedEnemies.size() > 0)
	{
		// Copy constructor
		return new Enemy(*allowedEnemies[Random::getRandomNumber(0, static_cast<int>(allowedEnemies.size()) - 1)]);
	}

	return nullptr;
}

Enemy* EnemyGenerator::createBoss(int z)
{
	// Maak een enemy aan
	Enemy* result = createEnemy(bossesOptions_.at(Random::getRandomNumber(0, static_cast<int>(bossesOptions_.size()) - 1)));

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
	if (Random::getRandomNumber(1, 100) <= 100)
	{
		std::vector<Trap*> allowedTraps;
		for (int i = z; i <= z + 2; i++)
		{
			allowedTraps.insert(allowedTraps.end(), possibleTraps_[i].begin(), possibleTraps_[i].end());
		}

		if (allowedTraps.size() > 0)
		{
			// Copy constructor
			return new Trap(*allowedTraps[Random::getRandomNumber(0, static_cast<int>(allowedTraps.size()) - 1)]);
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
		else if (type == "triggerCommands")
		{
			trap->triggerCommands_ = {};
			split(value, ',', trap->triggerCommands_);
		}
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

// ---------------------------- WEAPON GENERATOR ----------------------------

WeaponGenerator::WeaponGenerator()
{
	weaponOptions_ = std::vector<std::string>();
	weaponMaterialOptions_ = std::vector<std::string>();

	std::string line;
	std::ifstream input_file("weapon_options.txt");
	if (input_file) {
		while (getline(input_file, line)) {
			std::string optionName = line.substr(0, line.find(":"));
			std::string option = line.substr(line.find(":") + 1, line.length());

			if (optionName.compare("weapon") == 0) {
				weaponOptions_.push_back(option);
			}
			else if (optionName.compare("weaponMaterial") == 0) {
				weaponMaterialOptions_.push_back(option);
			}
		}
	}
	input_file.close();

	for (size_t i = 0; i < weaponOptions_.size(); i++) {
		for (size_t j = 0; j < weaponMaterialOptions_.size(); j++) {
			Weapon* weapon = parseLine(weaponOptions_.at(i), weaponMaterialOptions_.at(j));
			possibleWeapons_[weapon->level_].push_back(weapon);
		}
	}

}

WeaponGenerator::~WeaponGenerator()
{
	for (auto it = possibleWeapons_.begin(); it != possibleWeapons_.end(); ++it)
	{
		std::for_each(it->second.begin(), it->second.end(), [](Weapon* w)
		{
			delete w;
		});
	}
}

Weapon* WeaponGenerator::parseLine(std::string weaponOption, std::string weaponMaterialOption)
{
	// Gegevens soort wapen
	std::vector<std::string> weaponData = std::vector<std::string>();

	split(weaponOption, ',', weaponData);

	std::string weaponType = weaponData.at(0);
	std::string weaponLevel = weaponData.at(1);
	std::string weaponAttack = weaponData.at(2);

	// Gegevens materiaal van het wapen
	std::vector<std::string> weaponMaterialData = std::vector<std::string>();

	split(weaponMaterialOption, ',', weaponMaterialData);

	std::string weaponMaterialType = weaponMaterialData.at(0);
	std::string weaponMaterialLevel = weaponMaterialData.at(1);
	std::string weaponMaterialAttack = weaponMaterialData.at(2);

	// Bepaal de benodigde gegevens
	int totalLevel = std::stoi(weaponLevel) + std::stoi(weaponMaterialLevel);
	int totalAttack = std::stoi(weaponAttack) + std::stoi(weaponMaterialAttack);

	return new Weapon(weaponType, weaponMaterialType, totalLevel, totalAttack);
}

Weapon* WeaponGenerator::createWeapon(int z)
{
	//TODO: hoeveel kans willen we dat een kamer een wapen heeft?
	if (Random::getRandomNumber(1, 100) <= 10)
	{
		std::vector<Weapon*> allowedWeapons;
		for (int i = z; i <= z + 2; i++)
		{
			allowedWeapons.insert(allowedWeapons.end(), possibleWeapons_[i].begin(), possibleWeapons_[i].end());
		}

		if (allowedWeapons.size() > 0)
		{
			return new Weapon(*allowedWeapons[Random::getRandomNumber(0, static_cast<int>(allowedWeapons.size()) - 1)]);
		}
	}

	return nullptr;
}

// ---------------------------- SHIELD GENERATOR ----------------------------

ShieldGenerator::ShieldGenerator()
{
	shieldSizeOptions_ = std::vector<std::string>();
	shieldMaterialOptions_ = std::vector<std::string>();

	std::string line;
	std::ifstream input_file("shield_options.txt");
	if (input_file) {
		while (getline(input_file, line)) {
			std::string optionName = line.substr(0, line.find(":"));
			std::string option = line.substr(line.find(":") + 1, line.length());

			if (optionName.compare("shieldSize") == 0) {
				shieldSizeOptions_.push_back(option);
			}
			else if (optionName.compare("shieldMaterial") == 0) {
				shieldMaterialOptions_.push_back(option);
			}
		}
	}
	input_file.close();

	for (size_t i = 0; i < shieldSizeOptions_.size(); i++) {
		for (size_t j = 0; j < shieldMaterialOptions_.size(); j++) {
			Shield* shield = parseLine(shieldSizeOptions_.at(i), shieldMaterialOptions_.at(j));
			possibleShields_[shield->level_].push_back(shield);
		}
	}

}

ShieldGenerator::~ShieldGenerator()
{
	for (auto it = possibleShields_.begin(); it != possibleShields_.end(); ++it)
	{
		std::for_each(it->second.begin(), it->second.end(), [](Shield* s)
		{
			delete s;
		});
	}
}

Shield* ShieldGenerator::parseLine(std::string shieldSizeOption, std::string shieldMaterialOption)
{
	// Gegevens grootte van het schild
	std::vector<std::string> shieldSizeData = std::vector<std::string>();

	split(shieldSizeOption, ',', shieldSizeData);

	std::string shieldSize = shieldSizeData.at(0);
	std::string shieldSizeLevel = shieldSizeData.at(1);
	std::string shieldSizeAttack = shieldSizeData.at(2);

	// Gegevens materiaal van het wapen
	std::vector<std::string> shieldMaterialData = std::vector<std::string>();

	split(shieldMaterialOption, ',', shieldMaterialData);

	std::string shieldMaterialType = shieldMaterialData.at(0);
	std::string shieldMaterialLevel = shieldMaterialData.at(1);
	std::string shieldMaterialAttack = shieldMaterialData.at(2);

	// Bepaal de benodigde gegevens
	int totalLevel = std::stoi(shieldSizeLevel) + std::stoi(shieldMaterialLevel);
	int totalAttack = std::stoi(shieldSizeAttack) + std::stoi(shieldMaterialAttack);

	return new Shield(shieldSize, shieldMaterialType, totalLevel, totalAttack);
}

Shield* ShieldGenerator::createShield(int z)
{
	//TODO: hoeveel kans willen we dat een kamer een schild heeft?
	if (Random::getRandomNumber(1, 100) <= 20)
	{
		std::vector<Shield*> allowedShields;
		for (int i = z; i <= z + 2; i++)
		{
			allowedShields.insert(allowedShields.end(), possibleShields_[i].begin(), possibleShields_[i].end());
		}

		if (allowedShields.size() > 0)
		{
			return new Shield(*allowedShields[Random::getRandomNumber(0, static_cast<int>(allowedShields.size()) - 1)]);
		}
	}

	return nullptr;
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
	weaponGenerator_ = new WeaponGenerator();
	shieldGenerator_ = new ShieldGenerator();
	trapGenerator_ = new TrapGenerator(enemyGenerator_);
}

RoomGenerator::~RoomGenerator()
{
	delete trapGenerator_;
	delete weaponGenerator_;
	delete shieldGenerator_;
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
		result->addEnemy(enemyGenerator_->createBoss(z));
		break;
	case Room::ROOM_TYPE::StartLocation:
		result = new Room(createType, generateDescription(), z);
		break;
	case Room::ROOM_TYPE::NormalRoom:
		result = new Room(createType, generateDescription(), z);
		addEnemies(result, z);
		addTraps(result, z);
		addWeapon(result, z);
		addShield(result, z);
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
		+ sizeOptions_.at(Random::getRandomNumber(0, static_cast<int>(sizeOptions_.size()) - 1)) + " "
		+ floorOptions_.at(Random::getRandomNumber(0, static_cast<int>(floorOptions_.size()) - 1)) + " "
		+ decorOptions_.at(Random::getRandomNumber(0, static_cast<int>(decorOptions_.size()) - 1)) + " "
		+ chestOptions_.at(Random::getRandomNumber(0, static_cast<int>(chestOptions_.size()) - 1)) + " "
		+ tidyOptions_.at(Random::getRandomNumber(0, static_cast<int>(tidyOptions_.size()) - 1)) + " "
		+ lightingOptions_.at(Random::getRandomNumber(0, static_cast<int>(lightingOptions_.size()) - 1)) + " "
		+ atmosphereOptions_.at(Random::getRandomNumber(0, static_cast<int>(atmosphereOptions_.size()) - 1)) + " ";
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

void RoomGenerator::addWeapon(Room* room, int z)
{
	Weapon* weapon = weaponGenerator_->createWeapon(z);

	if (weapon != nullptr)
		room->addItem(weapon);
}

void RoomGenerator::addShield(Room* room, int z)
{
	Shield* shield = shieldGenerator_->createShield(z);

	if (shield != nullptr)
		room->addItem(shield);
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
			newY < map->ySize_)
		{
			Room *otherRoom = map->rooms_[map->index(newX, newY, z)];

			bool addCorridor = Random::getRandomNumber(0, 4) == 0; // 20% kans op een extra gang. Normaal niet in maze generation, maar nodig voor handgranaat. 

			if (otherRoom == nullptr)
			{
				otherRoom = addRoom(newX, newY, z, map); // Recursieve call om een nieuwe room te genereren
				addCorridor = true;
			}

			if (addCorridor)
			{
				currentRoom->addExit(currentExit, otherRoom);
				otherRoom->addExit(oppositeExit, currentRoom);
			}
		}
	}

	return currentRoom;
}

