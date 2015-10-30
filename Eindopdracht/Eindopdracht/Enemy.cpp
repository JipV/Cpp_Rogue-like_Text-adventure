#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(std::string type, std::string size, int level, int maxHP, int chanceToHit, int chanceToDefend, int attack, int chanceHeroEscapes)
{
	isDefeated_ = false;

	type_ = type;
	size_ = size;

	level_ = level;
	maxHP_ = maxHP;
	currentHP_ = maxHP;
	chanceToHit_ = chanceToHit;
	chanceToDefend_ = chanceToDefend;
	attack_ = attack;
	chanceHeroEscapes_ = chanceHeroEscapes;
}

Enemy::~Enemy()
{
}

void Enemy::getAttackedByHero(int damage)
{
	currentHP_ -= damage;
	if (currentHP_ <= 0) {
		isDefeated_ = true;
	}
}

bool Enemy::getIsDefeated()
{
	return isDefeated_;
}

std::string Enemy::getType()
{
	return type_;
}

int Enemy::getCurrentHP()
{
	return currentHP_;
}

int Enemy::getChanceHeroEscapes()
{
	return chanceHeroEscapes_;
}