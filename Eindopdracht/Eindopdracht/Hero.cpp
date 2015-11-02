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
	chanceToHit_(50), 
	chanceToDefend_(50), 
	attack_(1),  
	mindfulness_(2)
{
	items_ = std::vector<Item*>();
}

Hero::~Hero()
{
	currentRoom_ = nullptr; // Rooms horen bij Map, die zal ze verwijderen

	for (size_t i = 0; i < items_.size(); i++)
		delete items_.at(i);

	delete weapon_;
	weapon_ = nullptr;
	
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
	
	// Val vijand aan
	int totalAttack = attack_;
	if (weapon_ != nullptr) {
		totalAttack += weapon_->getAttack();
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

void Hero::rest()
{
	//RUST UIT
}

void Hero::viewItems()
{
	std::cout << "\nJe beschikt over de volgende spullen:\n";
	for (int i = 0; i < items_.size(); i++) {
		std::cout << "- " << *items_.at(i) << "\n";
	}
}

void Hero::changeWeapon()
{
	std::map<std::string, Weapon*> weaponOptions = std::map<std::string, Weapon*>();
	for (size_t i = 0; i < getWeapons().size(); i++) {
		std::cout << "\nOptie " << i + 1 << ": " << *getWeapons().at(i);
		weaponOptions[std::to_string(i + 1)] = getWeapons().at(i);
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

	weapon_ = weaponOptions.at(weaponNumber);
	if (weapon_ != nullptr) {
		std::cout << "\nJe wapen is nu " << *weapon_ << ".\n";
	}
	else {
		std::cout << "\nJe hebt nu geen wapen vast.\n";
	}
}

void Hero::changeShield()
{
	std::map<std::string, Shield*> shieldOptions = std::map<std::string, Shield*>();
	for (size_t i = 0; i < getShields().size(); i++) {
		std::cout << "\nOptie " << i + 1 << ": " << *getShields().at(i);
		shieldOptions[std::to_string(i + 1)] = getShields().at(i);
	}
	std::cout << "\nOptie " << shieldOptions.size() + 1 << ": geen";
	shieldOptions[std::to_string(shieldOptions.size() + 1)] = nullptr;
	std::cout << "\n";

	std::cout << "\nWelk schild wil je gebruiken?\n";
	std::cout << "(";
	for (size_t i = 0; i < shieldOptions.size(); i++) {
		std::cout << i + 1;
		if (i != shieldOptions.size() - 1) {
			std::cout << " | ";
		}
	}
	std::cout << ")\n";

	std::string shieldNumber;

	bool valid = false;

	while (!valid) {
		std::cout << "\nShield: ";
		std::getline(std::cin, shieldNumber);

		auto it = shieldOptions.find(shieldNumber);
		if (it != shieldOptions.end()) {
			valid = true;
		}
		else
			std::cout << "Het ingevoerde schild is niet geldig. Voer opnieuw een schild in.\n";
	}

	shield_ = shieldOptions.at(shieldNumber);

	if (shield_ != nullptr) {
		std::cout << "\nJe hebt nu " << *shield_ << " vast.\n";
	}
	else {
		std::cout << "\nJe hebt nu geen schild vast.\n";
	}
}

void Hero::viewCharacteristics()
{
	std::cout << "\nEigenschappen held:\n";
	std::cout << "Naam:\t\t\t" << name_ << "\n";
	std::cout << "Level:\t\t\t" << level_ << "\n";
	std::cout << "HP:\t\t\t" << currentHP_ << "\n";
	std::cout << "XP:\t\t\t" << xp_ << "\n";
	std::cout << "Chance to hit:\t\t" << chanceToHit_ << "%\n";
	std::cout << "Chance to defend:\t" << chanceToDefend_ << "%\n";
	std::cout << "Attack:\t\t\t" << attack_ << "\n";
	std::cout << "Mindfulness:\t\t" << mindfulness_ << "\n";
}

void Hero::save()
{
	std::ofstream output_file{ name_ + ".txt" };
	output_file << "name:" << name_ << "\n";
	output_file << "level:" << level_ << "\n";
	output_file << "maxHP:" << maxHP_ << "\n";
	output_file << "currentHP:" << currentHP_ << "\n";
	output_file << "xp:" << xp_ << "\n";
	output_file << "chanceToHit:" << chanceToHit_ << "\n";
	output_file << "chanceToDefend:" << chanceToDefend_ << "\n";
	output_file << "attack:" << attack_ << "\n";
	output_file << "perception:" << mindfulness_ << "\n";
	output_file.close();

	std::cout << "\nJe held is opgeslagen.\n";
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
						std::cout << "\nJe kunt je niet verdedigen, omdat je geen schild hebt en daardoor verlies je " << enemy->getAttack();
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

	if (getWeapons().size() > 0)
	{
		actions->push_back("wissel wapen");
	}

	if (getShields().size() > 0)
	{
		actions->push_back("wissel schild");
	}

	if (items_.size() > 0)
	{
		actions->push_back("bekijk spullen");
	}

	actions->push_back("bekijk eigenschappen");
	actions->push_back("held opslaan");

	currentRoom_->getActions(actions);
}

bool Hero::handleAction(std::string fullCommand, std::vector<std::string> action)
{
	std::string command = action[0];

	if (command == "vecht" && currentRoom_->hasEnemies())
	{
		fight();
		return true;
	}
	if (fullCommand == "wissel wapen")
	{
		changeWeapon();
		return true;
	}
	if (fullCommand == "wissel schild")
	{
		changeShield();
		return true;
	}
	if (fullCommand == "bekijk spullen")
	{
		viewItems();
		return true;
	}
	if (fullCommand == "bekijk eigenschappen")
	{
		viewCharacteristics();
		return true;
	}
	if (fullCommand == "held opslaan")
	{
		save();
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
	// Deze commandos kunnen niet onderbroken worden door vallen
	if (command == "vlucht" && action.size() == 2)
	{
		if (currentRoom_->hasEnemies())
			return flee(action[1]);

		return false;
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


void Hero::setLevel(int level)
{
	level_ = level;
}

void Hero::setMaxHP(int maxHP)
{
	maxHP_ = maxHP;
}

void Hero::setCurrentHP(int currentHP)
{
	currentHP_ = currentHP;
}

void Hero::setXP(int xp)
{
	xp_ = xp;
}

void Hero::setChanceToHit(int chanceToHit)
{
	chanceToHit_ = chanceToHit;
}

void Hero::setChanceToDefend(int chanceToDefend)
{
	chanceToDefend_ = chanceToDefend;
}

void Hero::setAttack(int attack)
{
	attack_ = attack;
}

void Hero::setMindfulness(int mindfulness)
{
	mindfulness_ = mindfulness;
}

std::vector<Weapon*> Hero::getWeapons()
{
	std::vector<Weapon*> weapons = std::vector<Weapon*>();
	for (size_t i = 0; i < items_.size(); i++) {
		if (Weapon* weapon = dynamic_cast<Weapon*>(items_.at(i))) {
			weapons.push_back(weapon);
		}
	}
	return weapons;
}

std::vector<Shield*> Hero::getShields()
{
	std::vector<Shield*> shields = std::vector<Shield*>();
	for (size_t i = 0; i < items_.size(); i++) {
		if (Shield* shield = dynamic_cast<Shield*>(items_.at(i))) {
			shields.push_back(shield);
		}
	}
	return shields;
}