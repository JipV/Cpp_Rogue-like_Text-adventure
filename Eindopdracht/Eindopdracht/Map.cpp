#include "stdafx.h"
#include "Map.h"
#include "Room.h"
#include "Hero.h"
#include "KruskalMSP.h"
#include "Graph.h"
#include "Enemy.h"
#include "Trap.h"

Map::Map(int xSize, int ySize, int zSize)
	: xSize_{ xSize }, ySize_{ ySize }, zSize_{ zSize }, rooms_{ nullptr }
{
	rooms_ = new Room*[getSize()]();
}

Map::~Map()
{
	// rooms_ is een array van pointers, eerst moet dus alle inhoud verwijderd worden. delete[] rooms_; is niet genoeg.
	for (int i = 0; i <= index(xSize_ - 1, ySize_ - 1, zSize_ - 1); i++)
		delete rooms_[i];

	delete[] rooms_;
	rooms_ = nullptr;
	startLocation_ = nullptr; // Deze wordt al verwijderd samen met alle rooms
}

void Map::showMap(Room* currentRoom, bool showUnvisitedRooms)
{
	std::cout << "\nKerker kaart: \n";

	int z = currentRoom->getLevel();

	for (int y = 0; y < ySize_; y++)
	{
		// Teken kamers + gangen van west naar oost
		for (int x = 0; x < xSize_; x++)
		{
			Room* room = getRoom(x, y, z);

			if (room == currentRoom) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			}

			if (room->getIsVisited() || showUnvisitedRooms)
			{
				// Teken kamer
				switch (room->getType())
				{
				case Room::ROOM_TYPE::StartLocation:
					std::cout << 'S';
					break;
				case Room::ROOM_TYPE::NormalRoom:
					std::cout << 'N';
					break;
				case Room::ROOM_TYPE::StairsDown:
					std::cout << 'L';
					break;
				case Room::ROOM_TYPE::StairsUp:
					std::cout << 'H';
					break;
				case Room::ROOM_TYPE::EndEnemy:
					std::cout << 'E';
					break;
				}

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

				// Teken uitgang van west naar oost
				if (room->getAllExits().count("oost"))
					std::cout << '-';
				else
					std::cout << ' ';
			}
			else
			{
				std::cout << ".";
					
				if (x < xSize_ - 1)
				{
					Room* eastRoom = getRoom(x + 1, y, z);
					if (eastRoom->getIsVisited() &&
						eastRoom->getAllExits().count("west"))
						std::cout << '-';
					else
						std::cout << ' ';
				}
				else
					std::cout << ' ';
			}
		}
		std::cout << "\n";

		// Teken gangen van noord naar zuid
		for (int x = 0; x < xSize_; x++)
		{
			Room* room = getRoom(x, y, z);
			if (room->getIsVisited() || showUnvisitedRooms)
			{
				if (room->getAllExits().count("zuid"))
					std::cout << "| ";
				else
					std::cout << "  ";
			}
			else
			{
				if (y < ySize_-1)
				{
					Room* southRoom = getRoom(x, y + 1, z);
					if (southRoom->getIsVisited() &&
						southRoom->getAllExits().count("noord"))
						std::cout << "| ";
					else
						std::cout << "  ";
				}
				else
					std::cout << "  ";
			}
		}
		std::cout << "\n";
	}

	std::cout << "Legenda: \n";
	std::cout << "|- : Gangen \n";
	std::cout << "S  : Start locatie \n";
	std::cout << "E  : Eind vijhand \n";
	std::cout << "N  : Normale ruimte \n";
	std::cout << "L  : Trap omlaag \n";
	std::cout << "H  : Trap omhoog \n";
	std::cout << ".  : Niet bezocht \n";
}


