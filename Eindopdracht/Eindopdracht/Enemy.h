#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Weapon.h"
#include "Armour.h"
#include "Shield.h"

#include "string"

class Enemy
{
	public:

		enum ENEMY_TYPE {
			Rat,
			Wolf,
			Human,
			Zombie
		};

		enum ENEMY_SIZE {
			Normal,
			Big
		};

		Enemy();
		~Enemy();

	private:

		ENEMY_TYPE type_;
		ENEMY_SIZE size;
		bool chases;
		int level_;
		int hp_;

		Weapon weapon_;
		Armour armour_;
		Shield shield_;

		std::string description_;
};

#endif

