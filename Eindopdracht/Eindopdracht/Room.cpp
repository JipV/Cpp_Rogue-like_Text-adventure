#include "stdafx.h"
#include "Room.h"

Room::Room(ROOM_TYPE type, std::string description) : isVisited_(true), type_(type), description_(description)
{
	enemies_ = new std::vector<Enemy*>();
	items_ = new std::vector<Item*>();
	traps_ = new std::vector<Trap*>();
}

Room::~Room()
{
	for (int i = 0; i < enemies_->size(); i++)
		delete enemies_->at(i);
	delete enemies_;
	enemies_ = nullptr;

	for (int i = 0; i < items_->size(); i++)
		delete items_->at(i);
	delete items_;
	items_ = nullptr;

	for (int i = 0; i < traps_->size(); i++)
		delete traps_->at(i);
	delete traps_;
	traps_ = nullptr;
}

void Room::showDescription()
{
	std::cout << "\n" << description_ << "\n";
}

void Room::showExits()
{
	std::cout << "\nUitgangen: ";
	typedef std::map<std::string, Room*>::iterator it_type;
	for (it_type iterator = exits_.begin(); iterator != exits_.end(); ++iterator) {
		std::cout << iterator->first;
		if (iterator != --exits_.end()) {
			std::cout << ", ";
		}
	}
	std::cout << "\n";
}


void Room::showEnemies()
{
	if (enemies_->size() > 0) {
		std::cout << "\nVijhanden: ";
		for (size_t i = 0; i < enemies_->size(); i++) {
			std::cout << enemies_->at(i)->getType();
			if (i != enemies_->size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << "\n";
	}
}

void Room::getActions(std::vector<std::string>* actions)
{
	actions->push_back("kijk rond");
}

bool Room::handleAction(std::vector<std::string> action)
{
	std::string command = action[0];

	if (command == "kijk" &&
		action.size() == 2 &&
		action[1] == "rond")
	{
		showDescription();
		showExits();
		return true;
	}
	
	return false;
}

void Room::addExit(std::string name, Room* room)
{
	exits_[name] = room;
}


void Room::addEnemy(Enemy* enemy)
{
	enemies_->push_back(enemy);
}

void Room::removeEnemy(Enemy* enemy)
{
	enemies_->erase(std::remove(enemies_->begin(), enemies_->end(), enemy), enemies_->end());
}

Room* Room::getExit(std::string name)
{
	if (exits_.count(name))
	{
		return exits_[name];
	}
	return nullptr;
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

std::vector<Enemy*>* Room::getEnemies()
{
	return enemies_;
}