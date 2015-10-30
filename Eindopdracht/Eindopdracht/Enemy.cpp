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

std::string Enemy::getType()
{
	return type_;
}

int Enemy::getChanceHeroEscapes()
{
	return chanceHeroEscapes_;
}