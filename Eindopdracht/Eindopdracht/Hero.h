#ifndef __HERO_H__
#define __HERO_H__

class Room;
class Item;
class Weapon;
class Shield;
class Map;

class Hero
{
	public:

		Hero(std::string name);
		~Hero();

		bool getHasWon();
		bool getIsDefeated();

		void getAttackedByEnemies();
		void takeDirectDamage(int damage);

		void addItem(Item* item);
		void removeItem(Item* item);

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::string fullCommand, std::vector<std::string> action);

		Room* getCurrentRoom();
		void setCurrentRoom(Room* room);

		std::vector<Item*> getItems();

		void setLevel(int level);
		void setMaxHP(int maxHP);
		void setCurrentHP(int currentHP);
		void setXP(int xp);
		void setChanceToHit(int chanceToHit);
		void setChanceToDefend(int chanceToDefend);
		void setAttack(int attack);
		void setPerception(int perception);
		int getPerception();
		void addXP(int xp);
		void levelUp();

	private:
		bool hasWon_;
		bool isDefeated_;

		std::string name_;
		int level_;
		int maxHP_;
		int currentHP_;
		int xp_;
		int chanceToHit_;
		int chanceToDefend_;
		int attack_;
		int perception_;

		Room* currentRoom_;

		std::vector<Item*> items_;
		
		Weapon* weapon_ = nullptr;
		Shield* shield_ = nullptr;

		std::vector<Weapon*> getWeapons();
		std::vector<Shield*> getShields();

		bool goToRoom(std::string direction);
		void fight();
		bool flee(std::string direction);
		void rest();
		void viewItems();
		void changeWeapon();
		void changeShield();
		void useTalisman();
		void viewCharacteristics();
		

		void save();
};

#endif

