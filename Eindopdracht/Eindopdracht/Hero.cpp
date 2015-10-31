#include "stdafx.h"
#include "Hero.h"
#include "Room.h"
#include "Random.h"
#include "Enemy.h"

// ReSharper disable once CppPossiblyUninitializedMember
Hero::Hero(std::string name) : 
	name_(name), 
	level_(0), 
	maxHP_(20), 
	currentHP_(20), 
	xp_(0), 
	chanceToHit_(20), 
	chanceToDefend_(20), 
	attack_(1),  
	mindfulness_(2)
{
	items_ = new std::vector<Item*>();
}

Hero::~Hero()
{
	currentRoom_ = nullptr; // Rooms horen bij Map, die zal ze verwijderen

	for (size_t i = 0; i < items_->size(); i++)
		delete items_->at(i);
	delete items_;
	items_ = nullptr;
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

	// Val vijanden aan
	for (size_t i = 0; i < enemies->size(); i++) {
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
	
	std::cout << "\n";

	// Vijanden vallen aan
	getAttackedByEnemies();
}

bool Hero::flee(std::string direction)
{
	Room* newRoom = currentRoom_->getExit(direction);

	if (newRoom) // nullptr == false
	{
		for (size_t i = 0; i < currentRoom_->getEnemies()->size(); i++) {
			if (Random::getRandomNumber(0, 100) <= currentRoom_->getEnemies()->at(i)->getChanceHeroEscapes()) {
				std::cout << "\nJe wordt tegengehouden door een vijand en kunt daardoor niet vluchten.\n";

				// Vijanden vallen aan
				getAttackedByEnemies();

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

void Hero::getAttackedByEnemies()
{
	// TODO: GAME OVER

	for (size_t i = 0; i < currentRoom_->getEnemies()->size(); i++) {
		if (!isDefeated_) {
			Enemy* enemy = currentRoom_->getEnemies()->at(i);
			std::cout << "\nJe wordt aangevallen door " << enemy->getType();

			if (Random::getRandomNumber(0, 100) <= enemy->getChanceToHit()) {
				if (Random::getRandomNumber(0, 100) <= chanceToDefend_) {
					std::cout << ", maar je verdedigt je, waardoor de aanval mislukt.\n";
				}
				else {
					currentHP_ -= enemy->getAttack();
					if (currentHP_ <= 0) {
						currentHP_ = 0;
						isDefeated_ = true;
					}
					std::cout << " en verliest daardoor " << enemy->getAttack();
					if (enemy->getAttack() == 1) {
						std::cout << " levenspunt.\n";
					}
					else {
						std::cout << " levenspunten.\n";
					}
				}
			}
			else {
				std::cout << ", maar hij mist.\n";
			}
		}
	}
	std::cout << "\nJe hebt nog " << currentHP_ << " levenspunten over.\n";
}

void Hero::takeDirectDamage(int damage)
{
	currentHP_ -= damage;

	if (currentHP_ <= 0)
	{
		std::cout << "Je bent dood!" << std::endl;

		currentHP_ = 0;
		isDefeated_ = true;
	}

	std::cout << "\nJe hebt nog " << currentHP_ << " levenspunten over.\n";
}

void Hero::addItem(Item* item)
{
	items_->push_back(item);
}

void Hero::removeItem(Item* item)
{
	items_->erase(std::remove(items_->begin(), items_->end(), item), items_->end());
}

void Hero::getActions(std::vector<std::string>* actions)
{
	if (currentRoom_->hasEnemies())
	{
		actions->push_back("vlucht [richting]");
		actions->push_back("vecht");
	}
	else 
	{
		actions->push_back("loop [richting]");
	}

	currentRoom_->getActions(actions);
}

bool Hero::handleAction(std::string fullCommand, std::vector<std::string> action)
{
	std::string command = action[0];

	// Deze moet voor hero handle actions, gezien de acties onderbroken kunnen worden door traps
	if (currentRoom_->handleAction(fullCommand, action, this))
		return true;

	if (command == "loop" && action.size() == 2)
	{
		if (currentRoom_->hasEnemies())
			return false;

		return goToRoom(action[1]);
	}
	if (command == "vlucht" && action.size() == 2)
	{
		if (currentRoom_->hasEnemies())
			return flee(action[1]);

		return false;
	}
	if (command == "vecht")
	{
		fight();
		return true;
	}

	return false;
}

Room* Hero::getCurrentRoom()
{
	return currentRoom_;
}

void Hero::setCurrentRoom(Room* room)
{
	currentRoom_ = room;
	currentRoom_->showDescription();
	room->setIsVisited(true);
}

std::vector<Item*>* Hero::getItems()
{
	return items_;
}
