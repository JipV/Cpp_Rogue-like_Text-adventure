#include "stdafx.h"
#include "Map.h"
#include "Room.h"
#include "Hero.h"
#include "KruskalMSP.h"
#include "Graph.h"
#include "Random.h"

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
				else if (room->isCollapsed("oost"))
					std::cout << '~';
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
					else if (eastRoom->getIsVisited() &&
						eastRoom->isCollapsed("west"))
						std::cout << '~';
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
				else if (room->isCollapsed("zuid"))
					std::cout << "~ ";
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
					else if (southRoom->getIsVisited() &&
						southRoom->isCollapsed("noord"))
						std::cout << "~ ";
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
	std::cout << "~  : Ingestortte gang \n";
	std::cout << "S  : Start locatie \n";
	std::cout << "E  : Eind vijhand \n";
	std::cout << "N  : Normale ruimte \n";
	std::cout << "L  : Trap omlaag \n";
	std::cout << "H  : Trap omhoog \n";
	std::cout << ".  : Niet bezocht \n";
}

void Map::getActions(std::vector<std::string>* actions)
{
	actions->push_back("kaart");
	actions->push_back("handgranaat");
}

bool Map::handleAction(std::string fullCommand, Hero* hero)
{
	// Acties hier kunnen nooit onderbroken worden door een val, dit lijkt mij ook niet de bedoeling.
	if (fullCommand == "kaart")
	{
		showMap(hero->getCurrentRoom(), false);
		return true;
	}

	if (fullCommand == "cheatKaart")
	{
		showMap(hero->getCurrentRoom(), true);
		return true;
	}

	if (fullCommand == "handgranaat")
	{
		destroyCorridors(hero->getCurrentRoom());
		return true;
	}

	return false;
}

void Map::addRoom(Room* room, int x, int y, int z)
{
	rooms_[index(x, y, z)] = room;
}

void Map::destroyCorridors(Room* currentRoom)
{
	KruskalMST mst = KruskalMST(getAllRooms(currentRoom->getLevel()));

	std::vector<Corridor> corridorsToCollapse = mst.getNonCrucialCorridors();
	size_t amountToCollapse = Random::getRandomNumber(10, 15);

	if (corridorsToCollapse.size() == 0)
	{
		std::cout << "Je vreest dat een extra handgranaat een cruciale passage zal blokkeren. Het is beter om deze niet meer te gebruiken op deze verdieping." << std::endl;
	}
	else
	{
		std::cout << "De kerker schudt op zijn grondvesten, alle tegenstanders in de kamer zijn verslagen! Een donderend geluid maakt duidelijk dat gedeeltes van de kerker zijn ingestort..." << std::endl;
		currentRoom->removeAllEnemies();

		// Als alles moet instorten, hoeven we geen random kamers te selecteren.
		if (corridorsToCollapse.size() <= amountToCollapse)
		{
			std::for_each(corridorsToCollapse.begin(), corridorsToCollapse.end(), [](Corridor c)
			{
				c.Room1->collapseCorridorToRoom(c.Room2);
				c.Room2->collapseCorridorToRoom(c.Room1);
			});
		}
		else
		{
			for (size_t i = 0; i < amountToCollapse; i++)
			{
				int index = Random::getRandomNumber(0, corridorsToCollapse.size() - 1);

				Corridor c = corridorsToCollapse.at(index);
				c.Room1->collapseCorridorToRoom(c.Room2);
				c.Room2->collapseCorridorToRoom(c.Room1);
				corridorsToCollapse.erase(corridorsToCollapse.begin() + index);
			}
		}
	}
	
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

