#ifndef __HERO_H__
#define __HERO_H__

#include "Room.h"
#include "Weapon.h"
#include "Armour.h"
#include "Shield.h"
#include "Item.h"

#include "string"
#include "vector"

class Hero
{
	public:

		Hero(std::string name);
		~Hero();

		bool goToRoom(std::string direction);
		void fight();
		bool flee(std::string direction);
		void search();
		void rest();
		void viewItems();

		void addItem(Item item);
		void removeItem(Item item);

		std::string getName();
		
		int getLevel();
		void setLevel(int level);
		
		int getHP();
		void setHP(int hp);
		
		int getXP();
		void setXP(int xp);

		int getAttack();
		void setAttack(int attack);

		int getDefense();
		void setDefense(int defense);

		int getMindfulness();
		void setMindfulness(int mindfulness);

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::vector<std::string> action);

		Room* getCurrentRoom();
		void setCurrentRoom(Room* room);

		std::vector<Item> getItems();

	private:

		std::string name_;
		int level_;
		int hp_;
		int xp_;
		int attack_;
		int defense_;
		int mindfulness_;

		Room* currentRoom_;

		Weapon weapon_;
		Armour armour_;
		Shield shield_;

		std::vector<Item> items_;
};

#endif

