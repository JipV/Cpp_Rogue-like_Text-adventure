#pragma once
#include "Room.h"

class Map;
class Enemy;
class Trap;
class Weapon;
class Shield;

class TrapGenerator
{
public:
	TrapGenerator();
	~TrapGenerator();

	Trap* createTrap(int z);

private:
	std::map<int, std::vector<Trap*>> possibleTraps_;

	void parseLine(std::string line, Trap* trap);
	void saveTrap(Trap* trap);
};

class EnemyGenerator
{
public:
	EnemyGenerator();
	~EnemyGenerator();

	Enemy* createEnemy(int z);
private:
	std::vector<std::string> monsterOptions_;
	std::vector<std::string> bossesOptions_;
	std::vector<std::string> enemySizeOptions_;
};

class WeaponGenerator
{
public:
	WeaponGenerator();
	~WeaponGenerator();

	Weapon* createWeapon(int z);

private:
	std::vector<std::string> weaponOptions_;
	std::vector<std::string> weaponMaterialOptions_;

	std::vector<Weapon*> possibleWeapons_;

	Weapon* parseLine(std::string weaponOption, std::string weaponMaterialOption);
};

class ShieldGenerator
{
public:
	ShieldGenerator();
	~ShieldGenerator();

	Shield* createShield(int z);

private:
	std::vector<std::string> shieldSizeOptions_;
	std::vector<std::string> shieldMaterialOptions_;

	std::vector<Shield*> possibleShields_;

	Shield* parseLine(std::string shieldSizeOption, std::string shieldMaterialOption);
};

class RoomGenerator
{
public:
	RoomGenerator();
	~RoomGenerator();

	void setSpecialRoom(int x, int y, Room::ROOM_TYPE type);
	Room* createRoom(int x, int y, int z);

private:
	int x_, y_;
	Room::ROOM_TYPE specialType_;

	EnemyGenerator* enemyGenerator_;
	TrapGenerator* trapGenerator_;
	WeaponGenerator* weaponGenerator_;
	ShieldGenerator* shieldGenerator_;

	std::vector<std::string> sizeOptions_;
	std::vector<std::string> floorOptions_;
	std::vector<std::string> decorOptions_;
	std::vector<std::string> chestOptions_;
	std::vector<std::string> tidyOptions_;
	std::vector<std::string> lightingOptions_;
	std::vector<std::string> atmosphereOptions_;

	std::string generateDescription();
	void addEnemies(Room* room, int z);
	void addWeapon(Room* room, int z);
	void addShield(Room* room, int z);
	void addTraps(Room* room, int z);
};

class MapGenerator
{
public:
	MapGenerator();
	~MapGenerator();

	Map* generateMap(int xSize, int ySize, int zSize);

private:
	RoomGenerator* roomGenerator_;

	Room* addRoom(int x, int y, int z, Map* map);
};
