#pragma once
#include "Room.h"

class Room;

struct Corridor
{
	Room* Room1;
	Room* Room2;

	Corridor(Room* room1, Room* room2)
	{
		Room1 = room1;
		Room2 = room2;
	}
};

struct Graph
{
	std::vector<Room*> Rooms;
	std::vector<Corridor> Corridors;

	Graph(std::vector<Room*> rooms)
	{
		Rooms = rooms;
		Corridors = std::vector<Corridor>();

		std::for_each(rooms.begin(), rooms.end(), [this](Room* r)
		{
			Room* south = r->getExit("zuid");
			Room* east = r->getExit("oost");

			// west en noord worden vanaf de andere kamer gemaakt. 

			if (south)
				Corridors.push_back(Corridor(r, south));

			if (east)
				Corridors.push_back(Corridor(r, east));
		});
	}
};
