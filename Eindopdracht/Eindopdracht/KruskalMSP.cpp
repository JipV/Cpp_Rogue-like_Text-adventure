#include "stdafx.h"
#include "KruskalMSP.h"
#include "Graph.h"

KruskalMST::KruskalMST(Graph graph)
{
	crucialCorridors_ = std::vector<Corridor>();
	nonCrucialCorridors_ = std::vector<Corridor>();

	size_t requiredEdges = graph.Rooms.size() - 1;
	
	// De graph is unweighted, sorteren is niet nodig. 

	std::unordered_map<Room*, Room*> roomsMap;

	std::for_each(graph.Rooms.begin(), graph.Rooms.end(), [&roomsMap](Room* r)
	{
		roomsMap[r] = r;
	});
	parents_ = roomsMap;

	auto iterator = graph.Corridors.begin();

	while (crucialCorridors_.size() < requiredEdges) // Stop als we alle nodige gangen gevonden hebben
	{
		Room* parent1 = findParent(iterator->Room1);
		Room* parent2 = findParent(iterator->Room2);

		// Als de rooms dezelfde parent hebben, is er een lus
		if (parent1 != parent2)
		{
			crucialCorridors_.push_back(*iterator);
			combineTrees(parent1, parent2);
		}
		else
		{
			nonCrucialCorridors_.push_back(*iterator);
		}

		++iterator;
	}

	// Alle overgebleven gangen zijn niet cruciaal.
	//while (iterator != graph.Corridors.end())
	//{
	//	nonCrucialCorridors_.push_back(*iterator);
	//	++iterator;
	//}
	nonCrucialCorridors_.insert(nonCrucialCorridors_.end(), iterator, graph.Corridors.end());

	std::cout << "KruskalMSP done. Rooms: " << graph.Rooms.size() << ", Corridors: " << graph.Corridors.size() << ", crucial: " << crucialCorridors_.size() << ", noncrucial: " << nonCrucialCorridors_.size() << std::endl;
}

KruskalMST::~KruskalMST()
{
}

std::vector<Corridor> KruskalMST::getCrucialCorridors()
{
	return crucialCorridors_;
}

std::vector<Corridor> KruskalMST::getNonCrucialCorridors()
{
	return nonCrucialCorridors_;
}

Room* KruskalMST::findParent(Room* room)
{
	if (parents_[room] != room)
		parents_[room] = findParent(parents_[room]);

	return parents_[room];
}

void KruskalMST::combineTrees(Room* room1, Room* room2)
{
	Room* parent1 = findParent(room1);
	Room* parent2 = findParent(room2);

	parents_[parent1] = parent2;
}