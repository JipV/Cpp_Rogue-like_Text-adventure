#include "stdafx.h"
#include "Map.h"
#include "Random.h"

Map::Map()
	: xSize_{ 0 }, ySize_{ 0 }, zSize_{ 0 }, rooms_{ nullptr }
{
}

Map::Map(int xSize, int ySize, int zSize)
	: xSize_{ xSize }, ySize_{ ySize }, zSize_{ zSize }, rooms_{ nullptr }
{
	rooms_ = new Room[getSize()];
	createMap();
}

Map::~Map()
{
	delete[] rooms_;
	rooms_ = nullptr;
}

// DE KAMERS WORDEN NOG NIET GOED GELINKT
// ER WORDT NOG NIET VOOR GEZORGT DAT ALLE RUIMTES BEREIKBAAR ZIJN
void Map::createMap() 
{
	// Maak kamers aan
	for (int z = 0; z < zSize_; z++)
	{
		for (int y = 0; y < ySize_; y++)
		{
			for (int x = 0; x < xSize_; x++)
			{
				addRoom(new Room(), x, y, z);
			}
		}
	}

	// Start locatie
	//int randomXPosition = rand() % xSize_;
	//getRoom(randomXPosition, 0, 0)->setType(Room::ROOM_TYPE::StartLocation);
	getRoom(Random::getRandomNumber(0, xSize_ - 1), 0, 0)->setType(Room::ROOM_TYPE::StartLocation);

	// Locatie eindvijhand
	//randomXPosition = rand() % xSize_;
	//int randomYPosition = rand() % ySize_;
	//getRoom(randomXPosition, randomYPosition, zSize_ - 1)->setType(Room::ROOM_TYPE::EndEnemy);
	getRoom(Random::getRandomNumber(0, xSize_ - 1), Random::getRandomNumber(0, ySize_ - 1), zSize_ - 1)->setType(Room::ROOM_TYPE::EndEnemy);
	// ER WORDT NOG EINDVIJHAND TOEGEVOEGD AAN DE KAMER

	// Maak trappen aan
	for (int z = 1; z < zSize_; z++)
	{
		//int randomNumberOfStairs = rand() % (xSize_) + 1;
		int randomNumberOfStairs = Random::getRandomNumber(0, xSize_);

		for (int i = 0; i < randomNumberOfStairs; i++) // ER NOG VOOR WORDEN GEZORGD DAT ER EEN NORMAAL AANTAL TRAPPEN KUNNEN WORDEN TOEGEVOEGD (Nu kunnen het er in verhouding veel zijn)
		{
			//int randomXPosition = rand() % xSize_;
			int randomXPosition = Random::getRandomNumber(0, xSize_);
			Room* currentRoom = getRoom(randomXPosition, 0, z);
			Room* roomUp = getRoom(randomXPosition, ySize_ - 1, z - 1);

			currentRoom->addExit("boven", roomUp);
			roomUp->addExit("beneden", currentRoom);

			currentRoom->setType(Room::ROOM_TYPE::StairsUp);
			roomUp->setType(Room::ROOM_TYPE::StairsDown);
		}
	}

	// Maak gangen aan
	for (int z = 0; z < zSize_; z++)
	{
		for (int y = 0; y < ySize_; y++)
		{
			for (int x = 0; x < xSize_; x++)
			{
				Room* currentRoom = getRoom(x, y, z);
				
				// Gangen naar oost en west
				if (x + 1 < xSize_) {
					Room* roomEast = getRoom(x + 1, y, z);

					//if (rand() % 2 == 1)
					if (Random::getRandomNumber(0, 1) == 1)
					{
						currentRoom->addExit("oost", roomEast);
						roomEast->addExit("west", currentRoom);
					}
				}

				// Gangen naar noord en zuid
				if (y + 1 < ySize_) {
					Room* roomSouth = getRoom(x + 1, y, z);

					//if (rand() % 2 == 1)
					if (Random::getRandomNumber(0, 1) == 1)
					{
						currentRoom->addExit("zuid", roomSouth);
						roomSouth->addExit("noord", currentRoom);
					}
				}
			}

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
					{
						std::cout << '-';
					}
					else
					{
						std::cout << ' ';
					}
				}
				else
				{
					std::cout << ". ";
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
					{
						std::cout << "| ";
					}
					else
					{
						std::cout << "  ";
					}
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
	rooms_[index(x, y, z)] = *room;
}

Room* Map::getRoom(int x, int y, int z)
{
	return &rooms_[index(x, y, z)];
}

Room* Map::getStartLocation()
{
	for (int x = 0; x < xSize_; x++)
	{
		Room* room = getRoom(x, 0, 0);
		if (room->getType() == Room::ROOM_TYPE::StartLocation)
		{
			return room;
		}
	}
	return nullptr;
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

