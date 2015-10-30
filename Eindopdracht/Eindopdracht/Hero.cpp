#include "stdafx.h"
#include "Hero.h"
#include "Room.h"
#include "Random.h"

Hero::Hero(std::string name)
{
	name_ = name;
	level_ = 0;
	maxHP_ = 20;
	currentHP_ = 20;
	xp_ = 0;
	chanceToHit_ = 20;
	chanceToDefend_ = 20;
	attack_ = 1;
	mindfulness_ = 2;

	items_ = std::vector<Item>();
}

Hero::~Hero()
{
	delete currentRoom_;
	currentRoom_ = nullptr;
}

bool Hero::goToRoom(std::string direction)
{
	Room* newRoom = currentRoom_->getExit(direction);

	if (newRoom != nullptr)
	{
		setCurrentRoom(newRoom);
		return true;
	}

	return false;
}

void Hero::fight()
{
	std::vector<Enemy*>* enemies = currentRoom_->getEnemies();

	/*
	// Toon gegevens vijanden
	for (int i = 0; i < enemies->size(); i++) {
		std::cout << "\nHet aantal levenspunten van " << enemies->at(i)->getType() << " is " << enemies->at(i)->getCurrentHP();
	}
	std::cout << "\n";
	*/

	// Val vijanden aan
	for (int i = 0; i < enemies->size(); i++) {
		if (Random::getRandomNumber(0, 100) <= chanceToHit_) {
			if (enemies->at(i)->getAttackedByHero(attack_)) {
				std::cout << "\nJe valt " << enemies->at(i)->getType() << " aan en doet " << attack_ << " schade.";
				if (enemies->at(i)->getIsDefeated()) {
					std::cout << "\nJe hebt " << enemies->at(i)->getType() << " verslagen.";
					currentRoom_->removeEnemy(enemies->at(i));
				}
				else {
					std::cout << "\nHet aantal levenspunten van " << enemies->at(i)->getType() << " is " << enemies->at(i)->getCurrentHP();
				}
			}
			else {
				std::cout << "\nJe valt " << enemies->at(i)->getType() << ", maar " << enemies->at(i)->getType() << " verdedigt zich, waardoor de aanval mislukt.";
			}
		}
		else {
			std::cout << "\nJe valt " << enemies->at(i)->getType() << " aan en mist.";
		}
		std::cout << "\n";
	}
	

	/*// Toon gegevens vijanden
	for (int i = 0; i < enemies->size(); i++) {
		if (enemies->at(i)->getIsDefeated()) {
			std::cout << "\nJe hebt " << enemies->at(i)->getType() << " verslagen.";
			currentRoom_->removeEnemy(enemies->at(i));
		}
		else {
			std::cout << "\nHet aantal levenspunten van " << enemies->at(i)->getType() << " is " << enemies->at(i)->getCurrentHP();
		}
	}
	std::cout << "\n";*/

	// Vijanden vallen aan
	for (int i = 0; i < enemies->size(); i++) {
	}
}

bool Hero::flee(std::string direction)
{
	// TODO: WANNEER ONTSNAPPEN NIET LUKT, HEEFT DAT GEVOLGEN (vijhanden vallen aan)

	Room* newRoom = currentRoom_->getExit(direction);

	if (newRoom != nullptr)
	{
		for (int i = 0; i < currentRoom_->getEnemies()->size(); i++) {
			if (Random::getRandomNumber(0, 100) <= currentRoom_->getEnemies()->at(i)->getChanceHeroEscapes()) {
				std::cout << "\nJe wordt tegengehouden door een vijand en kunt daardoor niet vluchten.\n";
				return true;
			}
		}
		
		std::cout << "\nHet lukt de vijanden niet om je tegen te houden, waardoor het lukt je om te vluchten.\n";
		setCurrentRoom(newRoom);

		return true;
	}

	return false;
}

void Hero::search()
{
	//ZOEK
}

void Hero::rest()
{
	//RUST UIT
}

void Hero::viewItems()
{
	//BEKIJK ITEMS
}

void Hero::addItem(Item item)
{
	items_.push_back(item);
}

void Hero::removeItem(Item item)
{
	/*for (std::vector<Item>::iterator it = items_.begin(); it != items_.end(); it++)
	{
		if (*it == item)
		{
			items_.erase(it);
			break;
		}
	}*/
}

void Hero::getActions(std::vector<std::string>* actions)
{
	if (currentRoom_->getEnemies()->size() > 0) {
		actions->push_back("vlucht [richting]");
		actions->push_back("vecht");
	}
	else {
		actions->push_back("loop [richting]");
	}

	currentRoom_->getActions(actions);
}


bool Hero::handleAction(std::vector<std::string> action)
{
	std::string command = action[0];

	if (command == "loop" && action.size() == 2)
	{
		return goToRoom(action[1]);
	}
	if (command == "vlucht" && action.size() == 2)
	{
		return flee(action[1]);
	}
	if (command == "vecht")
	{
		fight();
		return true;
	}

	return currentRoom_->handleAction(action);
}

Room* Hero::getCurrentRoom()
{
	return currentRoom_;
}

void Hero::setCurrentRoom(Room* room)
{
	currentRoom_ = room;
	currentRoom_->showDescription();
	currentRoom_->showExits();
	currentRoom_->showEnemies();
	room->setIsVisited(true);
}

std::vector<Item> Hero::getItems()
{
	return items_;
}
