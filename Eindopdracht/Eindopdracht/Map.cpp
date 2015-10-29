#include "stdafx.h"
#include "Map.h"
#include "Random.h"
#include <array>

Map::Map()
	: xSize_{ 0 }, ySize_{ 0 }, zSize_{ 0 }, rooms_{ nullptr }
{
}

Map::Map(int xSize, int ySize, int zSize)
	: xSize_{ xSize }, ySize_{ ySize }, zSize_{ zSize }, rooms_{ nullptr }
{
	rooms_ = new Room*[getSize()]();
	createMap();
}

Map::~Map()
{
	// rooms_ is een array van pointers, eerst moet dus alle inhoud verwijderd worden. delete[] rooms_; is niet genoeg.
	for (int i = 0; i < index(xSize_ - 1, ySize_ - 1, zSize_ - 1); i++)
		delete rooms_[i];

	delete[] rooms_;
	rooms_ = nullptr;
}

void Map::createMap()
{
	// Aantal rooms per verdieping
	int numRooms = xSize_ * ySize_;

	// Genereer een random startpositie
	int x = Random::getRandomNumber(0, xSize_ - 1),
	    y = Random::getRandomNumber(0, ySize_ - 1);

	// Variabelen voor het eindpunt van de vorige verdieping
	int endX = x,
		endY = y;

	// Loop door alle verdiepingen
	for (int z = 0; z < getZSize(); z++)
	{
		// Eerste verdieping, start met een startlocation.
		if (z == 0)
		{
			startLocation_ = new Room();
			startLocation_->setType(Room::ROOM_TYPE::StartLocation);

			addRoom(startLocation_, x, y, z);
		}
		// Niet de eerste verdieping, maak een stairs up
		else
		{
			x = endX;
			y = endY;

			Room* stairsUp = new Room();
			stairsUp->setType(Room::ROOM_TYPE::StairsUp);
			addRoom(stairsUp, x, y, z);

			Room* stairsDown = getRoom(x, y, z - 1);

			stairsUp->addExit("omhoog", stairsDown);
			stairsDown->addExit("omlaag", stairsUp);
		}

		// Genereer random eindpositie die niet gelijk is aan de startpositie
		while (x == endX &&
			y == endY)
		{
			endX = Random::getRandomNumber(0, xSize_ - 1);
			endY = Random::getRandomNumber(0, ySize_ - 1);
		}

		// Genereer de maze voor deze verdieping
		generateRoom(x, y, z);

		if (z < zSize_-1)
			getRoom(endX, endY, z)->setType(Room::ROOM_TYPE::StairsDown);
		else
			getRoom(endX, endY, z)->setType(Room::ROOM_TYPE::EndEnemy);
	}
}

// Genereert een kamer en probeert kamers er rondomheen te genereren. 
// Dit is een aangepast Depth-first-search algoritme
void Map::generateRoom(int x, int y, int z)
{
	Room* currentRoom = getRoom(x, y, z);
	if (currentRoom == nullptr) // Start positie/stairs down is al geset.
	{
		currentRoom = new Room();
		addRoom(currentRoom, x, y, z);
	}

	std::array<std::string, 4> possibleExits = { "noord", "oost", "zuid", "west" };

	for (int i = 3; i >= 0; i--)
	{
		// Kies een random exit om te verwerken
		int exitToCheck = Random::getRandomNumber(0, i);
		std::string currentExit = possibleExits[exitToCheck];
		std::string oppositeExit = "";

		// Zet de huidige exit op zijn plek. De huidige plek word niet meer gebruikt.
		possibleExits[exitToCheck] = possibleExits[i];

		int newX = x,
			newY = y;

		// Bereken nieuwe coordinaten en exit van de nieuwe room
		if (currentExit == "noord")
		{
			newY--;
			oppositeExit = "zuid";
		}
		else if (currentExit == "zuid")
		{
			oppositeExit = "noord";
			newY++;
		}
		else if (currentExit == "oost")
		{
			oppositeExit = "west";
			newX++;
		}
		else if (currentExit == "west")
		{
			oppositeExit = "oost";
			newX--;
		}

		// Als de coordinaten geldig zijn, genereer een nieuwe kamer. 
		if (newX >= 0 &&
			newX < xSize_ &&
			newY >= 0 &&
			newY < ySize_ &&
			isNull(newX, newY, z))
		{
			generateRoom(newX, newY, z); // Recursieve call om een nieuwe room te genereren
			Room* newRoom = getRoom(newX, newY, z);

			currentRoom->addExit(currentExit, newRoom);
			newRoom->addExit(oppositeExit, currentRoom);
		}
	}
}


void Map::showMap(Room* currentRoom)
{
	std::cout << "\nKerker kaart: \n";

	for (int z = 0; z < zSize_; z++)
	{
		for (int y = 0; y < ySize_; y++)
		{
			// Teken kamers + gangen van west naar oost
			for (int x = 0; x < xSize_; x++)
			{
				Room* room = getRoom(x, y, z);

				if (room == currentRoom) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				}

				if (room->getIsVisited())
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
					if (room->getExits().count("oost"))
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
							eastRoom->getExits().count("west"))
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
				if (room->getIsVisited())
				{
					if (room->getExits().count("zuid"))
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
							southRoom->getExits().count("noord"))
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

void Map::addRoom(Room* room, int x, int y, int z)
{
	rooms_[index(x, y, z)] = room;
}

bool Map::isNull(int x, int y, int z)
{
	Room* room = rooms_[index(x, y, z)];
	bool result = rooms_[index(x, y, z)] == nullptr;
	return result;
}

Room* Map::getRoom(int x, int y, int z)
{
	return &*rooms_[index(x, y, z)];
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

