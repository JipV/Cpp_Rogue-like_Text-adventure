#include "stdafx.h"
#include "Hero.h"

Hero::Hero()
{
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

int Hero::getHP()
{
	return hp_;
}

int Hero::getXP()
{
	return xp_;
}

int Hero::getAttack()
{
	return attack_;
}

int Hero::getDefense()
{
	return defense_;
}

int Hero::getMindfulness()
{
	return mindfulness_;
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
