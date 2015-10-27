#include "stdafx.h"
#include "Room.h"

#include "stdlib.h"
#include "time.h"

Room::Room()
{
	sizeOptions_.push_back("Je staat in een grote kamer ");
	sizeOptions_.push_back("Je staat in een middelgrote kamer ");
	sizeOptions_.push_back("Je staat in een kleine kamer ");

	floorOptions_.push_back("met een stenen vloer ");
	floorOptions_.push_back("met een houte vloer ");
	floorOptions_.push_back("met een oude krakende vloer ");

	decorOptions_.push_back("en in het midden staat een tafel. Om de tafel staan vier stoelen. ");
	decorOptions_.push_back("en in de hoek staat een bed. ");
	decorOptions_.push_back("en tegen de muur staat een grote kast. ");
	decorOptions_.push_back(" en er staat geen meubels. ");

	chestOptions_.push_back("In de hoek staat een kist. ");
	chestOptions_.push_back("Er staan twee kisten in de kamer. ");
	chestOptions_.push_back("");

	tidyOptions_.push_back("Verder is de kamer schoon en netjes ");
	tidyOptions_.push_back("Verder is de kamer vies en rommelig ");
	tidyOptions_.push_back("Verder is de redelijk schoon en netjes ");

	lightingOptions_.push_back("en wordt verlicht met kaarsen. ");
	lightingOptions_.push_back("en wordt verlicht met fakkels. ");
	lightingOptions_.push_back("en wordt verlicht door een haardvuur. ");
	lightingOptions_.push_back("en wordt niet verlicht, waardoor het er donker is. ");

	atmosphereOptions_.push_back("De sfeer in de kamer is aangenaam.");
	atmosphereOptions_.push_back("De sfeer in de kamer is niet prettig.");

	isVisited_ = true;
	type_ = NormalRoom;

	srand(time(NULL));

	description_ = "Beschrijving: ";
	description_ += sizeOptions_.at((rand() % sizeOptions_.size()));
	description_ += floorOptions_.at((rand() % floorOptions_.size()));
	description_ += decorOptions_.at((rand() % decorOptions_.size()));
	description_ += chestOptions_.at((rand() % chestOptions_.size()));
	description_ += tidyOptions_.at((rand() % tidyOptions_.size()));
	description_ += lightingOptions_.at((rand() % lightingOptions_.size()));
	description_ += atmosphereOptions_.at((rand() % atmosphereOptions_.size()));
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