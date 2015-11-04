#include "stdafx.h"
#include "KruskalMSP.h"
#include "Graph.h"

KruskalMST::KruskalMST(Graph graph)
{
	crucialCorridors_ = std::vector<Corridor>();
	nonCrucialCorridors_ = std::vector<Corridor>();

	size_t requiredEdges = graph.Rooms.size() - 1;
	
	// Schud de volgorde van gangen, anders storten er alleen horizontale gangen in. "weight" van elke gang is gelijk, dus dit geeft geen problemen.
	std::random_shuffle(graph.Corridors.begin(), graph.Corridors.end());

	std::for_each(graph.Rooms.begin(), graph.Rooms.end(), [this](Room* r)
	{
		parents_[r] = r;
	});

	auto iterator = graph.Corridors.begin();

	while (crucialCorridors_.size() < requiredEdges) // Stop als we alle nodige gangen gevonden hebben
	{
		Room* parent1 = findParent(iterator->Room1);
		Room* parent2 = findParent(iterator->Room2);

		// Als de rooms dezelfde parent hebben, is er een lus
		if (parent1 == parent2)
		{
			nonCrucialCorridors_.push_back(*iterator);
		}
		else
		{
			crucialCorridors_.push_back(*iterator);
			combineTrees(parent1, parent2);
		}

		++iterator;
	}

	// Alle overgebleven gangen zijn niet cruciaal.
	nonCrucialCorridors_.insert(nonCrucialCorridors_.end(), iterator, graph.Corridors.end());
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