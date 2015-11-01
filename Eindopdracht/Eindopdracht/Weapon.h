#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"

class Weapon : public Item
{
	public:

		Weapon(std::string type, int level, int attack);
		~Weapon();

		std::string getType();
		int getAttack();

	private:

		std::string type_;
		int level_;
		int attack_;
};

#endif
