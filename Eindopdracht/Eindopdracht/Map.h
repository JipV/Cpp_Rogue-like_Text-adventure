#ifndef __MAP_H__
#define __MAP_H__

#include "Room.h"

class Map
{
	public:

		Map();
		Map(int xSize, int ySize, int zSize);
		~Map();
		
		void createMap();
		void showMap(Room* currentRoom);

		void addRoom(Room* room, int x, int y, int z);
		Room* getRoom(int x, int y, int z);

		Room* getStartLocation();

		int getSize();

		int getXSize();
		int getYSize();
		int getZSize();
	
	private:

		int xSize_;
		int ySize_;
		int zSize_;

		Room* rooms_ = nullptr;

		int index(int x, int y, int z);
};

#endif

