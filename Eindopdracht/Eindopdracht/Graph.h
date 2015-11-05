#pragma once
#include "Room.h"

class Room;

struct Corridor
{
	Room* Room1;
	Room* Room2;
	int Weight;

	Corridor(Room* room1, Room* room2)
	{
		Room1 = room1;
		Room2 = room2;
		Weight = 0;
	}

	Corridor(Room* room1, Room* room2, int weight)
	{
		Room1 = room1;
		Room2 = room2;
		Weight = weight;
	}
};

struct Graph
{
	std::vector<Room*> Rooms;
	std::vector<Corridor> Corridors;

	// Maakt een ongeordende vector van gangen
	Graph(std::vector<Room*> rooms)
	{
		Rooms = rooms;

		std::for_each(rooms.begin(), rooms.end(), [this](Room* r)
		{
			// west en noord worden vanaf de andere kamer gemaakt. 
			Room* south = r->getExit("zuid");
			Room* east = r->getExit("oost");

			if (south)
				Corridors.push_back(Corridor(r, south));

			if (east)
				Corridors.push_back(Corridor(r, east));
		});
	}

	// Maakt een graph waarin de gangen en rooms op afstand (verste eerst) gesorteerd zijn.
	// Hiervoor wordt een aangepaste breadth first search gebruikt.
	Graph(Room* centerRoom)
	{
		std::queue<Room*> queue;
		std::unordered_map<Room*, int> steps;

		Rooms.push_back(centerRoom);
		queue.push(centerRoom);
		steps[centerRoom] = 0;

		int currentSteps = 0;

		while (!queue.empty()) {
			Room* currentRoom = queue.front();
			currentSteps = steps.at(currentRoom);

			queue.pop();
			currentSteps++;

			std::unordered_map<std::string, Room*> exits = currentRoom->getAllExits();

			std::for_each(exits.begin(), exits.end(), [&queue, &steps, currentRoom, currentSteps, this](std::pair<std::string, Room*> pair)
			{
				// Voeg alleen gangen naar oost of zuid toe, west en noord zijn dezelfde gangen vanaf een andere kamer.
				if (pair.first == "oost" ||
					pair.first == "zuid")
				{
					// Voeg de gangen toe aan het begin van de lijst, zo heeft KruskalMST ze nodig.
					Corridors.insert(Corridors.begin(), Corridor(currentRoom, pair.second, currentSteps));
				}

				// Als de uitgang op dezelfde verdieping is en nog niet bekenen is, 
				if (pair.first != "omlaag" &&
					pair.first != "omhoog" &&
					steps.count(pair.second) == 0) // Sneller dan rooms doorlopen, unordered_map is een hashmap.
				{
					Rooms.push_back(pair.second);
					queue.push(pair.second);
					steps[pair.second] = currentSteps;
				}
			});
		}
	}
};
