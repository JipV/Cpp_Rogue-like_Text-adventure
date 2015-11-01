#ifndef __ROOM_H__
#define __ROOM_H__

class Enemy;
class Item;
class Trap;
class Hero;

class Room
{
	friend class MapGenerator;
	friend class RoomGenerator;
	public:
		enum ROOM_TYPE {
			StartLocation,
			NormalRoom,
			StairsDown,
			StairsUp,
			EndEnemy
		};

		Room(ROOM_TYPE type, std::string description, int level);
		~Room();

		void showDescription();

		int getLevel();

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::string fullCommand, std::vector<std::string> action, Hero* hero);

		Room* getExit(std::string name);
		std::map<std::string, Room*> getAllExits();
		
		void addEnemy(Enemy* enemy);
		void removeEnemy(Enemy* enemy);
		bool hasEnemies();
		std::vector<Enemy*> getEnemies();

		bool getIsVisited();
		void setIsVisited(bool isVisited);

		ROOM_TYPE getType();
	private:

		bool isVisited_;
		bool searched_;
		int level_;

		ROOM_TYPE type_;

		std::string description_;

		std::map<std::string, Room*> exits_;

		std::vector<Enemy*> enemies_;
		std::vector<Item*> items_;
		Trap* trap_ = nullptr;

		void addExit(std::string name, Room* room);
		void addItem(Item* item);
		void setTrap(Trap* trap);
		void searchRoom(Hero* hero);
};

#endif
