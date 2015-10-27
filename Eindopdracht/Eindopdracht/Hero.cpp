#include "stdafx.h"
#include "Hero.h"

Hero::Hero(std::string name)
{
	name_ = name;
	level_ = 0;
	hp_ = 0;
	xp_ = 0;
	attack_ = 0;
	defense_ = 0;
	mindfulness_ = 0;

	items_ = std::vector<Item>();
}

Hero::~Hero()
{
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

Room* Hero::getCurrentRoom()
{
	return currentRoom_;
}

void Hero::setCurrentRoom(Room* room)
{
	currentRoom_ = room;
}

std::vector<Item> Hero::getItems()
{
	return items_;
}
