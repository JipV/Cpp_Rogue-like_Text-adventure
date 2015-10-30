#include "stdafx.h"
#include "Enemy.h"
#include "Random.h"

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

bool Enemy::getAttackedByHero(int damage)
{
	if (Random::getRandomNumber(0, 100) <= chanceToDefend_) {
		currentHP_ -= damage;
		if (currentHP_ <= 0) {
			isDefeated_ = true;
		}
		return true;
	}
	return false;
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