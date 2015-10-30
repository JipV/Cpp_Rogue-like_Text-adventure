#ifndef __ROOM_H__
#define __ROOM_H__

class Enemy;
class Item;
class Trap;

class Room
{
	friend class MapGenerator;
	friend class RoomGenerator;
	public:
		enum ROOM_TYPE {
			StartLocation,
			NormalRoom,
			StairsDown,
			StairsUp,
			EndEnemy
		};

		Room(ROOM_TYPE type, std::string description);
		~Room();

		void showDescription();

		void getActions(std::vector<std::string>* actions);
		bool handleAction(std::string fullCommand, std::vector<std::string> action);

		Room* getExit(std::string name);
		std::map<std::string, Room*> getAllExits();

		bool hasEnemies();

		bool getIsVisited();
		void setIsVisited(bool isVisited);

		ROOM_TYPE getType();

		

	private:

		bool isVisited_;

		ROOM_TYPE type_;

		std::string description_;

		std::map<std::string, Room*> exits_;

		std::vector<Enemy*>* enemies_ = nullptr;
		std::vector<Item*>* items_ = nullptr;
		Trap* trap_ = nullptr;

		void addExit(std::string name, Room* room);
		void addEnemy(Enemy* enemy);
		void setTrap(Trap* trap);
};

#endif
