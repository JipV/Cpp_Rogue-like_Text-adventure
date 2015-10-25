#ifndef __HERO_H__
#define __HERO_H__

#include "Item.h";

#include "string";
#include "vector";

class Hero
{
	public:

		Hero();
		~Hero();

		void addItem(Item item);
		void removeItem(Item item);

		std::string getName();
		int getLevel();
		int getHP();
		int getXP();
		int getAttack();
		int getDefense();
		int getMindfulness();
		std::vector<Item> getItems();

	private:

		std::string name_;
		int level_;
		int hp_;
		int xp_;
		int attack_;
		int defense_;
		int mindfulness_;

		std::vector<Item> items_;
};

#endif

