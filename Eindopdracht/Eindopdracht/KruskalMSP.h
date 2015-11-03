#pragma once

#include "Graph.h"

class KruskalMST
{
public:
	KruskalMST(Graph graph);
	~KruskalMST();

	std::vector<Corridor> getCrucialCorridors();
	std::vector<Corridor> getNonCrucialCorridors();

private:
	std::unordered_map<Room*, Room*> parents_;

	std::vector<Corridor> crucialCorridors_;
	std::vector<Corridor> nonCrucialCorridors_;

	Room* findParent(Room* room);
	void combineTrees(Room* room1, Room* room2);
};