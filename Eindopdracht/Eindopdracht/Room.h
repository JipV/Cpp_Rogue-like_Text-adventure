#ifndef __ROOM_H__
#define __ROOM_H__

#include "Enemy.h"
#include "Item.h"
#include "Trap.h"

class Room
{
	public:

		enum ROOM_TYPE {
			StartLocation,
			NormalRoom,
			StairsDown,
			StairsUp,
			EndEnemy
		};

		Room();
		Room(ROOM_TYPE type, std::string description);
		~Room();

		void showDescription();
		void showExits();
		void showEnemies();

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::vector<std::string> action);

		void addExit(std::string name, Room* room);
		Room* getExit(std::string name);
		
		void addEnemy(Enemy* enemy);
		bool hasEnemies();

		bool getIsVisited();
		void setIsVisited(bool isVisited);

		ROOM_TYPE getType();
		void setType(ROOM_TYPE type);

		std::map<std::string, Room*> getExits();

	private:

		bool isVisited_;

		ROOM_TYPE type_;

		std::string description_;

		std::map<std::string, Room*> exits_;

		std::vector<Enemy*>* enemies_ = nullptr;
		std::vector<Item*>* items_ = nullptr;
		std::vector<Trap*>* traps_ = nullptr;
};

#endif
