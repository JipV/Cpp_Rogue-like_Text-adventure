#pragma once

#include "Graph.h"

class KruskalMST
{
public:
	explicit KruskalMST(Room* middleRoom);
	explicit KruskalMST(std::vector<Room*> rooms);
	~KruskalMST();

	std::vector<Corridor> getCrucialCorridors();
	std::vector<Corridor> getNonCrucialCorridors();

private:
	std::unordered_map<Room*, Room*> parents_;

	std::vector<Corridor> crucialCorridors_;
	std::vector<Corridor> nonCrucialCorridors_;

	void findMST(Graph graph);
	Room* findParent(Room* room);
	void combineTrees(Room* room1, Room* room2);
};