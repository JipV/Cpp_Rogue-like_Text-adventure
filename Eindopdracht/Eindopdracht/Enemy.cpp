#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(std::string type, std::string size, int level, int hp, int attack, int defense, int chanceHeroEscapes)
{
	type_ = type;
	size_ = size;
	level_ = level;
	hp_ = hp;
	attack_ = attack;
	defense_ = defense;
	chanceHeroEscapes_ = chanceHeroEscapes;
}

Enemy::~Enemy()
{
}

std::string Enemy::getType() const
{
	return type_;
}

// Hierdoor werkt std::cout << Enemy
std::ostream& operator<<(std::ostream& os, const Enemy& enemy)
{
	os << enemy.getType();
	return os;
}