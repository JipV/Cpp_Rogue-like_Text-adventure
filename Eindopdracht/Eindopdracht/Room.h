#ifndef __ROOM_H__
#define __ROOM_H__

#include "Room.h"
#include "Enemy.h"
#include "Item.h"
#include "Trap.h"

#include "string"
#include "map"
#include "vector"

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
		~Room();

		void showDescription();
		void showExits();

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::vector<std::string> action);

		void addExit(std::string name, Room* room);
		Room* getExit(std::string name);

		bool getIsVisited();
		void setIsVisited(bool isVisited);

		ROOM_TYPE getType();
		void setType(ROOM_TYPE type);

		std::map<std::string, Room*> getExits();

	private:

		bool isVisited_;

		ROOM_TYPE type_;

		std::vector<std::string> sizeOptions_;
		std::vector<std::string> floorOptions_;
		std::vector<std::string> decorOptions_;
		std::vector<std::string> chestOptions_;
		std::vector<std::string> tidyOptions_;
		std::vector<std::string> lightingOptions_;
		std::vector<std::string> atmosphereOptions_;

		std::string description_;

		std::map<std::string, Room*> exits_;

		std::vector<Enemy> enemies_;
		std::vector<Item> items_;
		std::vector<Trap> traps_;
};

#endif
