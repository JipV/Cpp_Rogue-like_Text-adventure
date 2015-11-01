#include "stdafx.h"
#include "Hero.h"
#include "Room.h"
#include "Random.h"
#include "Enemy.h"
#include "Item.h"
#include "Weapon.h"
#include "Shield.h"

// ReSharper disable once CppPossiblyUninitializedMember
Hero::Hero(std::string name) : 
	name_(name), 
	level_(0), 
	maxHP_(20), 
	currentHP_(20), 
	xp_(0), 
	chanceToHit_(20), 
	chanceToDefend_(20), 
	attack_(1),  
	mindfulness_(2)
{
	items_ = std::vector<Item*>();
	weapons_ = std::vector<Weapon*>();

	///////////////////////////////////////////////////////////////////////////
	////////////////////////// OM TE TESTEN //////////////////////
	///////////////////////////////////////////////////////////////////////////
	weapons_.push_back(new Weapon("zwaard", "ijzer", 1, 2)); // MOET ER NOG UIT !!!!
	weapons_.push_back(new Weapon("bijl", "staal", 1, 2)); // MOET ER NOG UIT !!!!
	
	shield_ = new Shield("groot", "staal", 1, 2); // MOET ER NOG UIT !!!!
}

Hero::~Hero()
{
	currentRoom_ = nullptr; // Rooms horen bij Map, die zal ze verwijderen

	for (size_t i = 0; i < items_.size(); i++)
		delete items_.at(i);

	for (size_t i = 0; i < weapons_.size(); i++)
		delete weapons_.at(i);

	
	delete shield_;
	shield_ = nullptr;
}

bool Hero::goToRoom(std::string direction)
{
	Room* newRoom = currentRoom_->getExit(direction);

	if (newRoom != nullptr)
	{
		setCurrentRoom(newRoom);
		return true;
	}

	return false;
}

void Hero::fight()
{
	// Bepaal vijand
	Enemy* enemy = nullptr;

	if (currentRoom_->getEnemies().size() > 1) {
		
		std::map<std::string, Enemy*> enemyOptions = std::map<std::string, Enemy*>();
		for (size_t i = 0; i < currentRoom_->getEnemies().size(); i++) {
			std::cout << "\nOptie " << i + 1 << ": " << *currentRoom_->getEnemies().at(i);
			enemyOptions[std::to_string(i + 1)] = currentRoom_->getEnemies().at(i);
		}
		std::cout << "\n";

		std::cout << "\nWelke vijand wil je aanvallen?\n";
		std::cout << "(";
		for (size_t i = 0; i < enemyOptions.size(); i++) {
			std::cout << i + 1;
			if (i != enemyOptions.size() - 1) {
				std::cout << " | ";
			}
		}
		std::cout << ")\n";

		std::string enemyNumber;

		bool valid = false;

		while (!valid) {
			std::cout << "\nEnemy: ";
			std::getline(std::cin, enemyNumber);

			auto it = enemyOptions.find(enemyNumber);
			if (it != enemyOptions.end()) {
				valid = true;
			}
			else
				std::cout << "De ingevoerde vijand is niet geldig. Voer opnieuw een vijand in.\n";
		}

		enemy = enemyOptions.at(enemyNumber);
	}
	else {
		enemy = currentRoom_->getEnemies().at(0);
	}

	// Bepaal wapen
	Weapon* weapon = nullptr;

	if (weapons_.size() > 0) {
		std::map<std::string, Weapon*> weaponOptions = std::map<std::string, Weapon*>();
		for (size_t i = 0; i < weapons_.size(); i++) {
			std::cout << "\nOptie " << i + 1 << ": " << *weapons_.at(i);
			weaponOptions[std::to_string(i + 1)] = weapons_.at(i);
		}
		std::cout << "\nOptie " << weaponOptions.size() + 1 << ": geen";
		weaponOptions[std::to_string(weaponOptions.size() + 1)] = nullptr;
		std::cout << "\n";

		std::cout << "\nWelk wapen wil je gebruiken?\n";
		std::cout << "(";
		for (size_t i = 0; i < weaponOptions.size(); i++) {
			std::cout << i + 1;
			if (i != weaponOptions.size() - 1) {
				std::cout << " | ";
			}
		}
		std::cout << ")\n";

		std::string weaponNumber;

		bool valid = false;

		while (!valid) {
			std::cout << "\nWeapon: ";
			std::getline(std::cin, weaponNumber);

			auto it = weaponOptions.find(weaponNumber);
			if (it != weaponOptions.end()) {
				valid = true;
			}
			else
				std::cout << "Het ingevoerde wapen is niet geldig. Voer opnieuw een wapen in.\n";
		}

		weapon = weaponOptions.at(weaponNumber);
	}
	
	// Val vijand aan
	int totalAttack = attack_;
	if (weapon != nullptr) {
		totalAttack += weapon->getAttack();
	}

	if (Random::getRandomNumber(0, 100) <= chanceToHit_) {
		if (enemy->getAttackedByHero(totalAttack)) {
			std::cout << "\nJe valt de " << *enemy << " aan en doet " << totalAttack << " schade.";
			if (enemy->getIsDefeated()) {
				std::cout << "\nJe hebt de " << *enemy << " verslagen.";
				currentRoom_->removeEnemy(enemy);
			}
			else {
				std::cout << "\nHet aantal levenspunten van de " << *enemy << " is " << enemy->getCurrentHP();
			}
		}
		else {
			std::cout << "\nJe valt de " << *enemy << ", maar hij verdedigt zich, waardoor de aanval mislukt.";
		}
	}
	else {
		std::cout << "\nJe valt de " << *enemy << " aan en mist.";
	}	
	std::cout << "\n";

	// Vijanden vallen aan
	getAttackedByEnemies();
}

