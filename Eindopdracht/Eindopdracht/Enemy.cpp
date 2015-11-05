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
	if (Random::getRandomNumber(0, 100) > chanceToDefend_) {
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

std::string Enemy::getType() const
{
	return type_;
}

std::string Enemy::getSize() const
{
	return size_;
}

int Enemy::getCurrentHP()
{
	return currentHP_;
}

int Enemy::getChanceToHit()
{
	return chanceToHit_;
}

int Enemy::getAttack()
{
	return attack_;
}

int Enemy::getChanceHeroEscapes()
{
	return chanceHeroEscapes_;
}

int Enemy::getXP()
{
	// Totaal niet van WoW gekopiëerd ;)
	return 45 + (5*level_);
	// L1: 50 XP
	// L2: 55 XP
	// ...
	// L9: 90 XP
	// L10: 95 XP
}

void Enemy::viewCheatInfo()
{
	std::cout << "    ";
	if (size_ != "normaal")
		std::cout << size_ << " ";
	std::cout << type_ << " (" << currentHP_ << "/" << maxHP_ << " HP)\n";
}

// Hierdoor werkt std::cout << Enemy
std::ostream& operator<<(std::ostream& os, const Enemy& enemy)
{
	if (enemy.getSize() != "normaal")
		os << enemy.getSize() << " ";
	os << enemy.getType();
	return os;
}