void Map::useCompass(Room* currentRoom)
{
	struct Vertex {
		Room* vertex;
		std::map<std::string, Room*> edges;
		double distance;
		Vertex* previousVertex;
		bool done;
	};

	std::vector<Vertex*> vertices = std::vector<Vertex*>();
	std::queue<Vertex*> queue = std::queue<Vertex*>();

	// Maak vertices aan
	for (int i = 0; i < zSize_; i++) {
		std::vector<Room*> rooms = getAllRooms(i);
		std::for_each(rooms.begin(), rooms.end(), [currentRoom, &vertices, &queue](Room* room)
		{
			Vertex* newVertex = new Vertex();
			newVertex->vertex = room;
			newVertex->edges = room->getAllExits();
			newVertex->distance = std::numeric_limits<double>::infinity();
			newVertex->previousVertex = nullptr;
			newVertex->done = false;

			// Zet de afstand van de eerste vertex op 0 en van de rest op oneindig
			if (room == currentRoom) {
				newVertex->distance = 0;
				queue.push(newVertex); // Begin bij de vertex met de laagste afstand
			}

			vertices.push_back(newVertex);
		});
	}

	/*std::cout << "\nAlle vertices:\n";
	std::for_each(vertices.begin(), vertices.end(), [](Vertex* v)
	{
	std::cout << v->vertex << " weight: " << v->distance << "\n";
	});*/

	// Bepaal afstanden
	while (!queue.empty()) {
		Vertex* vertex = queue.front();
		queue.pop();

		std::for_each(vertex->edges.begin(), vertex->edges.end(), [&queue, vertices, vertex](std::pair<std::string, Room*> exitPair)
		{
			// Haal de vertex uit de vector, die hoort bij de gang
			auto vertex2 = std::find_if(vertices.begin(), vertices.end(), [exitPair](Vertex* vertex2) {return vertex2->vertex == exitPair.second; });
			if (vertex2 != vertices.end())
			{
				int weigthEdge = exitPair.second->getTotalHPEnemies();
				if (exitPair.second->getTrap() != nullptr) {
					weigthEdge += exitPair.second->getTrap()->getLevel();
				}
				int distance = vertex->distance + weigthEdge;

				// Bepaal de kleinste afstand
				if (distance < (*vertex2)->distance) {
					(*vertex2)->distance = distance;
					(*vertex2)->previousVertex = vertex;
				}

				// Voeg eventueel vertex toe aan de queue
				if (!(*vertex2)->done) {
					queue.push(*vertex2);
				}
			}
		});

		vertex->done = true;
	}

	/*std::cout << "\nAlle vertices:\n";
	std::for_each(vertices.begin(), vertices.end(), [](Vertex* v)
	{
		std::cout << v->vertex << " weight: " << v->distance << "\n";
	});*/

	std::vector<Vertex*> route = std::vector<Vertex*>();

	// Bepaalde korste route
	auto v = std::find_if(vertices.begin(), vertices.end(), [](Vertex* vertex2) {return vertex2->vertex->getType() == Room::EndEnemy; });
	if (v != vertices.end())
	{
		Vertex* currentVertex = *v;
		while (currentVertex != nullptr) {
			route.push_back(currentVertex);
			currentVertex = currentVertex->previousVertex;
		}
	}

	// Zet de vertices in de goede volgorde (van held naar doel, in plaats van doel naar held)
	std::reverse(route.begin(), route.end());

	/*std::cout << "\nRooms in route:\n";
	std::for_each(route.begin(), route.end(), [](Vertex* v)
	{
		std::cout << v->vertex << "\n";
	});*/

	std::cout << "\nJe haalt het kompas uit je zak. Het trilt in je hand en projecteert in grote lichtgevende letters in de lucht:\n" << std::endl;

	int numberOfTraps = 0;
	int numberOfEnemies = 0;
	std::vector<int> HPs = std::vector<int>();

	// Toont de route die je moet lopen
	for (int i = 0; i < route.size(); i++) {
		std::for_each(route.at(i)->edges.begin(), route.at(i)->edges.end(), [route, i, &numberOfEnemies, &HPs, &numberOfTraps](std::pair<std::string, Room*> exitPair)
		{
			if (i + 1 < route.size()) {
				if (exitPair.second == route.at(i + 1)->vertex) {
					std::cout << exitPair.first;
					if (i != route.size() - 2) {
						std::cout << " - ";
					}

					std::vector<Enemy*> enemies = route.at(i)->vertex->getEnemies();
					numberOfEnemies += enemies.size();
					std::for_each(enemies.begin(), enemies.end(), [&HPs](Enemy* enemy)
					{
						HPs.push_back(enemy->getCurrentHP());
					});

					if (route.at(i)->vertex->getTrap() != nullptr) {
						numberOfTraps++;
					}
				}
			}
		});
	}
	std::cout << "\n";

	// Toont aantal vijanden die je tegenkomt
	if (numberOfEnemies == 1) {
		std::cout << numberOfEnemies << " tegenstander";
	} 
	else {
		std::cout << numberOfEnemies << " tegenstanders";
	}

	// Toont hp van vijanden
	if (numberOfEnemies > 0) {
		std::cout << " (";
		for (int i = 0; i < HPs.size(); i++) {
			std::cout << HPs.at(i) << " hp";
			if (i != HPs.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << ")" << std::endl;
	}

	// Toont aantal vallen
	if (numberOfTraps == 1) {
		std::cout << "\n" << numberOfTraps << " val" << std::endl;
	}
	else {
		std::cout << "\n" << numberOfTraps << " vallen" << std::endl;
	}


	// STAPPENPLAN

	// Zet de eerste vertex op 0, de rest op oneindig (dat is de afstand naar de vertex)

	// Kies de vertex met de laagste afstand en ga alle edges langs

	// (Vertex 1 is de vertex waar je vanaf komt, vertex 2 is de vertex waar je naar toe gaat via de edge)
	// (De weight van een edge is is het totale hp van alle enemies in vertex 2 + eventueel de val)

	// Update de afstand van de vertex 2 als de afstand van vertex 1 + weight van de edge samen LAGER is dan de afstand van vertex 2
	// Als de afstand moet worden geupdate, sla bij vertex 2 vertext 1 op, zodat je later kan terug lopen

	// Als alle edges van een vertex zijn doorlopen, dan aangeven dat de vertex klaar is

	// Als alle vertex dingen geweest zijn, dan kun je terug lopen, om zo de korste route te hebben

}


void Map::getActions(std::vector<std::string>* actions)
{
	actions->push_back("kaart");
	actions->push_back("gebruik kompas");
}

bool Map::handleAction(std::string fullCommand, Hero* hero)
{
	// Acties hier kunnen nooit onderbroken worden door een val, dit lijkt mij ook niet de bedoeling.
	if (fullCommand == "kaart")
	{
		showMap(hero->getCurrentRoom(), false);
		return true;
	}
	if (fullCommand == "gebruik kompas")
	{
		useCompass(hero->getCurrentRoom());
		return true;
	}

	if (fullCommand == "cheatKaart")
	{
		showMap(hero->getCurrentRoom(), true);
		return true;
	}

	if (fullCommand == "handgranaat")
	{
		destroyCorridors(hero->getCurrentRoom()->getLevel());
		return false; // Room moet dit ook nog afhandelen
	}

	return false;
}

void Map::addRoom(Room* room, int x, int y, int z)
{
	rooms_[index(x, y, z)] = room;
}

void Map::destroyCorridors(int z)
{
	std::cout << "handgranaat!\n";
	KruskalMST mst = KruskalMST(getAllRooms(z));

	std::vector<Corridor> corridorsToCollapse = mst.getNonCrucialCorridors();

	std::for_each(corridorsToCollapse.begin(), corridorsToCollapse.end(), [](Corridor c)
	{
		c.Room1->collapseCorridorToRoom(c.Room2);
		c.Room2->collapseCorridorToRoom(c.Room1);
	});
}

Room* Map::getRoom(int x, int y, int z)
{
	return &*rooms_[index(x, y, z)];
}

std::vector<Room*> Map::getAllRooms(int z)
{
	std::vector<Room*> rooms = std::vector<Room*>();

	for (int i = index(0, 0, z); i <= index(xSize_-1, ySize_-1, z); i++)
	{
		rooms.push_back(&*rooms_[i]);
	}

	return rooms;
}

Room* Map::getStartLocation()
{
	return startLocation_;
}

int Map::getSize()
{
	return xSize_ * ySize_ * zSize_;
}

int Map::getXSize()
{
	return xSize_;
}

int Map::getYSize()
{
	return ySize_;
}

int Map::getZSize()
{
	return zSize_;
}

int Map::index(int x, int y, int z)
{
	return ySize_ * xSize_ * z + xSize_ * y + x;
}

