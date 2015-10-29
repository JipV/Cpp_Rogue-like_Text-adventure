#ifndef __MAP_H__
#define __MAP_H__

class Room;
class RoomGenerator;
class Hero;

class Map
{
public:
	Map(int xSize, int ySize, int zSize);
	~Map();

	void createMap();
	void showMap(Room* currentRoom);

	void getActions(std::vector<std::string>* actions);
	bool handleAction(std::vector<std::string> action, Hero* hero);

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

	Room** rooms_ = nullptr;
	Room* startLocation_ = nullptr;

	RoomGenerator* roomGenerator_;

	int index(int x, int y, int z);
	void generateRoom(int x, int y, int z);
	bool isNull(int x, int y, int z);
};

#endif

