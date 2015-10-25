#include "stdafx.h"
#include "Room.h"

Room::Room()
{
	isVisited_ = true;
	type_ = NormalRoom;
}

Room::~Room()
{
}

void Room::addExit(std::string name, Room* room)
{
	exits_[name] = room;
}

bool Room::getIsVisited()
{
	return isVisited_;
}

void Room::setIsVisited(bool isVisited)
{
	isVisited_ = isVisited;
}

Room::ROOM_TYPE Room::getType()
{
	return type_;
}

void Room::setType(ROOM_TYPE type)
{
	type_ = type;
}

std::string Room::getDescription()
{
	return description_;
}

std::map<std::string, Room*> Room::getExits()
{
	return exits_;
}