#include "stdafx.h"
#include "Hero.h"
#include "Room.h"
#include "Random.h"

Hero::Hero(std::string name)
{
	name_ = name;
	level_ = 0;
	hp_ = 20;
	xp_ = 0;
	attack_ = 1;
	defense_ = 1;
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

	// Toon gegevens vijanden
	for (int i = 0; i < enemies->size(); i++) {
		std::cout << "\nHet aantal levenspunten van " << enemies->at(i)->getType() << " is " << enemies->at(i)->getCurrentHP();
	}
	std::cout << "\n";

	// Val vijanden aan
	for (int i = 0; i < enemies->size(); i++) {
		
		//int number = Random::getRand	omNumber(0, 10);

		std::cout << "\nJe valt " << enemies->at(i)->getType() << " aan en doet " << attack_ << " schade";
		enemies->at(i)->getAttackedByHero(attack_);
	}
	std::cout << "\n"; 

	enemies = currentRoom_->getEnemies();

	// Toon gegevens vijanden
	for (int i = 0; i < enemies->size(); i++) {
		if (enemies->at(i)->getIsDefeated()) {
			std::cout << "\nJe hebt " << enemies->at(i)->getType() << " verslagen.";
			currentRoom_->removeEnemy(enemies->at(i));
		}
		else {
			std::cout << "\nHet aantal levenspunten van " << enemies->at(i)->getType() << " is " << enemies->at(i)->getCurrentHP();
		}
	}
	std::cout << "\n";

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

std::string Hero::getName()
{
	return name_;
}

int Hero::getLevel()
{
	return level_;
}

void Hero::setLevel(int level)
{
	level_ = level;
}

int Hero::getHP()
{
	return hp_;
}

void Hero::setHP(int hp)
{
	hp_ = hp;
}

int Hero::getXP()
{
	return xp_;
}

void Hero::setXP(int xp)
{
	xp_ = xp;
}

int Hero::getAttack()
{
	return attack_;
}

void Hero::setAttack(int attack)
{
	attack_ = attack;
}

int Hero::getDefense()
{
	return defense_;
}

void Hero::setDefense(int defense)
{
	defense_ = defense;
}

int Hero::getMindfulness()
{
	return mindfulness_;
}

void Hero::setMindfulness(int mindfulness)
{
	mindfulness_ = mindfulness;
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
