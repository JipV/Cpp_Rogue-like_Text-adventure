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

		enum ROOM_SIZE {
			Large,
			Medium,
			Small
		};

		enum ROOM_DECOR {
			TableAndChairs,
			Bed,
			Empty
		};

		enum ROOM_LIGHTING {
			Candle,
			Torch,
			Fire,
			None
		};

		Room();
		~Room();

		void addExit(std::string name, Room* room);

		bool getIsVisited();
		void setIsVisited(bool isVisited);

		ROOM_TYPE getType();
		void setType(ROOM_TYPE type);

		std::string getDescription();
		std::map<std::string, Room*> getExits();

	private:

		bool isVisited_;

		ROOM_TYPE type_;

		ROOM_SIZE size_;
		bool isTidy_;
		ROOM_DECOR decor_;
		ROOM_LIGHTING lighting;

		std::string description_;

		std::map<std::string, Room*> exits_;

		std::vector<Enemy> enemies_;
		std::vector<Item> items_;
		std::vector<Trap> traps_;
};

#endif
