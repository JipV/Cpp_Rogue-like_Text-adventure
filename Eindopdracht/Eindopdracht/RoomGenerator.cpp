#include "stdafx.h"
#include "RoomGenerator.h"
#include "Random.h"


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