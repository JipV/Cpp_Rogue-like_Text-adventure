#ifndef __MAP_H__
#define __MAP_H__

class Room;
class Hero;

class Map
{
friend class MapGenerator; // MapGenerator mag private variabelen aanmaken. 
public:
	Map(int xSize, int ySize, int zSize);
	~Map();

	void getActions(std::vector<std::string>* actions);
	bool handleAction(std::string fullCommand, Hero* hero);

	Room* getRoom(int x, int y, int z);
	std::vector<Room*> getAllRooms(int z);
	Room* getStartLocation();

	int getSize();

	int getXSize();
	int getYSize();
	int getZSize();

private:
	int xSize_;
	int ySize_;
	int zSize_;

	Room** rooms_ = nullptr;
	Room* startLocation_ = nullptr;

	int index(int x, int y, int z);
	void addRoom(Room* room, int x, int y, int z);

	void showMap(Room* currentRoom, bool showUnvisitedRooms);
	void useCompass(Room* currentRoom, std::vector<Room*> allRooms);
	void destroyCorridors(int z);
	
};

#endif

