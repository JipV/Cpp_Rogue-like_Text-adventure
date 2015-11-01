#include "stdafx.h"
#include "Room.h"
#include "Enemy.h"
#include "Trap.h"
#include "Item.h"
#include "Hero.h"

Room::Room(ROOM_TYPE type, std::string description, int level) : 
	isVisited_(false), 
	searched_(false),
	level_(level), 
	type_(type), 
	description_(description)
{
	enemies_ = std::vector<Enemy*>();
	items_ = std::vector<Item*>();
}

Room::~Room()
{
	std::for_each(enemies_.begin(), enemies_.end(), [](Enemy* e)
	{
		delete e;
	});

	std::for_each(items_.begin(), items_.end(), [](Item* i)
	{
		delete i;
	});

	delete trap_;
	trap_ = nullptr;
}

void Room::showDescription()
{
	std::cout << description_;
	
	if (trap_)
		trap_->showRoomDescription();

	std:: cout << std::endl << std::endl
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

	if (enemies_.size() > 0)
	{
		std::cout << "Vijanden: ";
		for (auto iterator = enemies_.begin(); iterator != enemies_.end(); ++iterator)
		{
			std::cout << **iterator;
			if (iterator != --enemies_.end())
				std::cout << ", ";
		}
		std::cout << "." << std::endl
			<< std::endl;
	}
}

int Room::getLevel()
{
	return level_;
}

void Room::getActions(std::vector<std::string>* actions)
{
	actions->push_back("kijk rond");
	
	if (!searched_)
		actions->push_back("doorzoek kamer");
}

bool Room::handleAction(std::string fullCommand, std::vector<std::string> action, Hero* hero)
{
	// Deze commandos kunnen niet onderbroken worden door vallen

	if (fullCommand == "doorzoek kamer")
	{
		searchRoom(hero);
		return true;
	}
	if (fullCommand == "kijk rond")
	{
		showDescription();
		return true;
	}

	// Kijk of er een val af gaat
	if (enemies_.empty() && trap_ && trap_->handleAction(fullCommand, action, hero, this))
	{
		return true;
	}

	// Hier komen commandos die onderbroken kunnen worden door vallen.
	
	return false;
}

void Room::addExit(std::string name, Room* room)
{
	exits_[name] = room;
}


void Room::addEnemy(Enemy* enemy)
{
	enemies_.push_back(enemy);
}


void Room::removeEnemy(Enemy* enemy)
{
	enemies_.erase(std::remove(enemies_.begin(), enemies_.end(), enemy), enemies_.end());
}

void Room::addItem(Item* item)
{
	items_.push_back(item);
}

void Room::setTrap(Trap* trap)
{
	trap_ = trap;
}

void Room::searchRoom(Hero* hero)
{
	if (searched_)
		return;

	searched_ = true;

	std::cout << "Je doorzoekt de kamer... " << std::endl;

	bool foundSomething = false;

	if (!items_.empty())
	{
		foundSomething = true;

		std::for_each(items_.begin(), items_.end(), [hero](Item* i)
		{
			std::cout << "Je hebt " << *i << " gevonden!" << std::endl;
			hero->addItem(i);
		});
	}

	if (trap_)
		foundSomething = trap_->searchRoom() || foundSomething;

	// TODO: Andere dingen doorzoeken (zoals bijvoorbeeld kisten)

	if (!foundSomething)
		std::cout << "Maar je vind niks." << std::endl;
}

bool Room::hasEnemies()
{
	return (enemies_.size() > 0);
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

std::vector<Enemy*> Room::getEnemies()
{
	return enemies_;
}