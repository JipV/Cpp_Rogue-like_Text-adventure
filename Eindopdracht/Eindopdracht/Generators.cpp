#include "stdafx.h"
#include "Generators.h"
#include "Random.h"
#include "Map.h"

// ---------------------------- MAP GENERATOR ----------------------------

MapGenerator::MapGenerator()
{
	roomGenerator_ = new RoomGenerator();
}

MapGenerator::~MapGenerator()
{
	delete roomGenerator_;
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

			result->startLocation_ = roomGenerator_->createRoom(x, y);

			result->addRoom(result->startLocation_, x, y, z);
		}
		// Niet de eerste verdieping, maak een stairs up
		else
		{
			roomGenerator_->setSpecialRoom(endX, endY, Room::ROOM_TYPE::StairsUp);

			x = endX;
			y = endY;

			Room* stairsUp = roomGenerator_->createRoom(x, y);
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
		currentRoom = roomGenerator_->createRoom(x, y);
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
}

RoomGenerator::~RoomGenerator()
{
}

void RoomGenerator::setSpecialRoom(int x, int y, Room::ROOM_TYPE type)
{
	x_ = x;
	y_ = y;
	specialType_ = type;
}

Room* RoomGenerator::createRoom(int x, int y)
{
	Room::ROOM_TYPE createType = Room::ROOM_TYPE::NormalRoom;

	if (x_ == x &&
		y_ == y)
	{	// Hier willen we een speciale room
		createType = specialType_;
	}

	switch (createType)
	{
	case Room::ROOM_TYPE::StairsDown:
		return new Room(createType, "Je staat in een kamer met een trap naar beneden.");
	case Room::ROOM_TYPE::StairsUp:
		return new Room(createType, "Je staat in een kamer met een trap naar boven.");
	case Room::ROOM_TYPE::EndEnemy:
		return new Room(createType, "Je staat in een grote kamer, het lijkt wel of hier ooit iets groots leefde...");
	case Room::ROOM_TYPE::StartLocation:
	case Room::ROOM_TYPE::NormalRoom:
		return new Room(createType, generateDescription());
	default:
		return new Room(Room::ROOM_TYPE::NormalRoom, generateDescription());
	}
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