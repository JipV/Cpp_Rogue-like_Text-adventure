#pragma once
#include "Room.h"

class RoomGenerator
{
public:
	RoomGenerator();
	~RoomGenerator();

	void setSpecialRoom(int x, int y, Room::ROOM_TYPE type);
	Room* createRoom(int x, int y);

private:
	int x_, y_;
	Room::ROOM_TYPE specialType_;

	std::vector<std::string> sizeOptions_;
	std::vector<std::string> floorOptions_;
	std::vector<std::string> decorOptions_;
	std::vector<std::string> chestOptions_;
	std::vector<std::string> tidyOptions_;
	std::vector<std::string> lightingOptions_;
	std::vector<std::string> atmosphereOptions_;

	std::string generateDescription();
};

