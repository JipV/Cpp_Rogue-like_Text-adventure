#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Weapon.h"
#include "Armour.h"
#include "Shield.h"

#include "string"

class Enemy
{
	public:

		Enemy(std::string type, std::string size, int level, int hp, int attack, int defense, int chanceHeroEscapes);
		~Enemy();

		std::string getType();

	private:

		std::string type_;
		std::string size_;
		int level_;
		int hp_;
		int attack_;
		int defense_;
		int chanceHeroEscapes_;

		Weapon weapon_;
		Armour armour_;
		Shield shield_;

		std::string description_;
};

#endif

