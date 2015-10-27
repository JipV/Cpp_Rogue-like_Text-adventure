#include "stdafx.h"
#include "Room.h"

#include "iostream"
#include "fstream"
#include "string"
#include "stdlib.h"
#include "time.h"

Room::Room()
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

	isVisited_ = true;
	type_ = NormalRoom;

	srand(time(NULL));

	description_ = "Beschrijving: ";
	description_ += sizeOptions_.at((rand() % sizeOptions_.size())) + " ";
	description_ += floorOptions_.at((rand() % floorOptions_.size())) + " ";
	description_ += decorOptions_.at((rand() % decorOptions_.size())) + " ";
	description_ += chestOptions_.at((rand() % chestOptions_.size())) + " ";
	description_ += tidyOptions_.at((rand() % tidyOptions_.size())) + " ";
	description_ += lightingOptions_.at((rand() % lightingOptions_.size())) + " ";
	description_ += atmosphereOptions_.at((rand() % atmosphereOptions_.size()));
}

Room::~Room()
{
}

void Room::addExit(std::string name, Room* room)
{
	exits_[name] = room;
}

bool Room::getIsVisited()
{
	return isVisited_;
}

void Room::setIsVisited(bool isVisited)
{
	isVisited_ = isVisited;
}

Room::ROOM_TYPE Room::getType()
{
	return type_;
}

void Room::setType(ROOM_TYPE type)
{
	type_ = type;
}

std::string Room::getDescription()
{
	return description_;
}

std::map<std::string, Room*> Room::getExits()
{
	return exits_;
}