bool Hero::flee(std::string direction)
{
	Room* newRoom = currentRoom_->getExit(direction);

	if (newRoom) // nullptr == false
	{
		for (size_t i = 0; i < currentRoom_->getEnemies().size(); i++) {
			if (Random::getRandomNumber(0, 100) <= currentRoom_->getEnemies().at(i)->getChanceHeroEscapes()) {
				std::cout << "\nJe wordt tegengehouden door een vijand en kunt daardoor niet vluchten.\n";

				// Vijanden vallen aan
				getAttackedByEnemies();

				return true;
			}
		}
		
		std::cout << "\nHet lukt de vijanden niet om je tegen te houden, waardoor het lukt je om te vluchten.\n";
		setCurrentRoom(newRoom);

		return true;
	}
	
	return false;
}

void Hero::search()
{
	//ZOEK
}

void Hero::rest()
{
	//RUST UIT
}

void Hero::viewItems()
{
	//BEKIJK ITEMS
}

void Hero::getAttackedByEnemies()
{
	// TODO: GAME OVER

	for (size_t i = 0; i < currentRoom_->getEnemies().size(); i++) {
		if (!isDefeated_) {
			Enemy* enemy = currentRoom_->getEnemies().at(i);
			std::cout << "\nJe wordt aangevallen door de " << *enemy << ".\n";

			if (Random::getRandomNumber(0, 100) <= enemy->getChanceToHit()) {
				if (shield_ != nullptr && Random::getRandomNumber(0, 100) <= chanceToDefend_) {
					
					int totalEnemyAttack = enemy->getAttack() - shield_->getDefence();

					if (totalEnemyAttack <= 0) {
						totalEnemyAttack = 0;
					}

					if (totalEnemyAttack == 1) {
						std::cout << "\nJe gebruikt je schild, waardoor je maar " << totalEnemyAttack << " levenspunt verliest.\n";
					}
					else {
						std::cout << "\nJe gebruikt je schild, waardoor je maar " << totalEnemyAttack << " levenspunten verliest.\n";
					}	
				}
				else {
					currentHP_ -= enemy->getAttack();
					
					if (shield_ != nullptr) {
						std::cout << "\nJe verdedigt je niet goed en verliest daardoor " << enemy->getAttack();
					}
					else {
						std::cout << "\nJe kunt je niet verdedigen, omdat je geen schild hebt en daardoor verliest je " << enemy->getAttack();
					}

					if (enemy->getAttack() == 1) {
						std::cout << " levenspunt.\n";
					}
					else {
						std::cout << " levenspunten.\n";
					}
				}

				// Check of de speler verslagen is
				if (currentHP_ <= 0) {
					currentHP_ = 0;
					isDefeated_ = true;
				}
			}
			else {
				std::cout << "\nJe hebt gelukt. De " << *enemy << " mist.\n";
			}
		}
	}
	std::cout << "\nJe hebt nog " << currentHP_ << " levenspunten over.\n";
}

void Hero::takeDirectDamage(int damage)
{
	currentHP_ -= damage;

	if (currentHP_ <= 0)
	{
		std::cout << "Je bent dood!" << std::endl;

		currentHP_ = 0;
		isDefeated_ = true;
	}

	std::cout << "\nJe hebt nog " << currentHP_ << " levenspunten over.\n";
}

void Hero::addItem(Item* item)
{
	items_.push_back(item);
}

void Hero::removeItem(Item* item)
{
	items_.erase(std::remove(items_.begin(), items_.end(), item), items_.end());
}

void Hero::getActions(std::vector<std::string>* actions)
{
	if (currentRoom_->hasEnemies())
	{
		actions->push_back("vlucht [richting]");
		actions->push_back("vecht");
	}
	else 
	{
		actions->push_back("loop [richting]");
	}

	currentRoom_->getActions(actions);
}

bool Hero::handleAction(std::string fullCommand, std::vector<std::string> action)
{
	std::string command = action[0];

	// Deze commandos kunnen niet onderbroken worden door vallen
	if (command == "vlucht" && action.size() == 2)
	{
		if (currentRoom_->hasEnemies())
			return flee(action[1]);

		return false;
	}
	if (command == "vecht" && currentRoom_->hasEnemies())
	{
		fight();
		return true;
	}

	// Handel room actions af en kijk of er een val af gaat
	if (currentRoom_->handleAction(fullCommand, action, this))
		return true;

	// Deze commandos kunnen wel onderbroken worden door vallen
	if (command == "loop" && action.size() == 2)
	{
		if (currentRoom_->hasEnemies())
			return false;

		return goToRoom(action[1]);
	}

	return false;
}

Room* Hero::getCurrentRoom()
{
	return currentRoom_;
}

void Hero::setCurrentRoom(Room* room)
{
	currentRoom_ = room;
	currentRoom_->showDescription();
	room->setIsVisited(true);
}

std::vector<Item*> Hero::getItems()
{
	return items_;
}
