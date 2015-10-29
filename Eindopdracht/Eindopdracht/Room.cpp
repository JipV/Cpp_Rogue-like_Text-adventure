#include "stdafx.h"
#include "Room.h"

#include "iostream"
#include "fstream"
#include "string"

#include "Random.h"

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

	description_ = "Beschrijving: ";
	description_ += sizeOptions_.at(Random::getRandomNumber(0, sizeOptions_.size() - 1)) + " ";
	description_ += floorOptions_.at(Random::getRandomNumber(0, floorOptions_.size() - 1)) + " ";
	description_ += decorOptions_.at(Random::getRandomNumber(0, decorOptions_.size() - 1)) + " ";
	description_ += chestOptions_.at(Random::getRandomNumber(0, chestOptions_.size() - 1)) + " ";
	description_ += tidyOptions_.at(Random::getRandomNumber(0, tidyOptions_.size() - 1)) + " ";
	description_ += lightingOptions_.at(Random::getRandomNumber(0, lightingOptions_.size() - 1)) + " ";
	description_ += atmosphereOptions_.at(Random::getRandomNumber(0, atmosphereOptions_.size() - 1)) + " ";
}

Room::~Room()
{
}

void Room::showDescription()
{
	std::cout << "\n" << description_ << "\n";
}

void Room::showExits()
{
	std::cout << "\nUitgangen: ";
	typedef std::map<std::string, Room*>::iterator it_type;
	for (it_type iterator = exits_.begin(); iterator != exits_.end(); iterator++) {
		std::cout << iterator->first;
		if (iterator != --exits_.end()) {
			std::cout << ", ";
		}
	}
	std::cout << "\n";
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

std::map<std::string, Room*> Room::getExits()
{
	return exits_;
}