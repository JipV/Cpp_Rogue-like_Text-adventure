#include "stdafx.h"
#include "Room.h"
#include "Enemy.h"
#include "Trap.h"
#include "Item.h"

Room::Room(ROOM_TYPE type, std::string description) : isVisited_(true), type_(type), description_(description)
{
	enemies_ = new std::vector<Enemy*>();
	items_ = new std::vector<Item*>();
}

Room::~Room()
{
	std::for_each(enemies_->begin(), enemies_->end(), [](Enemy* e)
	{
		delete e;
	});
	delete enemies_;
	enemies_ = nullptr;

	std::for_each(items_->begin(), items_->end(), [](Item* i)
	{
		delete i;
	});
	delete items_;
	items_ = nullptr;

	delete trap_;
	trap_ = nullptr;
}

void Room::showDescription()
{
	std::cout << description_ << std::endl
		<< std::endl

		<< "Uitgangen: ";
	for (auto iterator = exits_.begin(); iterator != exits_.end(); ++iterator) 
	{
		std::cout << iterator->first;
		if (iterator != --exits_.end()) {
			std::cout << ", ";
		}
	}
	std::cout << "." << std::endl
		<< std::endl;

	if (enemies_->size() > 0)
	{
		std::cout << "Vijanden: ";
		for (auto iterator = enemies_->begin(); iterator != enemies_->end(); ++iterator)
		{
			std::cout << **iterator;
			if (iterator != --enemies_->end())
				std::cout << ", ";
		}
		std::cout << "." << std::endl
			<< std::endl;
	}

}

void Room::getActions(std::vector<std::string>* actions)
{
	actions->push_back("kijk rond");
	actions->push_back("doorzoek kamer");
}

bool Room::handleAction(std::string fullCommand, std::vector<std::string> action)
{
	if (fullCommand == "doorzoek kamer")
	{
		// TODO, kamer doorzoeken.
		return true;
	}

	if (trap_ && trap_->handleAction(fullCommand, action))
	{
		return true;
	}

	if (fullCommand == "kijk rond")
	{
		showDescription();
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

void Room::setTrap(Trap* trap)
{
	trap_ = trap;
}

bool Room::hasEnemies()
{
	return (enemies_->size() > 0);
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

std::map<std::string, Room*> Room::getAllExits()
{
	return exits_;
}