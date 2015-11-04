#include "stdafx.h"
#include "KruskalMST.h"
#include "Graph.h"

// Gangen worden op afstand (aantal stappen) gesorteerd. Verste gangen eerst.
KruskalMST::KruskalMST(Room* middleRoom)
{
	// De corridors bij deze manier van sorteren zijn gesorteerd of afstand, verste corridors eerst. 
	// Zonder verder sorteren betekend dit effectief dat de gangen rond de gegeven kamer het "zwaarste" wegen
	findMST(Graph(middleRoom));
}

// De oude methode, deze wordt momenteel niet gebruikt. 
KruskalMST::KruskalMST(std::vector<Room*> rooms)
{
	Graph graph = Graph(rooms);

	// Door de manier van sorteren bij een Graph gemaakt met een vector, zullen loops alleen door horizontale gangen gemaakt kunnen worden. 
	// Het shudden van de gangen voorkomt een kerker met veel lange verticale gangen.
	std::random_shuffle(graph.Corridors.begin(), graph.Corridors.end());

	findMST(graph);
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

void KruskalMST::findMST(Graph graph)
{
	crucialCorridors_ = std::vector<Corridor>();
	nonCrucialCorridors_ = std::vector<Corridor>();

	size_t requiredEdges = graph.Rooms.size() - 1;

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